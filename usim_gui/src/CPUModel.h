// === File: CPUModel.h ===
#pragma once

#include <atomic>
#include <thread>
#include <memory>
#include <string>
#include "mc6309.h"
#include "memory.h"
//#include "xr88c92.h"
//#include "term.h"

class CPUModel {
public:
    CPUModel(const std::string& hexfile);
    ~CPUModel();

    bool init();
    void startRunLoop();
    void stop();

    mc6309::Registers getRegisters() const;

private:
    void setNonBlocking(bool enable);

    static constexpr uint16_t ram_size = 0x8000;
    static constexpr uint16_t rom_base = 0xE000;
    static constexpr uint16_t rom_size = 0x10000 - rom_base;
    static constexpr uint16_t io_base = 0xF400;
    // static constexpr uint16_t io_mask = 0xFFF0;
    static constexpr int cycle_step = 100;
    static constexpr int pause_ms = 1000;

    mc6309                    cpu;
    // Terminal                  terminal;
    std::shared_ptr<RAM>      ram;
    std::shared_ptr<ROM>      rom;
    // std::shared_ptr<xr88c92>  serial;
    std::string               hex_path;

    std::atomic<bool>         running{false};
    std::thread               cpu_thread;
};