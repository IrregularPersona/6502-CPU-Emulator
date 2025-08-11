### Intro
This is a learning project: a simple 6502 CPU emulator. It's a work-in-progress (about 27/137 instructions implemented). Inspired by Dave Poo's series.

### Build
Requires CMake and a C++17 compiler.

```
mkdir -p build
cd build
cmake ..
cmake --build .
```

The binary will be placed in `bin/main`.

### Run
```
./bin/main
```

### Sources
Thanks to:
- [Dave Poo's Video](www.youtube.com/watch?v=qJgsuQoy9bc)
- [6502 Instruction Set](https://www.masswerk.at/6502/6502_instruction_set.html)

### Instruction Coverage
- LDA (Load Accumulator)
  - [x] IM
  - [x] ZP
  - [x] ZPX
  - [x] ABS
  - [x] ABS,X
  - [x] ABS,Y
  - [x] (IND,X)
  - [x] (IND),Y
- LDX (Load X)
  - [x] IM
  - [x] ZP
  - [x] ZP,Y
  - [x] ABS
  - [x] ABS,Y
- LDY (Load Y)
  - [x] IM
  - [x] ZP
  - [x] ZP,X
  - [x] ABS
  - [x] ABS,X
- STA (Store A)
  - [x] ZP
  - [x] ZP,X
  - [x] ABS
  - [x] ABS,X
  - [x] ABS,Y
  - [x] (IND,X)
  - [x] (IND),Y
- STX (Store X)
  - [ ] ZP
  - [ ] ZP,Y
  - [ ] ABS
- STY (Store Y)
  - [ ] ZP
  - [ ] ZP,X
  - [ ] ABS

- AND (A &= M)
  - [x] IM
  - [x] ZP
  - [x] ZP,X
  - [x] ABS
  - [x] ABS,X
  - [x] ABS,Y
  - [x] (IND,X)
  - [x] (IND),Y
- ORA (A |= M)
  - [x] IM
  - [x] ZP
  - [x] ZP,X
  - [x] ABS
  - [x] ABS,X
  - [x] ABS,Y
  - [x] (IND,X)
  - [x] (IND),Y
- EOR (A ^= M)
  - [x] IM
  - [x] ZP
  - [x] ZP,X
  - [x] ABS
  - [x] ABS,X
  - [x] ABS,Y
  - [x] (IND,X)
  - [x] (IND),Y

- ADC (Add with Carry)
  - [ ] IM
  - [ ] ZP
  - [ ] ZP,X
  - [x] ABS
  - [ ] ABS,X
  - [ ] ABS,Y
  - [ ] (IND,X)
  - [ ] (IND),Y
- SBC (Subtract with Carry)
  - [ ] IM
  - [ ] ZP
  - [ ] ZP,X
  - [x] ABS
  - [ ] ABS,X
  - [ ] ABS,Y
  - [ ] (IND,X)
  - [ ] (IND),Y

- INC/DEC (Memory)
  - [x] INC ZP
  - [x] INC ZP,X
  - [x] INC ABS
  - [x] INC ABS,X
  - [x] DEC ZP
  - [x] DEC ZP,X
  - [x] DEC ABS
  - [x] DEC ABS,X
- INX/INY/DEX/DEY (Registers)
  - [x] INX
  - [x] INY
  - [x] DEX
  - [x] DEY

- CMP/CPX/CPY
  - [x] CMP IM, ZP, ZP,X, ABS, ABS,X, ABS,Y, (IND,X), (IND),Y
  - [x] CPX IM, ZP, ABS
  - [x] CPY IM, ZP, ABS

- Shifts/Rotates
  - [x] LSR A
  - [x] LSR ZP
  - [x] LSR ZP,X
  - [x] LSR ABS
  - [x] LSR ABS,X
  - [ ] ASL (all)
  - [ ] ROL (all)
  - [ ] ROR (all)

- Branches
  - [x] BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS

- Flags and Stack
  - [x] CLC, CLD, CLI, CLV, SEC, SED, SEI
  - [x] PHA, PHP, PLA, PLP

- Subroutines/Interrupts
  - [x] JSR ABS
  - [x] RTS
  - [x] RTI
  - [x] BRK
  - [ ] BIT (ZP, ABS)
