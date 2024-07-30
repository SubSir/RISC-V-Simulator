#include "memory.hpp"
#include "opcode.hpp"
#include "tools.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

void Memory::initialize() {
  int pointer = 0;
  while (!std::cin.eof()) {
    std::string line;
    std::cin >> line;
    if (line[0] == '@') {
      pointer = std::stoi(line.substr(1, line.length() - 1), nullptr, 16);
    } else {
      std::stringstream ss;
      ss << std::hex << line;
      unsigned int bit;
      ss >> bit;
      mem[pointer++] <= bit;
    }
  }
}

void Memory::work() {
  int pc_now = to_unsigned(pc);
  if (from_rob) {
    int pc_predict = to_unsigned(from_rob_predict);
    if (from_rob_jump != to_unsigned(jump[pc_predict])) {
      pc_now = to_unsigned(from_rob_pc);
    }
    if (from_rob_jump) {
      predict[pc_predict] <= std::min(to_signed(predict[pc_predict]) + 1, 4);
    } else {
      predict[pc_predict] <= std::max(to_signed(predict[pc_predict]) - 1, 0);
    }
  }
  pc_past <= pc_now;
  if (from_rs) {
    if (pc_now < mem.size()) {
      // std::cout << "        Memread : " << to_unsigned(pc) << std::endl;
      rs_get_out <= 1;
      Bit<32> ins = read_a_word(pc_now);
      int funct7 = to_unsigned(ins.range<31, 25>());
      int funct3 = to_unsigned(ins.range<14, 12>());
      int opcode = to_unsigned(ins.range<6, 0>());
      int rds = to_unsigned(ins.range<11, 7>());
      int rs1 = to_unsigned(ins.range<19, 15>());
      int rs2 = to_unsigned(ins.range<24, 20>());
      bool use1 = 1, use2 = 1, userd = 1;
      if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0000000) {
        // ADD
        to_rs_op <= ADD;
        pc <= pc_now + 4;
      } else if (opcode == 0b0110011 && funct3 == 0b000 &&
                 funct7 == 0b0100000) {
        // SUB
        to_rs_op <= SUB;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b111 &&
                 funct7 == 0b0000000) {
        // AND
        to_rs_op <= AND;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b110 &&
                 funct7 == 0b0000000) {
        // OR
        to_rs_op <= OR;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b100 &&
                 funct7 == 0b0000000) {
        // XOR
        to_rs_op <= XOR;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b001 &&
                 funct7 == 0b0000000) {
        // SLL
        to_rs_op <= SLL;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                 funct7 == 0b0000000) {
        // SRL
        to_rs_op <= SRL;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                 funct7 == 0b0100000) {
        // SRA
        to_rs_op <= SRA;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b010 &&
                 funct7 == 0b0000000) {
        // SLT
        to_rs_op <= SLT;
        pc <= pc_now + 4;

      } else if (opcode == 0b0110011 && funct3 == 0b011 &&
                 funct7 == 0b0000000) {
        // SLTU
        to_rs_op <= SLTU;
        pc <= pc_now + 4;

      } else if (opcode == 0b0010011 && funct3 == 0b000) {
        // ADDI
        to_rs_op <= ADDI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b111) {
        // ANDI
        to_rs_op <= ANDI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b110) {
        // ORI
        to_rs_op <= ORI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b100) {
        // XORI
        to_rs_op <= XORI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b001 &&
                 funct7 == 0b0000000) {
        // SLLI
        to_rs_op <= SLLI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_unsigned(ins.range<24, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                 funct7 == 0b0000000) {
        // SRLI
        to_rs_op <= SRLI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_unsigned(ins.range<24, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                 funct7 == 0b0100000) {
        // SRAI
        to_rs_op <= SRAI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_unsigned(ins.range<24, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b010) {
        // SLTI
        to_rs_op <= SLTI;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0010011 && funct3 == 0b011) {
        // SLTIU
        to_rs_op <= SLTIU;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_unsigned(ins.range<31, 20>());
      } else if (opcode == 0b0000011 && funct3 == 0b000) {
        // LB
        to_rs_op <= LB;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0000011 && funct3 == 0b100) {
        // LBU
        to_rs_op <= LBU;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0000011 && funct3 == 0b001) {
        // LH
        to_rs_op <= LH;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0000011 && funct3 == 0b101) {
        // LHU
        to_rs_op <= LHU;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b0000011 && funct3 == 0b010) {
        // LW
        to_rs_op <= LW;
        use2 = 0;
        pc <= pc_now + 4;

        to_rs_a <= to_signed(ins.range<31, 20>());
        // std::cout << "lw,rd = " << rds
        //           << " ,rs1 = " << rs1 << " ,i = " << i
        //           << std::endl;
      } else if (opcode == 0b0100011 && funct3 == 0b000) {
        // SB
        to_rs_op <= SB;
        userd = 0;
        pc <= pc_now + 4;

        Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b0100011 && funct3 == 0b001) {
        // SH
        to_rs_op <= SH;
        userd = 0;
        pc <= pc_now + 4;

        Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b0100011 && funct3 == 0b010) {
        // SW
        to_rs_op <= SW;
        userd = 0;
        pc <= pc_now + 4;

        Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b000) {
        // BEQ
        to_rs_op <= BEQ;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};
        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b101) {
        // BGE
        to_rs_op <= BGE;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};

        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b111) {
        // BGEU
        to_rs_op <= BGEU;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};
        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b100) {
        // BLT
        to_rs_op <= BLT;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};
        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b110) {
        // BLTU
        to_rs_op <= BLTU;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};
        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100011 && funct3 == 0b001) {
        // BNE
        to_rs_op <= BNE;
        userd = 0;
        Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                   Bit<1>()};
        if (predict[pc_now] > 1) {
          pc <= pc + to_signed(imm);
          to_rs_jump <= 1;
          jump[pc_now] <= 1;
        } else {
          pc <= pc_now + 4;
          to_rs_jump <= 0;
          jump[pc_now] <= 0;
        }
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b1100111 && funct3 == 0b000) {
        // JALR
        to_rs_op <= JALR;
        use2 = 0;
        pc <= pc_now + 4;
        jump[pc_now] <= 0;
        to_rs_a <= to_signed(ins.range<31, 20>());
      } else if (opcode == 0b1101111) {
        // JAL
        to_rs_op <= JAL;
        use1 = 0;
        use2 = 0;

        Bit imm = {ins[31], ins.range<19, 12>(), ins[20], ins.range<30, 21>(),
                   Bit<1>()};
        pc <= pc_now + to_signed(imm);
        to_rs_a <= to_signed(imm);
      } else if (opcode == 0b0010111) {
        // AUIPC
        to_rs_op <= AUIPC;
        use1 = 0;
        use2 = 0;
        pc <= pc_now + 4;

        Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
        to_rs_a <= to_signed(c);
      } else if (opcode == 0b0110111) {
        // LUI
        to_rs_op <= LUI;
        use1 = 0;
        use2 = 0;
        pc <= pc_now + 4;

        Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
        to_rs_a <= to_signed(c);
      } else {
        to_rs_op <= ELSE;
        use1 = 0;
        use2 = 0;
        userd = 0;
        pc <= pc_now + 4;
      }
      if (use1) {
        to_rs_rs1 <= rs1;
      } else {
        to_rs_rs1 <= 32;
      }
      if (use2) {
        to_rs_rs2 <= rs2;
      } else {
        to_rs_rs2 <= 32;
      }
      if (userd) {
        to_rs_rd <= rds;
      } else {
        to_rs_rd <= 32;
      }
      return;
    }
  }
  rs_get_out <= 0;
  pc <= pc_now;
}

Bit<8> Memory::read_byte(int address) {
  address %= 0x10000;
  if (address >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  return mem[address];
}

Bit<16> Memory::read_half_word(int address) {
  address %= 0x10000;
  if (address + 1 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  Bit<8> a = mem[address + 1];
  Bit<8> b = mem[address];
  Bit c = {a, b};
  return c;
}

Bit<32> Memory::read_a_word(int address) {
  address %= 0x10000;
  if (address + 3 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  Bit<8> a = mem[address + 3];
  Bit<8> b = mem[address + 2];
  Bit<8> c = mem[address + 1];
  Bit<8> d = mem[address];
  Bit e = {a, b, c, d};
  return e;
}

void Memory::store_byte(int address, Bit<8> value) {
  address %= 0x10000;
  if (address >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value;
}

void Memory::store_half_word(int address, Bit<16> value) {
  address %= 0x10000;
  if (address + 1 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value.range<7, 0>();
  mem[address + 1] <= value.range<15, 8>();
}

void Memory::store_a_word(int address, Bit<32> value) {
  address %= 0x10000;
  if (address + 3 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value.range<7, 0>();
  mem[address + 1] <= value.range<15, 8>();
  mem[address + 2] <= value.range<23, 16>();
  mem[address + 3] <= value.range<31, 24>();
}