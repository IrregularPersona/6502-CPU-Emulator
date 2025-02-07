#include <stdio.h>
#include "../include/6502_emulator.hpp"

int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    // User input
    int operand1, operand2;
    char operation;

    printf("Enter first operand: ");
    scanf("%d", &operand1);
    printf("Enter second operand: ");
    scanf("%d", &operand2);
    fflush(stdin);
    printf("Enter operation (+ or -): ");
    scanf(" %c", &operation);
    
    // Store operands in memory
    constexpr Word operand1_loc = 0x0010;
    constexpr Word operand2_loc = 0x0011;
    constexpr Word result_loc = 0x0012;

    mem.Data[operand1_loc] = operand1; // Store first operand
    mem.Data[operand2_loc] = operand2; // Store second operand

    // Generate program based on operation
    Byte program[10];
    size_t program_size = 0;

    if (operation == '+') 
    {
      program[program_size++] = CPU::INS_LDA_ABS; // Load A => operand1
      program[program_size++] = operand1_loc & 0xFF;
      program[program_size++] = (operand1_loc >> 8) & 0xFF;
        
      program[program_size++] = CPU::INS_ADC_ABS; // Add operand2
      program[program_size++] = operand2_loc & 0xFF;
      program[program_size++] = (operand2_loc >> 8) & 0xFF;
        
      program[program_size++] = CPU::INS_STA_ABS; // Store result
      program[program_size++] = result_loc & 0xFF;
      program[program_size++] = (result_loc >> 8) & 0xFF;
        
    } else if (operation == '-') 
    {
      program[program_size++] = CPU::INS_LDA_ABS; // Load A => operand1
      program[program_size++] = operand1_loc & 0xFF;
      program[program_size++] = (operand1_loc >> 8) & 0xFF;
      
      program[program_size++] = CPU::INS_SBC_ABS; // Subtract operand2
      program[program_size++] = operand2_loc & 0xFF;
      program[program_size++] = (operand2_loc >> 8) & 0xFF;
      
      program[program_size++] = CPU::INS_STA_ABS; // Store result
      program[program_size++] = result_loc & 0xFF;
      program[program_size++] = (result_loc >> 8) & 0xFF;

    } else 
    {
        puts("Invalid operation!\n");
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
    printf("Result: %d\n", (int)(SByte)mem.Data[result_loc]);
    
    return 0;
}

