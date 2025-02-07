#include <stdlib.h>

using Byte = unsigned char;
using SByte = signed char;
using Word = unsigned short;
using u32 = unsigned int;

struct Mem 
{
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Initialize() 
  {
    for (u32 i = 0; i < MAX_MEM; i++) 
    {
      Data[i] = 0;
    }
  }

  // read 1 byte
  Byte operator[](u32 Address) const { return Data[Address]; }

  // write 1 byte
  Byte &operator[](u32 Address) { return Data[Address]; }
};

struct CPU 
{

  Word PC; // Program Counter
  Word SP; // Stack Pointer

  Byte A, X, Y;

  Byte C : 1; // Status Flag
  Byte Z : 1; // Status Flag
  Byte I : 1; // Status Flag
  Byte D : 1; // Status Flag
  Byte B : 1; // Status Flag
  Byte V : 1; // Status Flag
  Byte N : 1; // Status Flag

  void Reset(Mem &memory) 
  {
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = 0;
    A = X = Y = 0;
    memory.Initialize();
  }

  Byte FetchByte(u32 &Cycles, Mem &memory) 
  {
    Byte Data = memory[PC];
    PC++;
    Cycles--;
    return Data;
  }

  Byte ReadByte(u32 &Cycles, Byte Address, Mem &memory) 
  {
    Byte Data = memory[Address];
    Cycles--;
    return Data;
  }

  // Opcodes

  // ADC (Add with Carry)
  static constexpr Byte INS_ADC_IM = 0x69;
  static constexpr Byte INS_ADC_ZP = 0x65;
  static constexpr Byte INS_ADC_ZPX = 0x75;
  static constexpr Byte INS_ADC_ABS = 0x6D;
  static constexpr Byte INS_ADC_ABSX = 0x7D;
  static constexpr Byte INS_ADC_ABSY = 0x79;
  static constexpr Byte INS_ADC_INDX = 0x61;
  static constexpr Byte INS_ADC_INDY = 0x71;

  // AND (Bitwise AND)
  static constexpr Byte INS_AND_IM = 0x29;
  static constexpr Byte INS_AND_ZP = 0x25;
  static constexpr Byte INS_AND_ZPX = 0x35;
  static constexpr Byte INS_AND_ABS = 0x2D;
  static constexpr Byte INS_AND_ABSX = 0x3D;
  static constexpr Byte INS_AND_ABSY = 0x39;
  static constexpr Byte INS_AND_INDX = 0x21;
  static constexpr Byte INS_AND_INDY = 0x31;

  // ASL (Arithmetic Shift Left)
  static constexpr Byte INS_ASL_A = 0x0A;
  static constexpr Byte INS_ASL_ZP = 0x06;
  static constexpr Byte INS_ASL_ZPX = 0x16;
  static constexpr Byte INS_ASL_ABS = 0x0E;
  static constexpr Byte INS_ASL_ABSX = 0x1E;

  // LDA (Load Accumulator)
  static constexpr Byte INS_LDA_IM = 0xA9;
  static constexpr Byte INS_LDA_ZP = 0xA5;
  static constexpr Byte INS_LDA_ZPX = 0xB5;
  static constexpr Byte INS_LDA_ABS = 0xAD;
  static constexpr Byte INS_LDA_ABS_X = 0xBD;
  static constexpr Byte INS_LDA_ABS_Y = 0xB9;
  static constexpr Byte INS_LDA_IND_X = 0xA1;
  static constexpr Byte INS_LDA_IND_Y = 0xB1;

  // LDX
  static constexpr Byte INS_LDX_IM = 0xA2; // Load X (Immediate)
  static constexpr Byte INS_LDX_ZP = 0xA6; // Load X (Zero Page)
  static constexpr Byte INS_LDX_ZPY = 0xB6; // Load X (Zero Page, Y)
  static constexpr Byte INS_LDX_ABS = 0xAE; // Load X (Absolute)
  static constexpr Byte INS_LDX_ABSY = 0xBE; // Load X (Absolute, Y)

  // LDY
  static constexpr Byte INS_LDY_IM = 0xA0; // Load Y (Immediate)
  static constexpr Byte INS_LDY_ZP = 0xA4; // Load Y (Zero Page)
  static constexpr Byte INS_LDY_ZPX = 0xB4; // Load Y (Zero Page, X)
  static constexpr Byte INS_LDY_ABS = 0xAC; // Load Y (Absolute)
  static constexpr Byte INS_LDY_ABSX = 0xBC; // Load Y (Absolute, X)

  // LSR
  static constexpr Byte INS_LSR_A = 0x4A; // Logical Shift Right (Accumulator)
  static constexpr Byte INS_LSR_ZP = 0x46; // Logical Shift Right (Zero Page)
  static constexpr Byte INS_LSR_ZPX = 0x56; // Logical Shift Right (Zero Page, X)
  static constexpr Byte INS_LSR_ABS = 0x4E; // Logical Shift Right (Absolute)
  static constexpr Byte INS_LSR_ABSX = 0x5E; // Logical Shift Right (Absolute, X)

  // ORA
  static constexpr Byte INS_ORA_IM = 0x09; // OR with Accumulator (Immediate)
  static constexpr Byte INS_ORA_ZP = 0x05; // OR with Accumulator (Zero Page)
  static constexpr Byte INS_ORA_ZPX = 0x15; // OR with Accumulator (Zero Page, X)
  static constexpr Byte INS_ORA_ABS = 0x0D; // OR with Accumulator (Absolute)
  static constexpr Byte INS_ORA_ABSX = 0x1D; // OR with Accumulator (Absolute, X)
  static constexpr Byte INS_ORA_ABSY = 0x19; // OR with Accumulator (Absolute, Y)
  static constexpr Byte INS_ORA_IND_X = 0x01; // OR with Accumulator (Indirect, X)
  static constexpr Byte INS_ORA_IND_Y = 0x11; // OR with Accumulator (Indirect), Y

  // EOR
  static constexpr Byte INS_EOR_IM = 0x49; // Exclusive OR (Immediate)
  static constexpr Byte INS_EOR_ZP = 0x45; // Exclusive OR (Zero Page)
  static constexpr Byte INS_EOR_ZPX = 0x55; // Exclusive OR (Zero Page, X)
  static constexpr Byte INS_EOR_ABS = 0x4D; // Exclusive OR (Absolute)
  static constexpr Byte INS_EOR_ABSX = 0x5D; // Exclusive OR (Absolute, X)
  static constexpr Byte INS_EOR_ABSY = 0x59; // Exclusive OR (Absolute, Y)
  static constexpr Byte INS_EOR_IND_X = 0x41; // Exclusive OR (Indirect, X)
  static constexpr Byte INS_EOR_IND_Y = 0x51; // Exclusive OR (Indirect), Y

  // INC
  static constexpr Byte INS_INC_ZP = 0xE6; // Increment (Zero Page)
  static constexpr Byte INS_INC_ZPX = 0xF6; // Increment (Zero Page, X)
  static constexpr Byte INS_INC_ABS = 0xEE; // Increment (Absolute)
  static constexpr Byte INS_INC_ABSX = 0xFE; // Increment (Absolute, X)

  // INX
  static constexpr Byte INS_INX = 0xE8; // Increment X

  // INY
  static constexpr Byte INS_INY = 0xC8; // Increment Y
  
  // DEC
  static constexpr Byte INS_DEC_ZP = 0xC6; // Decrement (Zero Page)
  static constexpr Byte INS_DEC_ZPX = 0xD6; // Decrement (Zero Page, X)
  static constexpr Byte INS_DEC_ABS = 0xCE; // Decrement (Absolute)
  static constexpr Byte INS_DEC_ABSX = 0xDE; // Decrement (Absolute, X)

  // CMP
  static constexpr Byte INS_CMP_IM = 0xC9; // Compare (Immediate)
  static constexpr Byte INS_CMP_ZP = 0xC5; // Compare (Zero Page)
  static constexpr Byte INS_CMP_ZPX = 0xD5; // Compare (Zero Page, X)
  static constexpr Byte INS_CMP_ABS = 0xCD; // Compare (Absolute)
  static constexpr Byte INS_CMP_ABSX = 0xDD; // Compare (Absolute, X)
  static constexpr Byte INS_CMP_ABSY = 0xD9; // Compare (Absolute, Y)
  static constexpr Byte INS_CMP_IND_X = 0xC1; // Compare (Indirect, X)
  static constexpr Byte INS_CMP_IND_Y = 0xD1; // Compare (Indirect), Y

  // CPX
  static constexpr Byte INS_CPX_IM = 0xE0; // Compare X (Immediate)
  static constexpr Byte INS_CPX_ZP = 0xE4; // Compare X (Zero Page)
  static constexpr Byte INS_CPX_ABS = 0xEC; // Compare X (Absolute)

  // CPY
  static constexpr Byte INS_CPY_IM = 0xC0; // Compare Y (Immediate)
  static constexpr Byte INS_CPY_ZP = 0xC4; // Compare Y (Zero Page)
  static constexpr Byte INS_CPY_ABS = 0xCC; // Compare Y (Absolute)

  // SBC (Subtract with Carry)
  static constexpr Byte INS_SBC_IM = 0xE9;
  static constexpr Byte INS_SBC_ZP = 0xE5;
  static constexpr Byte INS_SBC_ZPX = 0xF5;
  static constexpr Byte INS_SBC_ABS = 0xED;
  static constexpr Byte INS_SBC_ABSX = 0xFD;
  static constexpr Byte INS_SBC_ABSY = 0xF9;
  static constexpr Byte INS_SBC_IND_X = 0xE1;
  static constexpr Byte INS_SBC_IND_Y = 0xF1;

  // STA (Store Accumulator)
  static constexpr Byte INS_STA_ZP = 0x85;
  static constexpr Byte INS_STA_ZPX = 0x95;
  static constexpr Byte INS_STA_ABS = 0x8D;
  static constexpr Byte INS_STA_ABSX = 0x9D;
  static constexpr Byte INS_STA_ABSY= 0x99;
  static constexpr Byte INS_STA_IND_X= 0x81;
  static constexpr Byte INS_STA_IND_Y= 0x91;

  // STX (Store X)
  static constexpr Byte INS_STX_ZP = 0x86;
  static constexpr Byte INS_STX_ZPY = 0x96;
  static constexpr Byte INS_STX_ABS = 0x8E;

  // STY (Store Y)
  static constexpr Byte INS_STY_ZP = 0x84;
  static constexpr Byte INS_STY_ZPY = 0x94;
  static constexpr Byte INS_STY_ABS = 0x8C;

  // BCC
  static constexpr Byte INS_BCC = 0x90; // Branch on Carry Clear

  // BCS
  static constexpr Byte INS_BCS = 0xB0; // Branch on Carry Set

  // BEQ
  static constexpr Byte INS_BEQ = 0xF0; // Branch on Equal (Zero Set)

  // BIT
  static constexpr Byte INS_BIT_ZP = 0x24; // Bit Test (Zero Page)
  static constexpr Byte INS_BIT_ABS = 0x2C; // Bit Test (Absolute)

  // BMI
  static constexpr Byte INS_BMI = 0x30; // Branch on Minus (Negative Set)

  // BNE
  static constexpr Byte INS_BNE = 0xD0; // Branch on Not Equal (Zero Clear)

  // BPL
  static constexpr Byte INS_BPL = 0x10; // Branch on Plus (Negative Clear)

  // BVC
  static constexpr Byte INS_BVC = 0x50; // Branch on Overflow Clear

  // BVS
  static constexpr Byte INS_BVS = 0x70; // Branch on Overflow Set

  // CLC
  static constexpr Byte INS_CLC = 0x18; // Clear Carry

  // CLD
  static constexpr Byte INS_CLD = 0xD8; // Clear Decimal

  // CLI
  static constexpr Byte INS_CLI = 0x58; // Clear Interrupt Disable

  // CLV
  static constexpr Byte INS_CLV = 0xB8; // Clear Overflow

  // DEX
  static constexpr Byte INS_DEX = 0xCA; // Decrement X

  // DEY
  static constexpr Byte INS_DEY = 0x88; // Decrement Y
  

  // PHA (Push Accumulatpr)
  static constexpr Byte INS_PHA = 0x48; 

  // PHP (Push Processor Status [SR])
  static constexpr Byte INS_PHP = 0x08; 

  // PLA (Pull Accumulator)
  static constexpr Byte INS_PLA = 0x68; 

  // PLP (Pull Processor Status [SR])
  static constexpr Byte INS_PLP = 0x28; 

  // SEC (Set Carry)
  static constexpr Byte INS_SEC = 0x38;
  
  // SED (Set Decimal)
  static constexpr Byte INS_SED = 0xF8;
    
  // SEI (Set Interrupt Disable)
  static constexpr Byte INS_SEI = 0x78;
  
  // RTI (Return from Interrupt)
  static constexpr Byte INS_RTI = 0x40;

  // RTS (Return from Subroutine)
  static constexpr Byte INS_RTS = 0x60;
  
  // TAX (Transfer Accumulator to X)
  static constexpr Byte INS_TAX = 0xAA;
  
  // TAY (Transfer Accumulator to Y)
  static constexpr Byte INS_TAY = 0xA8;

  // TSX (Transfer Stack Pointer to X)
  static constexpr Byte INS_TSX = 0xBA;
  
  // TXA (Transfer X to Accumulator)
  static constexpr Byte INS_TXA = 0x8A; 

  // TXS (Transfer X to Stack Pointer)
  static constexpr Byte INS_TXS = 0x9A; 

  // TYA (Transfer Y to accumulator)
  static constexpr Byte INS_TYA = 0x98; 
    
  // BRK (Break Condition)
  static constexpr Byte BRK = 0x00;

  // JSR (Jump to Subroutine)
  static constexpr Byte INS_JSR_ABS = 0x20;

  void LDASetStatus() 
  {
    Z = (A == 0);
    N = (A & 0b10000000) > 0;
  }

  void LDXSetStatus() 
  {
    Z = (X == 0);
    N = (X & 0b10000000) > 0;
  }

  void LDYSetStatus() 
  {
    Z = (Y == 0);
    N = (Y & 0b10000000) > 0;
  }

  void Execute(u32 Cycles, Mem &memory) 
  {
    while (Cycles > 0) 
    {
      Byte Ins = FetchByte(Cycles, memory);
      switch (Ins) 
      {
      case INS_JSR_ABS: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);

        memory[SP - 1] = (PC + 2) >> 8;
        memory[SP - 2] = (PC + 2) & 0xFF;
        SP -= 2;
        PC = AbsoluteAddr;
        Cycles -= 2;
      } break;
      case INS_LDA_IM: 
      {
        Byte Value = FetchByte(Cycles, memory);
        A = Value;
        LDASetStatus();
      } break;
      case INS_LDA_ZP: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        A = ReadByte(Cycles, ZeroPageAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_ZPX: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
        Cycles--;
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_ABS: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
        A = (SByte)ReadByte(Cycles, AbsoluteAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_ABS_X: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = ((Word)HighByte << 8) | (Word)LowByte;
        Word EffectiveAddr = AbsoluteAddr + Y;
        if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr * 0xFF00)) {
          Cycles--;
        }
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_ABS_Y: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = ((Word)HighByte << 8) | (Word)LowByte;
        Word EffectiveAddr = AbsoluteAddr + Y;
        if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
          Cycles--;
        }
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_IND_X:   
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word EffectiveAddr = ZeroPageAddr + X;
        Word HighByte = ReadByte(Cycles, EffectiveAddr + 1, memory);
        Word LowByte = ReadByte(Cycles, EffectiveAddr, memory);
        Word AbsoluteAddr = LowByte | (HighByte << 8);
        A = ReadByte(Cycles, AbsoluteAddr, memory);
        LDASetStatus();
      } break;
      case INS_LDA_IND_Y: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word LowByte = ReadByte(Cycles, ZeroPageAddr, memory);
        Word HighByte = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
        Word AbsoluteAddr = LowByte | (HighByte << 8);
        Word EffectiveAddr = AbsoluteAddr + Y;
        if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
          Cycles--;
        }
        A = ReadByte(Cycles, AbsoluteAddr, memory);
        LDASetStatus();
      } break;
      case INS_AND_IM: 
      {
          Byte Value = FetchByte(Cycles, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_ZP: 
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte Value = ReadByte(Cycles, Addr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_ZPX: 
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte EffectiveAddr = (Addr + X) & 0xFF;
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_ABSX: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + X;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00))  {
            Cycles--;
          }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_ABSY: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
            Cycles--;
          }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_INDX: 
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
          Byte LowByte = ReadByte(Cycles, EffectiveAddr, memory);
          Byte HighByte = ReadByte(Cycles, (EffectiveAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_AND_INDY: 
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Byte LowByte = ReadByte(Cycles, ZeroPageAddr, memory);
          Byte HighByte = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
            Cycles--;
          }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case INS_STA_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          memory[AbsoluteAddr] = A;
          Cycles--;
      } break;
      case INS_ADC_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);

          Word Result = (Word)A + (Word)Value + (C ? 1 : 0);
          A = Result & 0xFF;
          C = (Result > 0xFF);
          LDASetStatus();
      } break;
      case INS_SBC_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word) HighByte << 8);
          SByte Value = ReadByte(Cycles, AbsoluteAddr, memory);

          Word Result = (Word)A - (Word)(Value)- (C ? 1 : 0);
          A = Result & 0xFF;
          Z = (A == 0);
          C = (Result < 0x100);
          N = (A & 0b10000000) > 0;
          LDASetStatus();
      } break;
      case BRK: 
      {
          return;    
      }
      default: 
      {
        printf("Instruction not handled %d\n", Ins);
        break;
      }
      }
    }
  }
};