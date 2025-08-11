#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdint>

using Byte = std::uint8_t;
using SByte = std::int8_t;
using Word = std::uint16_t;
using u32 = std::uint32_t;

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

  Byte ReadByte(u32 &Cycles, Word Address, Mem &memory) 
  {
    Byte Data = memory[Address];
    Cycles--;
    return Data;
  }

  enum class Opcode : Byte {
    // BRK/JSR
    BRK = 0x00,
    JSR_ABS = 0x20,

    // ADC (Add with Carry)
    ADC_IM = 0x69,
    ADC_ZP = 0x65,
    ADC_ZPX = 0x75,
    ADC_ABS = 0x6D,
    ADC_ABSX = 0x7D,
    ADC_ABSY = 0x79,
    ADC_INDX = 0x61,
    ADC_INDY = 0x71,

    // AND
    AND_IM = 0x29,
    AND_ZP = 0x25,
    AND_ZPX = 0x35,
    AND_ABS = 0x2D,
    AND_ABSX = 0x3D,
    AND_ABSY = 0x39,
    AND_INDX = 0x21,
    AND_INDY = 0x31,

    // ASL
    ASL_A = 0x0A,
    ASL_ZP = 0x06,
    ASL_ZPX = 0x16,
    ASL_ABS = 0x0E,
    ASL_ABSX = 0x1E,

    // LDA
    LDA_IM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_ABS = 0xAD,
    LDA_ABS_X = 0xBD,
    LDA_ABS_Y = 0xB9,
    LDA_IND_X = 0xA1,
    LDA_IND_Y = 0xB1,

    // LDX
    LDX_IM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZPY = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABSY = 0xBE,

    // LDY
    LDY_IM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZPX = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABSX = 0xBC,

    // LSR
    LSR_A = 0x4A,
    LSR_ZP = 0x46,
    LSR_ZPX = 0x56,
    LSR_ABS = 0x4E,
    LSR_ABSX = 0x5E,

    // ORA
    ORA_IM = 0x09,
    ORA_ZP = 0x05,
    ORA_ZPX = 0x15,
    ORA_ABS = 0x0D,
    ORA_ABSX = 0x1D,
    ORA_ABSY = 0x19,
    ORA_IND_X = 0x01,
    ORA_IND_Y = 0x11,

    // EOR
    EOR_IM = 0x49,
    EOR_ZP = 0x45,
    EOR_ZPX = 0x55,
    EOR_ABS = 0x4D,
    EOR_ABSX = 0x5D,
    EOR_ABSY = 0x59,
    EOR_IND_X = 0x41,
    EOR_IND_Y = 0x51,

    // INC
    INC_ZP = 0xE6,
    INC_ZPX = 0xF6,
    INC_ABS = 0xEE,
    INC_ABSX = 0xFE,

    // INX/INY
    INX = 0xE8,
    INY = 0xC8,

    // DEC
    DEC_ZP = 0xC6,
    DEC_ZPX = 0xD6,
    DEC_ABS = 0xCE,
    DEC_ABSX = 0xDE,

    // CMP
    CMP_IM = 0xC9,
    CMP_ZP = 0xC5,
    CMP_ZPX = 0xD5,
    CMP_ABS = 0xCD,
    CMP_ABSX = 0xDD,
    CMP_ABSY = 0xD9,
    CMP_IND_X = 0xC1,
    CMP_IND_Y = 0xD1,

    // CPX
    CPX_IM = 0xE0,
    CPX_ZP = 0xE4,
    CPX_ABS = 0xEC,

    // CPY
    CPY_IM = 0xC0,
    CPY_ZP = 0xC4,
    CPY_ABS = 0xCC,

    // SBC
    SBC_IM = 0xE9,
    SBC_ZP = 0xE5,
    SBC_ZPX = 0xF5,
    SBC_ABS = 0xED,
    SBC_ABSX = 0xFD,
    SBC_ABSY = 0xF9,
    SBC_IND_X = 0xE1,
    SBC_IND_Y = 0xF1,

    // Stores
    STA_ZP = 0x85,
    STA_ZPX = 0x95,
    STA_ABS = 0x8D,
    STA_ABSX = 0x9D,
    STA_ABSY = 0x99,
    STA_IND_X = 0x81,
    STA_IND_Y = 0x91,

    STX_ZP = 0x86,
    STX_ZPY = 0x96,
    STX_ABS = 0x8E,

    STY_ZP = 0x84,
    STY_ZPY = 0x94,
    STY_ABS = 0x8C,

    // Branches
    BCC = 0x90,
    BCS = 0xB0,
    BEQ = 0xF0,
    BMI = 0x30,
    BNE = 0xD0,
    BPL = 0x10,
    BVC = 0x50,
    BVS = 0x70,

    // Flags and stack ops
    CLC = 0x18,
    CLD = 0xD8,
    CLI = 0x58,
    CLV = 0xB8,
    DEX = 0xCA,
    DEY = 0x88,
    PHA = 0x48,
    PHP = 0x08,
    PLA = 0x68,
    PLP = 0x28,
    SEC = 0x38,
    SED = 0xF8,
    SEI = 0x78,
    RTI = 0x40,
    RTS = 0x60,
    TAX = 0xAA,
    TAY = 0xA8,
    TSX = 0xBA,
    TXA = 0x8A,
    TXS = 0x9A,
    TYA = 0x98,
  };

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
    auto setZN = [&](Byte value) {
      Z = (value == 0);
      N = (value & 0x80) != 0;
    };

    auto pushByte = [&](Byte v) {
      memory[SP] = v;
      SP--;
      Cycles--;
    };

    auto popByte = [&]() -> Byte {
      SP++;
      Byte v = memory[SP];
      Cycles--;
      return v;
    };

    auto getStatus = [&]() -> Byte {
      Byte p = 0;
      p |= (N ? 1 : 0) << 7;
      p |= (V ? 1 : 0) << 6;
      p |= 1 << 5;        // unused bit set
      p |= (B ? 1 : 0) << 4;
      p |= (D ? 1 : 0) << 3;
      p |= (I ? 1 : 0) << 2;
      p |= (Z ? 1 : 0) << 1;
      p |= (C ? 1 : 0) << 0;
      return p;
    };

    auto setStatus = [&](Byte p) {
      C = (p & 0x01) != 0;
      Z = (p & 0x02) != 0;
      I = (p & 0x04) != 0;
      D = (p & 0x08) != 0;
      B = (p & 0x10) != 0;
      V = (p & 0x40) != 0;
      N = (p & 0x80) != 0;
    };

    while (Cycles > 0) 
    {
      Byte Ins = FetchByte(Cycles, memory);
      Opcode Op = static_cast<Opcode>(Ins);
      switch (Op) 
      {
      case Opcode::JSR_ABS: 
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
      case Opcode::LDA_IM: 
      {
        Byte Value = FetchByte(Cycles, memory);
        A = Value;
        LDASetStatus();
      } break;
      case Opcode::LDA_ZP: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        A = ReadByte(Cycles, ZeroPageAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::LDA_ZPX: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
        Cycles--;
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::LDA_ABS: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
        A = (SByte)ReadByte(Cycles, AbsoluteAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::LDA_ABS_X: 
      {
        Byte LowByte = FetchByte(Cycles, memory);
        Byte HighByte = FetchByte(Cycles, memory);
        Word AbsoluteAddr = ((Word)HighByte << 8) | (Word)LowByte;
        Word EffectiveAddr = AbsoluteAddr + X;
        if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
          Cycles--;
        }
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::LDA_ABS_Y: 
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
      case Opcode::LDA_IND_X:   
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word EffectiveAddr = ZeroPageAddr + X;
        Word HighByte = ReadByte(Cycles, EffectiveAddr + 1, memory);
        Word LowByte = ReadByte(Cycles, EffectiveAddr, memory);
        Word AbsoluteAddr = LowByte | (HighByte << 8);
        A = ReadByte(Cycles, AbsoluteAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::LDA_IND_Y: 
      {
        Byte ZeroPageAddr = FetchByte(Cycles, memory);
        Word LowByte = ReadByte(Cycles, ZeroPageAddr, memory);
        Word HighByte = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
        Word AbsoluteAddr = LowByte | (HighByte << 8);
        Word EffectiveAddr = AbsoluteAddr + Y;
        if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) {
          Cycles--;
        }
        A = ReadByte(Cycles, EffectiveAddr, memory);
        LDASetStatus();
      } break;
      case Opcode::AND_IM: 
      {
          Byte Value = FetchByte(Cycles, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case Opcode::AND_ZP: 
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte Value = ReadByte(Cycles, Addr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case Opcode::AND_ZPX: 
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte EffectiveAddr = (Addr + X) & 0xFF;
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case Opcode::AND_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A &= Value;
          Z = (A == 0);
          N = (A & 0b10000000) > 0;
      } break;
      case Opcode::AND_ABSX: 
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
      case Opcode::AND_ABSY: 
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
      case Opcode::AND_INDX: 
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
      case Opcode::AND_INDY: 
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
      // ORA
      case Opcode::ORA_IM:
      {
          Byte Value = FetchByte(Cycles, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_ZP:
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte Value = ReadByte(Cycles, Addr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_ZPX:
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte EffectiveAddr = (Addr + X) & 0xFF;
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_ABS:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_ABSX:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + X;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_ABSY:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_IND_X:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
          Byte Low = ReadByte(Cycles, EffectiveAddr, memory);
          Byte High = ReadByte(Cycles, (EffectiveAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A |= Value;
          setZN(A);
      } break;
      case Opcode::ORA_IND_Y:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Byte Low = ReadByte(Cycles, ZeroPageAddr, memory);
          Byte High = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A |= Value;
          setZN(A);
      } break;

      // EOR
      case Opcode::EOR_IM:
      {
          Byte Value = FetchByte(Cycles, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_ZP:
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte Value = ReadByte(Cycles, Addr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_ZPX:
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte EffectiveAddr = (Addr + X) & 0xFF;
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_ABS:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_ABSX:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + X;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_ABSY:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_IND_X:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
          Byte Low = ReadByte(Cycles, EffectiveAddr, memory);
          Byte High = ReadByte(Cycles, (EffectiveAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          Byte Value = ReadByte(Cycles, AbsoluteAddr, memory);
          A ^= Value;
          setZN(A);
      } break;
      case Opcode::EOR_IND_Y:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Byte Low = ReadByte(Cycles, ZeroPageAddr, memory);
          Byte High = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          if ((EffectiveAddr & 0xFF00) != (AbsoluteAddr & 0xFF00)) { Cycles--; }
          Byte Value = ReadByte(Cycles, EffectiveAddr, memory);
          A ^= Value;
          setZN(A);
      } break;

      // STA variants
      case Opcode::STA_ZP:
      {
          Byte Addr = FetchByte(Cycles, memory);
          memory[Addr] = A;
          Cycles--;
      } break;
      case Opcode::STA_ZPX:
      {
          Byte Addr = FetchByte(Cycles, memory);
          Byte EffectiveAddr = (Addr + X) & 0xFF;
          memory[EffectiveAddr] = A;
          Cycles--;
      } break;
      case Opcode::STA_ABSX:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + X;
          memory[EffectiveAddr] = A;
          Cycles--;
      } break;
      case Opcode::STA_ABSY:
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          memory[EffectiveAddr] = A;
          Cycles--;
      } break;
      case Opcode::STA_IND_X:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Word EffectiveAddr = (ZeroPageAddr + X) & 0xFF;
          Byte Low = ReadByte(Cycles, EffectiveAddr, memory);
          Byte High = ReadByte(Cycles, (EffectiveAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          memory[AbsoluteAddr] = A;
          Cycles--;
      } break;
      case Opcode::STA_IND_Y:
      {
          Byte ZeroPageAddr = FetchByte(Cycles, memory);
          Byte Low = ReadByte(Cycles, ZeroPageAddr, memory);
          Byte High = ReadByte(Cycles, (ZeroPageAddr + 1) & 0xFF, memory);
          Word AbsoluteAddr = (Word)Low | ((Word)High << 8);
          Word EffectiveAddr = AbsoluteAddr + Y;
          memory[EffectiveAddr] = A;
          Cycles--;
      } break;

      // LDX
      case Opcode::LDX_IM:
      {
          X = FetchByte(Cycles, memory);
          LDXSetStatus();
      } break;
      case Opcode::LDX_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          X = ReadByte(Cycles, addr, memory);
          LDXSetStatus();
      } break;
      case Opcode::LDX_ZPY:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + Y) & 0xFF;
          Cycles--; // indexing cost
          X = ReadByte(Cycles, eff, memory);
          LDXSetStatus();
      } break;
      case Opcode::LDX_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          X = ReadByte(Cycles, abs, memory);
          LDXSetStatus();
      } break;
      case Opcode::LDX_ABSY:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + Y;
          if ((eff & 0xFF00) != (abs & 0xFF00)) { Cycles--; }
          X = ReadByte(Cycles, eff, memory);
          LDXSetStatus();
      } break;

      // LDY
      case Opcode::LDY_IM:
      {
          Y = FetchByte(Cycles, memory);
          LDYSetStatus();
      } break;
      case Opcode::LDY_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Y = ReadByte(Cycles, addr, memory);
          LDYSetStatus();
      } break;
      case Opcode::LDY_ZPX:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + X) & 0xFF;
          Cycles--;
          Y = ReadByte(Cycles, eff, memory);
          LDYSetStatus();
      } break;
      case Opcode::LDY_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Y = ReadByte(Cycles, abs, memory);
          LDYSetStatus();
      } break;
      case Opcode::LDY_ABSX:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + X;
          if ((eff & 0xFF00) != (abs & 0xFF00)) { Cycles--; }
          Y = ReadByte(Cycles, eff, memory);
          LDYSetStatus();
      } break;

      // INC
      case Opcode::INC_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          v = (Byte)(v + 1);
          memory[addr] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::INC_ZPX:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + X) & 0xFF;
          Byte v = ReadByte(Cycles, eff, memory);
          v = (Byte)(v + 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::INC_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          v = (Byte)(v + 1);
          memory[abs] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::INC_ABSX:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + X;
          Byte v = ReadByte(Cycles, eff, memory);
          v = (Byte)(v + 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;

      // DEC
      case Opcode::DEC_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          v = (Byte)(v - 1);
          memory[addr] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::DEC_ZPX:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + X) & 0xFF;
          Byte v = ReadByte(Cycles, eff, memory);
          v = (Byte)(v - 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::DEC_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          v = (Byte)(v - 1);
          memory[abs] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::DEC_ABSX:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + X;
          Byte v = ReadByte(Cycles, eff, memory);
          v = (Byte)(v - 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;

      // INX/INY
      case Opcode::INX:
      {
          X = (Byte)(X + 1);
          setZN(X);
      } break;
      case Opcode::INY:
      {
          Y = (Byte)(Y + 1);
          setZN(Y);
      } break;

      // DEX/DEY
      case Opcode::DEX:
      {
          X = (Byte)(X - 1);
          setZN(X);
      } break;
      case Opcode::DEY:
      {
          Y = (Byte)(Y - 1);
          setZN(Y);
      } break;

      // CMP family
      case Opcode::CMP_IM:
      {
          Byte v = FetchByte(Cycles, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_ZPX:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + X) & 0xFF;
          Byte v = ReadByte(Cycles, eff, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_ABSX:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + X;
          Byte v = ReadByte(Cycles, eff, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_ABSY:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + Y;
          Byte v = ReadByte(Cycles, eff, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_IND_X:
      {
          Byte zp = FetchByte(Cycles, memory);
          Byte eff = (zp + X) & 0xFF;
          Byte low = ReadByte(Cycles, eff, memory);
          Byte high = ReadByte(Cycles, (eff + 1) & 0xFF, memory);
          Word abs = (Word)low | ((Word)high << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CMP_IND_Y:
      {
          Byte zp = FetchByte(Cycles, memory);
          Byte low = ReadByte(Cycles, zp, memory);
          Byte high = ReadByte(Cycles, (zp + 1) & 0xFF, memory);
          Word abs = (Word)low | ((Word)high << 8);
          Word eff = abs + Y;
          Byte v = ReadByte(Cycles, eff, memory);
          Word r = (Word)A - (Word)v;
          Z = ((Byte)r == 0);
          C = A >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;

      // CPX
      case Opcode::CPX_IM:
      {
          Byte v = FetchByte(Cycles, memory);
          Word r = (Word)X - (Word)v;
          Z = ((Byte)r == 0);
          C = X >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CPX_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          Word r = (Word)X - (Word)v;
          Z = ((Byte)r == 0);
          C = X >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CPX_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          Word r = (Word)X - (Word)v;
          Z = ((Byte)r == 0);
          C = X >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;

      // CPY
      case Opcode::CPY_IM:
      {
          Byte v = FetchByte(Cycles, memory);
          Word r = (Word)Y - (Word)v;
          Z = ((Byte)r == 0);
          C = Y >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CPY_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          Word r = (Word)Y - (Word)v;
          Z = ((Byte)r == 0);
          C = Y >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;
      case Opcode::CPY_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          Word r = (Word)Y - (Word)v;
          Z = ((Byte)r == 0);
          C = Y >= v;
          N = ((Byte)r & 0x80) != 0;
      } break;

      // Flag instructions
      case Opcode::CLC: { C = 0; } break;
      case Opcode::CLD: { D = 0; } break;
      case Opcode::CLI: { I = 0; } break;
      case Opcode::CLV: { V = 0; } break;
      case Opcode::SEC: { C = 1; } break;
      case Opcode::SED: { D = 1; } break;
      case Opcode::SEI: { I = 1; } break;

      // Stack ops
      case Opcode::PHA: { pushByte(A); } break;
      case Opcode::PHP: { pushByte(getStatus()); } break;
      case Opcode::PLA: { A = popByte(); setZN(A); } break;
      case Opcode::PLP: { setStatus(popByte()); } break;

      // Transfers
      case Opcode::TAX: { X = A; setZN(X); } break;
      case Opcode::TAY: { Y = A; setZN(Y); } break;
      case Opcode::TXA: { A = X; setZN(A); } break;
      case Opcode::TYA: { A = Y; setZN(A); } break;
      case Opcode::TSX: { X = (Byte)(SP & 0xFF); setZN(X); } break;
      case Opcode::TXS: { SP = (Word)((SP & 0xFF00) | X); } break;

      // Shifts (only LSR implemented here)
      case Opcode::LSR_A:
      {
          C = (A & 0x01) != 0;
          A = (Byte)(A >> 1);
          setZN(A);
      } break;
      case Opcode::LSR_ZP:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte v = ReadByte(Cycles, addr, memory);
          C = (v & 0x01) != 0;
          v = (Byte)(v >> 1);
          memory[addr] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::LSR_ZPX:
      {
          Byte addr = FetchByte(Cycles, memory);
          Byte eff = (addr + X) & 0xFF;
          Byte v = ReadByte(Cycles, eff, memory);
          C = (v & 0x01) != 0;
          v = (Byte)(v >> 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::LSR_ABS:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Byte v = ReadByte(Cycles, abs, memory);
          C = (v & 0x01) != 0;
          v = (Byte)(v >> 1);
          memory[abs] = v; Cycles--;
          setZN(v);
      } break;
      case Opcode::LSR_ABSX:
      {
          Byte lo = FetchByte(Cycles, memory);
          Byte hi = FetchByte(Cycles, memory);
          Word abs = (Word)lo | ((Word)hi << 8);
          Word eff = abs + X;
          Byte v = ReadByte(Cycles, eff, memory);
          C = (v & 0x01) != 0;
          v = (Byte)(v >> 1);
          memory[eff] = v; Cycles--;
          setZN(v);
      } break;

      // Branches (relative addressing)
      case Opcode::BEQ:
      case Opcode::BNE:
      case Opcode::BCC:
      case Opcode::BCS:
      case Opcode::BPL:
      case Opcode::BMI:
      case Opcode::BVC:
      case Opcode::BVS:
      {
          Byte offset = FetchByte(Cycles, memory);
          bool cond = false;
          switch (Op) {
            case Opcode::BEQ: cond = Z; break;
            case Opcode::BNE: cond = !Z; break;
            case Opcode::BCC: cond = !C; break;
            case Opcode::BCS: cond = C; break;
            case Opcode::BPL: cond = !N; break;
            case Opcode::BMI: cond = N; break;
            case Opcode::BVC: cond = !V; break;
            case Opcode::BVS: cond = V; break;
            default: break;
          }
          if (cond) {
            Word oldPC = PC;
            PC = (Word)(PC + (std::int8_t)offset);
            Cycles--; // branch taken cost
            if ((oldPC & 0xFF00) != (PC & 0xFF00)) { Cycles--; }
          }
      } break;

      // Return from subroutine/interrupt
      case Opcode::RTS:
      {
          Byte low = popByte();
          Byte high = popByte();
          Word addr = (Word)low | ((Word)high << 8);
          PC = addr;
          Cycles -= 1; // rough extra cycle
      } break;
      case Opcode::RTI:
      {
          Byte p = popByte();
          setStatus(p);
          Byte low = popByte();
          Byte high = popByte();
          PC = (Word)low | ((Word)high << 8);
      } break;
      case Opcode::STA_ABS: 
      {
          Byte LowByte = FetchByte(Cycles, memory);
          Byte HighByte = FetchByte(Cycles, memory);
          Word AbsoluteAddr = (Word)LowByte | ((Word)HighByte << 8);
          memory[AbsoluteAddr] = A;
          Cycles--;
      } break;
      case Opcode::ADC_ABS: 
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
      case Opcode::SBC_ABS: 
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
      case Opcode::BRK: 
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
