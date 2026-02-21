#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <raylib.h>

uint8_t reg[0x0F]{};

uint16_t i{};

uint8_t memory[0x1000];

uint16_t pc{};

uint32_t display[0x1f][0x3f];

void print_memory(uint16_t start, uint16_t end) {
    std::cout << "\n";
    for (size_t i = start; i < end; i += 0x10) {
        std::cout << std::format("0x{:04x}: ", i);
        for (size_t j = 0; j < 0x10; j++) {
            std::cout << std::format("{:02x} ", memory[i + j]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void print_registers() {
    std::cout << "\n";
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            uint8_t r = i * 4 + j;
            std::cout << std::format("reg[0x{:02x}]: 0x{:02x} ", r, reg[r]);
        }
        std::cout << "\n";
    }
    std::cout << std::format("i: 0x{:04x} ", i);
    std::cout << "\n";
}

void init_memory() {
    memory[0x00] = 0xf0;
    memory[0x01] = 0x90;
    memory[0x02] = 0x90;
    memory[0x03] = 0x90;
    memory[0x04] = 0xf0;

    memory[0x10] = 0x20;
    memory[0x11] = 0x60;
    memory[0x12] = 0x20;
    memory[0x13] = 0x20;
    memory[0x14] = 0x70;

    memory[0x20] = 0xf0;
    memory[0x21] = 0x10;
    memory[0x22] = 0xf0;
    memory[0x23] = 0x80;
    memory[0x24] = 0xf0;

    memory[0x30] = 0xf0;
    memory[0x31] = 0x10;
    memory[0x32] = 0xf0;
    memory[0x33] = 0x10;
    memory[0x34] = 0xf0;

    memory[0x40] = 0x90;
    memory[0x41] = 0x90;
    memory[0x42] = 0xf0;
    memory[0x43] = 0x10;
    memory[0x44] = 0x10;

    memory[0x50] = 0xf0;
    memory[0x51] = 0x80;
    memory[0x52] = 0xf0;
    memory[0x53] = 0x10;
    memory[0x54] = 0xf0;

    memory[0x60] = 0xf0;
    memory[0x61] = 0x80;
    memory[0x62] = 0xf0;
    memory[0x63] = 0x90;
    memory[0x64] = 0xf0;

    memory[0x70] = 0xf0;
    memory[0x71] = 0x10;
    memory[0x72] = 0x20;
    memory[0x73] = 0x40;
    memory[0x74] = 0x40;

    memory[0x80] = 0xf0;
    memory[0x81] = 0x90;
    memory[0x82] = 0xf0;
    memory[0x83] = 0x90;
    memory[0x84] = 0xf0;

    memory[0x90] = 0xf0;
    memory[0x91] = 0x90;
    memory[0x92] = 0xf0;
    memory[0x93] = 0x10;
    memory[0x94] = 0xf0;

    memory[0xa0] = 0xf0;
    memory[0xa1] = 0x90;
    memory[0xa2] = 0xf0;
    memory[0xa3] = 0x90;
    memory[0xa4] = 0x90;

    memory[0xb0] = 0xe0;
    memory[0xb1] = 0x90;
    memory[0xb2] = 0xe0;
    memory[0xb3] = 0x90;
    memory[0xb4] = 0xe0;

    memory[0xc0] = 0xf0;
    memory[0xc1] = 0x80;
    memory[0xc2] = 0x80;
    memory[0xc3] = 0x80;
    memory[0xc4] = 0xf0;

    memory[0xd0] = 0xe0;
    memory[0xd1] = 0x90;
    memory[0xd2] = 0x90;
    memory[0xd3] = 0x90;
    memory[0xd4] = 0xe0;

    memory[0xe0] = 0xf0;
    memory[0xe1] = 0x80;
    memory[0xe2] = 0xf0;
    memory[0xe3] = 0x80;
    memory[0xe4] = 0xf0;

    memory[0xf0] = 0xf0;
    memory[0xf1] = 0x80;
    memory[0xf2] = 0xf0;
    memory[0xf3] = 0x80;
    memory[0xf4] = 0x80;
}

uint16_t read_instruction() {
    uint8_t high = memory[pc++];
    uint8_t low = memory[pc++];

    return (high << 8) | low;
}

void step() {
    uint16_t instruction = read_instruction();
    std::cout << std::format("0x{:04x}: ", instruction);

    if ((instruction >> 12) == 0x6) {
        uint8_t r = instruction >> 8 & 0x0F;
        uint8_t nn = instruction << 8;
        std::cout << std::format("reg[0x{:02x}] <- 0x{:02x}", r, nn) << "\n";

        reg[r] = nn;

    } else if ((instruction >> 12) == 0xc) {
        uint8_t r = instruction >> 8 & 0x0F;
        uint8_t mask = instruction & 0xFF;

        uint8_t num = std::rand();
        reg[r] = num & mask;

        std::cout << std::format(
                         "reg[0x{:02x}] <- 0x{:02x} & 0x{:02x} = 0x{:02x}", r,
                         num, mask, reg[r])
                  << "\n";

    } else if (instruction == 0x00e0) {
        std::cout << "clear screen\n";

    } else if ((instruction >> 12) == 0xa) {
        uint16_t addr = instruction & 0x7FF;
        std::cout << std::format("i <- 0x{:04x}", addr) << "\n";
        i = addr;

    } else if ((instruction >> 12) == 0xf) {
        if ((instruction & 0x00FF) == 0x0033) {
            uint8_t r = instruction >> 8 & 0x0F;
            std::cout << "\n";
            std::cout << std::format("    i: 0x{:04x} <- {:d}", i, reg[r] % 10)
                      << "\n";
            std::cout << std::format("    i + 1: 0x{:04x} <- {:d}", i + 1,
                                     (reg[r] / 10) % 10)
                      << "\n";
            std::cout << std::format("    i + 2: 0x{:04x} <- {:d}", i + 2,
                                     reg[r] / 100)
                      << "\n";

            memory[i] = reg[r] % 10;
            memory[i + 1] = (reg[r] / 10) % 10;
            memory[i + 2] = reg[r] / 100;

        } else if ((instruction & 0x00FF) == 0x0065) {
            uint8_t r = instruction >> 8 & 0x0F;
            std::cout << "\n";
            for (size_t j = 0; j <= r; j++) {
                std::cout
                    << std::format(
                           "    reg[0x{:02x}] <- i+{}[0x{:04x}]: 0x{:02x}: ", j,
                           j, i + j, memory[i + j])
                    << "\n";
                reg[j] = memory[i + j];
            }
        } else if ((instruction & 0x00FF) == 0x0029) {
            uint8_t r = instruction >> 8 & 0x0F;
            std::cout << "\n";
            std::cout << std::format("    i: 0x{:04x} <- 0x{:04x}", i,
                                     reg[r] * 0x10)
                      << "\n";

            i = reg[r] * 0x10;

        } else {
            std::cout << "unimplemented\n";
        }

    } else if ((instruction >> 12) == 0xd) {
        uint8_t r_x = (instruction >> 8) & 0x0F;
        uint8_t r_y = (instruction >> 4) & 0x0F;
        uint8_t n = instruction & 0x0F;

        uint8_t start_x = reg[r_x];
        uint8_t start_y = reg[r_y];

        for (size_t p = 0; p < n; p++) {
            for (size_t b = 0; b < 8; b++) {
                uint8_t pixel = (memory[i + p] >> (7 - b)) & 0x01;
                int y = (start_y + p) % 0x1f;
                int x = (start_x + b) % 0x3f;

                display[y][x] = pixel;
            }
        }
        std::cout << "printing\n";
    } else if ((instruction >> 12) == 0x7) {
        uint8_t r = instruction >> 8 & 0x0F;
        uint8_t nn = instruction & 0xFF;

        std::cout << std::format("reg[0x{:02x}]: 0x{:02x} += 0x{:02x}", r,
                                 reg[r], nn)
                  << "\n";

        reg[r] += nn;

    } else {
        std::cout << "unimplemented\n";
    }
}

int main(void) {
    std::srand(std::time({}));
    std::ifstream f{"./random_number_test.ch8", std::ios::binary};
    if (!f) {
        std::cerr << "Failed to load file";
        return 1;
    }

    init_memory();
    uint16_t p = 0x0200;
    while (f >> memory[p]) {
        p++;
    }

    bool c = false;

    InitWindow(0x3f * 16, 0x1f * 16, "CHIP-8");

    // print_memory();
    //

    pc = 0x0200;
    while (!WindowShouldClose()) {
        BeginDrawing();

        if (IsKeyPressed(KEY_SPACE) || c) {
            step();
        } else if (IsKeyPressed(KEY_C)) {
            c = !c;
        } else if (IsKeyPressed(KEY_R)) {
            print_registers();
        } else if (IsKeyPressed(KEY_M)) {
            print_memory(i, i + 16);
        }

        for (size_t y = 0; y < 0x1f; y++) {
            for (size_t x = 0; x < 0x3f; x++) {
                Color c = display[y][x] == 0 ? BLACK : WHITE;
                DrawRectangle(x * 16, y * 16, 16, 16, c);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    f.close();

    return 0;
}
