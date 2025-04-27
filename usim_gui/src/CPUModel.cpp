// === File: CPUModel.cpp ===
#include "CPUModel.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

CPUModel::CPUModel(const std::string& hexfile)
  : cpu()//, terminal(cpu)
  , ram(std::make_shared<RAM>(ram_size))
  , rom(std::make_shared<ROM>(rom_size))
  //, serial(std::make_shared<xr88c92>(terminal))
  , hex_path(hexfile)
{
    cpu.attach(ram, 0x0000, ~(ram_size - 1));
    cpu.attach(rom, rom_base, ~(rom_size - 1));
    //cpu.attach(serial, io_base, io_mask);
    //cpu.FIRQ.bind([&](){ return serial->IRQ; });
}

CPUModel::~CPUModel() {
    stop();
}

bool CPUModel::init() {
    if (!rom->load_intelhex(hex_path.c_str(), rom_base)) {
        std::cerr << "Failed to load HEX file: " << hex_path << std::endl;
        return false;
    }
    cpu.reset();
    return true;
}

void CPUModel::startRunLoop() {
    running = true;
    setNonBlocking(true);
    cpu_thread = std::thread([this] {
        while (running) {
            cpu.run(cycle_step);
            std::this_thread::sleep_for(std::chrono::milliseconds(pause_ms));
        }
    });
}

void CPUModel::stop() {
    running = false;
    cpu.halt();
    if (cpu_thread.joinable()) cpu_thread.join();
    setNonBlocking(false);
}

mc6309::Registers CPUModel::getRegisters() const {
    return cpu.get_registers();
}

void CPUModel::setNonBlocking(bool enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if (enable) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO;
        ttystate.c_cc[VMIN] = 0;
        ttystate.c_cc[VTIME] = 0;
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO;
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}