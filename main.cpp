//
//	main.cpp
//
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>
#include <argparse/argparse.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <termios.h>
#include <fcntl.h>

#include "mc6809.h"
#include "mc6309.h"
#include "xr88c92.h"
#include "term.h"
#include "memory.h"

std::atomic<bool> running{true};

// Make getchar non-blocking
void set_nonblocking(bool enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if (enable) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO;
        ttystate.c_cc[VMIN] = 0;
        ttystate.c_cc[VTIME] = 0;
    } else {
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    if (enable) {
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("usim");

    try {
        program.add_argument("-h", "--hexfile")
            .required()
            .help("Path to the hex file");

        program.parse_args(argc, argv);
        std::string hexfile = program.get<std::string>("-h");

        printf("Hex file: %s\n", hexfile.c_str());
    } catch (const std::exception &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        const char* help = program.help().str().c_str();
        fprintf(stderr, "%s\n", help);
        return EXIT_FAILURE;
    }

    using Word = uint16_t;

    const Word ram_size = 0x8000;
    const Word rom_base = 0xe000;
    const Word rom_size = 0x10000 - rom_base;

    mc6309 cpu;
    Terminal term(cpu);

    auto ram = std::make_shared<RAM>(ram_size);
    auto rom = std::make_shared<ROM>(rom_size);
    auto serial = std::make_shared<xr88c92>(term);

    cpu.attach(ram, 0x0000, ~(ram_size - 1));
    cpu.attach(rom, rom_base, ~(rom_size - 1));
    cpu.attach(serial, 0xf420, 0xfff0);

    cpu.FIRQ.bind([&]() {
		return serial->IRQ;
	});

	rom->load_intelhex(argv[2], rom_base);

    cpu.reset();

    std::thread cpu_thread([&cpu] {
        cpu.run(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cpu.run(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cpu.run(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cpu.run(100);
    });

    set_nonblocking(true);
    std::cout << "Press 's' to halt CPU..." << std::endl;

    while (running) {
        int ch = getchar();
        if (ch == 's' || ch == 'S') {
            std::cout << "\nHalting CPU..." << std::endl;
            cpu.halt();  // Make sure this safely stops the loop inside cpu.run()
            running = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cpu_thread.join();
    set_nonblocking(false);

    return EXIT_SUCCESS;
}