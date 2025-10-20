#include <iostream>
#include <limits>
#include "6502_emulator.hpp"


int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    std::cout << "Select demo:\n";
    std::cout << "  1) Calculator (+/-)\n";
    std::cout << "  2) Fibonacci sequence generator\n";
    std::cout << "> ";
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cerr << "Invalid input\n";
        return 1;
    }

    if (choice == 1) {
        // Calculator demo
        int operand1 = 0;
        int operand2 = 0;
        char operation = 0;

        std::cout << "Enter first operand (0-255): ";
        if (!(std::cin >> operand1)) {
            std::cerr << "Invalid input for first operand\n";
            return 1;
        }
        std::cout << "Enter second operand (0-255): ";
        if (!(std::cin >> operand2)) {
            std::cerr << "Invalid input for second operand\n";
            return 1;
        }
        std::cout << "Enter operation (+ or -): ";
        if (!(std::cin >> operation)) {
            std::cerr << "Invalid input for operation\n";
            return 1;
        }

        constexpr Word operand1_loc = 0x0010;
        constexpr Word operand2_loc = 0x0011;
        constexpr Word result_loc   = 0x0012;

        mem.Data[operand1_loc] = static_cast<Byte>(operand1);
        mem.Data[operand2_loc] = static_cast<Byte>(operand2);

        Byte program[10];
        size_t program_size = 0;

        if (operation == '+') {
            program[program_size++] = static_cast<Byte>(CPU::Opcode::LDA_ABS);
            program[program_size++] = operand1_loc & 0xFF;
            program[program_size++] = (operand1_loc >> 8) & 0xFF;
            program[program_size++] = static_cast<Byte>(CPU::Opcode::ADC_ABS);
            program[program_size++] = operand2_loc & 0xFF;
            program[program_size++] = (operand2_loc >> 8) & 0xFF;
            program[program_size++] = static_cast<Byte>(CPU::Opcode::STA_ABS);
            program[program_size++] = result_loc & 0xFF;
            program[program_size++] = (result_loc >> 8) & 0xFF;
        } else if (operation == '-') {
            program[program_size++] = static_cast<Byte>(CPU::Opcode::LDA_ABS);
            program[program_size++] = operand1_loc & 0xFF;
            program[program_size++] = (operand1_loc >> 8) & 0xFF;
            program[program_size++] = static_cast<Byte>(CPU::Opcode::SBC_ABS);
            program[program_size++] = operand2_loc & 0xFF;
            program[program_size++] = (operand2_loc >> 8) & 0xFF;
            program[program_size++] = static_cast<Byte>(CPU::Opcode::STA_ABS);
            program[program_size++] = result_loc & 0xFF;
            program[program_size++] = (result_loc >> 8) & 0xFF;
        } else {
            std::cerr << "Invalid operation!\n";
            return 1;
        }

        for (size_t i = 0; i < program_size; i++) {
            mem.Data[0x0000 + i] = program[i];
        }

        cpu.PC = 0x0000;
        cpu.Execute(10, mem);

        std::cout << "Result: " << static_cast<int>(static_cast<SByte>(mem.Data[result_loc])) << "\n";
        return 0;
    }

   if (choice == 2) {
       // Fibonacci sequence demo
       int n = 0;
       std::cout << "Enter n (0-20): ";
       if (!(std::cin >> n) || n < 0 || n > 20) {
           std::cerr << "Invalid input for n\n";
           return 1;
       }

       constexpr Word n_loc = 0x0010;
       constexpr Word result_loc = 0x0011;

       mem.Data[n_loc] = static_cast<Byte>(n);

       Byte program[] = {
           0xA9, 0x00,       // LDA #$00
           0x85, result_loc, // STA result
           0xA9, 0x01,       // LDA #$01
           0x85, 0x12,       // STA $12 (prev)
           0xA6, n_loc,      // LDX n (ZP)
           0xE0, 0x00,       // CPX #$00
           0xF0, 0x17,       // BEQ end_zero (to BRK)
           0xE0, 0x01,       // CPX #$01
           0xF0, 0x14,       // BEQ end_one
           0xCA,             // DEX (X = n-1)
           // loop:
           0x18,             // CLC
           0xA5, result_loc, // LDA result
           0x65, 0x12,       // ADC $12 (prev)
           0x85, 0x13,       // STA $13 (sum)
           0xA5, result_loc, // LDA result
           0x85, 0x12,       // STA $12 (prev = old result)
           0xA5, 0x13,       // LDA $13 (sum)
           0x85, result_loc, // STA result
           0xCA,             // DEX
           0xD0, 0xEE,       // BNE loop (relative -18)
           // end_zero:
           0x00,             // BRK
           // end_one:
           0xA9, 0x01,       // LDA #$01
           0x85, result_loc, // STA result
           0x00              // BRK
       };

       for (size_t i = 0; i < sizeof(program); ++i) {
           mem.Data[0x0000 + i] = program[i];
       }

       cpu.PC = 0x0000;
       cpu.Execute(10000, mem);

       std::cout << "Fibonacci(" << n << ") = " << static_cast<int>(mem.Data[result_loc]) << "\n";
       return 0;
   }

    std::cerr << "Unknown choice\n";
    return 1;
}

