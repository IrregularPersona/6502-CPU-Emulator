#include <iostream>
#include <limits>
#include "6502_emulator.hpp"


int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    // User input
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
    
    // Store operands in memory
    constexpr Word operand1_loc = 0x0010;
    constexpr Word operand2_loc = 0x0011;
    constexpr Word result_loc = 0x0012;

    mem.Data[operand1_loc] = static_cast<Byte>(operand1); // Store first operand
    mem.Data[operand2_loc] = static_cast<Byte>(operand2); // Store second operand

    // Generate program based on operation
    Byte program[10];
    size_t program_size = 0;

    if (operation == '+') 
    {
      program[program_size++] = static_cast<Byte>(CPU::Opcode::LDA_ABS); // Load A => operand1
      program[program_size++] = operand1_loc & 0xFF;
      program[program_size++] = (operand1_loc >> 8) & 0xFF;
        
      program[program_size++] = static_cast<Byte>(CPU::Opcode::ADC_ABS); // Add operand2
      program[program_size++] = operand2_loc & 0xFF;
      program[program_size++] = (operand2_loc >> 8) & 0xFF;
        
      program[program_size++] = static_cast<Byte>(CPU::Opcode::STA_ABS); // Store result
      program[program_size++] = result_loc & 0xFF;
      program[program_size++] = (result_loc >> 8) & 0xFF;
        
    } else if (operation == '-') 
    {
      program[program_size++] = static_cast<Byte>(CPU::Opcode::LDA_ABS); // Load A => operand1
      program[program_size++] = operand1_loc & 0xFF;
      program[program_size++] = (operand1_loc >> 8) & 0xFF;
      
      program[program_size++] = static_cast<Byte>(CPU::Opcode::SBC_ABS); // Subtract operand2
      program[program_size++] = operand2_loc & 0xFF;
      program[program_size++] = (operand2_loc >> 8) & 0xFF;
      
      program[program_size++] = static_cast<Byte>(CPU::Opcode::STA_ABS); // Store result
      program[program_size++] = result_loc & 0xFF;
      program[program_size++] = (result_loc >> 8) & 0xFF;

    } else 
    {
        std::cerr << "Invalid operation!\n";
        return 1;
    }

    // Load program into memory
    for (size_t i = 0; i < program_size; i++) 
    {
        mem.Data[0x0000 + i] = program[i];
    }

    cpu.PC = 0x0000;

    cpu.Execute(10, mem);

    // Output the result
    std::cout << "Result: " << static_cast<int>(static_cast<SByte>(mem.Data[result_loc])) << "\n";
    
    return 0;
}

