#include "../include/tools.h"
#include "memory.hpp"
#include "rob.hpp"
#include <fstream>
#include <iostream>
#include <ostream>

using namespace std;
int main() {
  // dark::CPU cpu;
  // RS rs;
  RoB rob;
  // cpu.add_module(&rs);
  // cpu.add_module(&rob);
  rob.memory.initialize();
  // rob.memory.from_rs = [&]() -> auto & { return rs.to_memory; };
  // rs.from_memory_wire = [&]() -> auto & { return rob.memory.to_rs_wire; };
  // rs.pc_wire = [&]() -> auto & { return rob.memory.pc_past; };
  // rs.from_rob = [&]() -> auto & { return rob.to_rs; };
  // rob.rob_get_in = [&]() -> auto & { return rs.rob_get_out; };
  // rs.rob_error = [&]() -> auto & { return rob.rob_error; };
  // rs.from_rob_wire_i = [&]() -> auto & { return rob.to_rs_wire_i; };
  // rs.from_rob_wire_value = [&]() -> auto & { return rob.to_rs_wire_value; };
  // rs.rs_get_in = [&]() -> auto & { return rob.memory.rs_get_out; };
  // rs.rob_rs_get_in = [&]() -> auto & { return rob.rs_get_out; };
  // rob.from_rs_wire_a = [&]() -> auto & { return rs.to_rob_wire_a; };
  // rob.from_rs_wire_dest = [&]() -> auto & { return rs.to_rob_wire_dest; };
  // rob.from_rs_wire_i = [&]() -> auto & { return rs.to_rob_wire_i; };
  // rob.from_rs_wire_op = [&]() -> auto & { return rs.to_rob_wire_op; };
  // rob.from_rs_wire_pc = [&]() -> auto & { return rs.to_rob_wire_pc; };
  // rob.from_rs_wire_rs1 = [&]() -> auto & { return rs.to_rob_wire_rs1; };
  // rob.from_rs_wire_rs2 = [&]() -> auto & { return rs.to_rob_wire_rs2; };
  // rob.from_rs_wire_time = [&]() -> auto & { return rs.to_rob_wire_time; };
  // cpu.run(1000000, true);
  std::array<Bit<32>, 32> regs;
  for (long long o = 0; o < 2000000000000000; o++) {
    regs[0] = 0;
    Bit<32> ins = rob.memory.read_a_word(to_unsigned(rob.memory.pc));
    int pc_change = 0;
    int funct7 = to_unsigned(ins.range<31, 25>());
    int funct3 = to_unsigned(ins.range<14, 12>());
    int opcode = to_unsigned(ins.range<6, 0>());
    int rds = to_unsigned(ins.range<11, 7>());
    int rs1 = to_unsigned(ins.range<19, 15>());
    int rs2 = to_unsigned(ins.range<24, 20>());
    // cout << "PC: " << to_unsigned(rob.memory.pc) << endl << "         ";
    if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0000000) {
      // ADD
      regs[rds] = (regs[rs1] + regs[rs2]);
    } else if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0100000) {
      // SUB
      regs[rds] = (regs[rs1] - regs[rs2]);
    } else if (opcode == 0b0110011 && funct3 == 0b111 && funct7 == 0b0000000) {
      // AND
      regs[rds] = (regs[rs1] & regs[rs2]);
    } else if (opcode == 0b0110011 && funct3 == 0b110 && funct7 == 0b0000000) {
      // OR
      regs[rds] = (regs[rs1] | regs[rs2]);
    } else if (opcode == 0b0110011 && funct3 == 0b100 && funct7 == 0b0000000) {
      // XOR
      regs[rds] = (regs[rs1] ^ regs[rs2]);
    } else if (opcode == 0b0110011 && funct3 == 0b001 && funct7 == 0b0000000) {
      // SLL
      Bit c = regs[rs2];
      int shift = to_unsigned(c.range<4, 0>());
      int d = to_unsigned(regs[rs1]);
      int b = d << shift;
      if (shift > 31) {
        b = 0;
      }
      regs[rds] = b;
    } else if (opcode == 0b0110011 && funct3 == 0b101 && funct7 == 0b0000000) {
      // SRL
      Bit c = regs[rs2];
      int shift = to_unsigned(c.range<4, 0>());
      unsigned int d = to_unsigned(regs[rs1]);
      int b = d >> shift;
      if (shift > 31) {
        b = 0;
      }
      regs[rds] = b;
    } else if (opcode == 0b0110011 && funct3 == 0b101 && funct7 == 0b0100000) {
      // SRA
      Bit c = regs[rs2];
      int shift = to_unsigned(c.range<4, 0>());
      int d = to_unsigned(regs[rs1]);
      int b = d >> shift;
      if (shift > 31) {
        if (c < 0) {
          b = 0xffffffff;
        } else {
          b = 0;
        }
      }
      regs[rds] = b;
    } else if (opcode == 0b0110011 && funct3 == 0b010 && funct7 == 0b0000000) {
      // SLT
      regs[rds] = (to_signed(regs[rs1]) < to_signed(regs[rs2]));
    } else if (opcode == 0b0110011 && funct3 == 0b011 && funct7 == 0b0000000) {
      // SLTU
      regs[rds] = (to_unsigned(regs[rs1]) < to_unsigned(regs[rs2]));
    } else if (opcode == 0b0010011 && funct3 == 0b000) {
      if (rds == 10 && to_unsigned(regs[rs1]) == 0 &&
          to_signed(ins.range<31, 20>()) == 255) {
        Bit a = regs[10];
        std::cout << std::dec << to_unsigned(a.range<7, 0>()) << std::endl;
        exit(0);
      }
      // ADDI
      regs[rds] = (regs[rs1] + to_signed(ins.range<31, 20>()));
      // cout << "ADDI, " << rs1 << '+' << " "
      // << to_signed(ins.range<31, 20>()) << endl;
    } else if (opcode == 0b0010011 && funct3 == 0b111) {
      // ANDI
      regs[rds] = (regs[rs1] & to_signed(ins.range<31, 20>()));
    } else if (opcode == 0b0010011 && funct3 == 0b110) {
      // ORI
      regs[rds] = (regs[rs1] | to_signed(ins.range<31, 20>()));
    } else if (opcode == 0b0010011 && funct3 == 0b100) {
      // XORI
      regs[rds] = (regs[rs1] ^ to_signed(ins.range<31, 20>()));
      // cout << "XORI, " << rs1 << '^' << " "
      // << to_signed(ins.range<31, 20>()) << endl;
    } else if (opcode == 0b0010011 && funct3 == 0b001 && funct7 == 0b0000000) {
      // SLLI
      // if (to_unsigned(ins[24]) == 0) {
      int shift = to_unsigned(ins.range<24, 20>());
      int c = to_unsigned(regs[rs1]);
      int b = c << shift;
      if (shift > 31) {
        b = 0;
      }
      regs[rds] = b;
      // }
    } else if (opcode == 0b0010011 && funct3 == 0b101 && funct7 == 0b0000000) {
      // SRLI
      // if (to_unsigned(ins[24]) == 0) {
      int shift = to_unsigned(ins.range<24, 20>());
      unsigned int c = to_unsigned(regs[rs1]);
      int b = c >> shift;
      if (shift > 31) {
        b = 0;
      }
      regs[rds] = b;
      // }
    } else if (opcode == 0b0010011 && funct3 == 0b101 && funct7 == 0b0100000) {
      // SRAI
      // if (to_unsigned(ins[24]) == 0) {
      int shift = to_unsigned(ins.range<24, 20>());
      int c = to_unsigned(regs[rs1]);
      int b = c >> shift;
      if (shift > 31) {
        if (c < 0) {
          b = 0xffffffff;
        } else {
          b = 0;
        }
      }
      regs[rds] = b;
      // }
    } else if (opcode == 0b0010011 && funct3 == 0b010) {
      // SLTI
      regs[rds] = (to_signed(regs[rs1]) < to_signed(ins.range<31, 20>()));
    } else if (opcode == 0b0010011 && funct3 == 0b011) {
      // SLTIU
      regs[rds] = (to_unsigned(regs[rs1]) < to_unsigned(ins.range<31, 20>()));
    } else if (opcode == 0b0000011 && funct3 == 0b000) {
      // LB
      Bit b = rob.memory.read_byte(
          to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>())));
      regs[rds] = to_signed(b);
    } else if (opcode == 0b0000011 && funct3 == 0b100) {
      // LBU
      Bit b = rob.memory.read_byte(
          to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>())));
      regs[rds] = to_unsigned(b);
    } else if (opcode == 0b0000011 && funct3 == 0b001) {
      // LH
      Bit b = rob.memory.read_half_word(
          to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>())));

      regs[rds] = to_signed(b);
    } else if (opcode == 0b0000011 && funct3 == 0b101) {
      // LHU
      Bit b = rob.memory.read_half_word(
          to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>())));

      regs[rds] = to_unsigned(b);
    } else if (opcode == 0b0000011 && funct3 == 0b010) {
      // LW
      Bit b = rob.memory.read_a_word(
          to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>())));
      // cout << "LW, "
      // << to_unsigned(regs[rs1] + to_signed(ins.range<31, 20>()))
      // << " " << rds << " : " << to_signed(b) << endl;
      regs[rds] = to_signed(b);
    } else if (opcode == 0b0100011 && funct3 == 0b000) {
      // SB
      Bit<32> b = regs[rs2];
      Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
      rob.memory.store_byte(to_unsigned(regs[rs1] + to_signed(imm)),
                            b.range<7, 0>());
    } else if (opcode == 0b0100011 && funct3 == 0b001) {
      // SH
      Bit<32> b = regs[rs2];
      Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
      rob.memory.store_half_word(to_unsigned(regs[rs1] + to_signed(imm)),
                                 b.range<15, 0>());
    } else if (opcode == 0b0100011 && funct3 == 0b010) {
      // SW
      Bit<32> b = regs[rs2];
      Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
      rob.memory.store_a_word(to_unsigned(regs[rs1] + to_signed(imm)), b);
      // cout << "SW, " << hex
      // << to_unsigned(regs[rs1] + to_signed(imm)) << " : "
      // << to_signed(b) << endl;
    } else if (opcode == 0b1100011 && funct3 == 0b000) {
      // BEQ
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (regs[rs1] == regs[rs2]) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100011 && funct3 == 0b101) {
      // BGE
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (to_signed(regs[rs1]) >= to_signed(regs[rs2])) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100011 && funct3 == 0b111) {
      // BGEU
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (to_unsigned(regs[rs1]) >= to_unsigned(regs[rs2])) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100011 && funct3 == 0b100) {
      // BLT
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (to_signed(regs[rs1]) < to_signed(regs[rs2])) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100011 && funct3 == 0b110) {
      // BLTU
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (to_unsigned(regs[rs1]) < to_unsigned(regs[rs2])) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100011 && funct3 == 0b001) {
      // BNE
      Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                 Bit<1>()};
      if (regs[rs1] != regs[rs2]) {
        rob.memory.pc = rob.memory.pc + to_signed(imm);
        pc_change = 1;
      }
    } else if (opcode == 0b1100111 && funct3 == 0b000) {
      // JALR
      regs[rds] = to_unsigned(rob.memory.pc) + 4;
      rob.memory.pc = regs[rs1] + to_signed(ins.range<31, 20>());
      rob.memory.pc[0] = 0;
      pc_change = 1;
      // cout << "JALR, " << hex << to_signed(rob.memory.pc) << endl;
    } else if (opcode == 0b1101111) {
      // JAL
      Bit imm = {ins[31], ins.range<19, 12>(), ins[20], ins.range<30, 21>(),
                 Bit<1>()};
      regs[rds] = to_unsigned(rob.memory.pc) + 4;
      rob.memory.pc = rob.memory.pc + to_signed(imm);
      pc_change = 1;
      // cout << "JAL, " << hex << to_signed(rob.memory.pc) << endl;
    } else if (opcode == 0b0010111) {
      // AUIPC
      Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
      Bit<32> b = to_signed(c);
      regs[rds] = to_unsigned(rob.memory.pc) + b;
    } else if (opcode == 0b0110111) {
      // LUI
      Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
      Bit<32> b = to_signed(c);
      regs[rds] = b;
      // cout << "LUI, " << to_signed(b) << endl;
    } else {
      // cout << "error" << endl;
    }
    if (!pc_change) {
      rob.memory.pc = rob.memory.pc + 4;
    }
    // for (int i = 0; i < 17; i++) {
    //   std::cout << "REG " << dec << i << ": " << hex << to_signed(regs[i])
    //             << ", ";
    // }
    // std::cout << endl;
  }
}