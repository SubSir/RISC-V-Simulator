#include "rs.hpp"
#include "concept.h"
#include "opcode.hpp"
#include "tools.h"
void RS::work() {
  int tmp = -1;
  if (from_memory) {
    from_memory = []() { return 0; };
    max_size_t des = 0;
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 1) {
        des = std::max(des, to_unsigned(dest[i]));
      }
    }
    des++;
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 0 && from_memory_wire != 0) {
        busy[i] <= 1;
        tmp = i;
        Bit<32> ins = from_memory_wire;
        Bit<7> funct7 = ins.range<31, 25>();
        Bit<3> funct3 = ins.range<14, 12>();
        Bit<7> opcode = ins.range<6, 0>();
        Bit<5> rd = ins.range<11, 7>();
        Bit<5> rs1 = ins.range<19, 15>();
        Bit<5> rs2 = ins.range<24, 20>();
        a[i] <= to_signed(funct7);
        dest[i] <= des;
        if (reorder_busy[to_unsigned(rs1)]) {
          vj[i] <= 0;
          qj[i] <= reorder[to_unsigned(rs1)];
        } else {
          vj[i] <= regs[to_unsigned(rs1)];
          qj[i] <= 0;
        }
        if (reorder_busy[to_unsigned(rs2)]) {
          vk[i] <= 0;
          qk[i] <= reorder[to_unsigned(rs2)];
        } else {
          vk[i] <= regs[to_unsigned(rs2)];
          qk[i] <= 0;
        }
        if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0000000) {
          // ADD
          op[i] <= ADD;
        } else if (opcode == 0b0110011 && funct3 == 0b000 &&
                   funct7 == 0b0100000) {
          // SUB
          op[i] <= SUB;
        } else if (opcode == 0b0110011 && funct3 == 0b111 &&
                   funct7 == 0b0000000) {
          // AND
          op[i] <= AND;
        } else if (opcode == 0b0110011 && funct3 == 0b110 &&
                   funct7 == 0b0000000) {
          // OR
          op[i] <= OR;
        } else if (opcode == 0b0110011 && funct3 == 0b100 &&
                   funct7 == 0b0000000) {
          // XOR
          op[i] <= XOR;
        } else if (opcode == 0b0110011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLL
          op[i] <= SLL;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRL
          op[i] <= SRL;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRA
          op[i] <= SRA;
        } else if (opcode == 0b0110011 && funct3 == 0b010 &&
                   funct7 == 0b0000000) {
          // SLT
          op[i] <= SLT;
        } else if (opcode == 0b0110011 && funct3 == 0b011 &&
                   funct7 == 0b0000000) {
          // SLTU
          op[i] <= SLTU;
        } else if (opcode == 0b0010011 && funct3 == 0b000) {
          // ADDI
          op[i] <= ADDI;
        } else if (opcode == 0b0010011 && funct3 == 0b111) {
          // ANDI
          op[i] <= ANDI;
        } else if (opcode == 0b0010011 && funct3 == 0b110) {
          // ORI
          op[i] <= ORI;
        } else if (opcode == 0b0010011 && funct3 == 0b100) {
          // XORI
          op[i] <= XORI;
        } else if (opcode == 0b0010011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLLI
          op[i] <= SLLI;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRLI
          op[i] <= SRLI;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRAI
          op[i] <= SRAI;
        } else if (opcode == 0b0010011 && funct3 == 0b010) {
          // SLTI
          op[i] <= SLTI;
        } else if (opcode == 0b0010011 && funct3 == 0b011) {
          // SLTIU
          op[i] <= SLTIU;
          // } else if (opcode == 0b0000011 && funct3 == 0b000) {
          //   // LB
          //   op[i] <= LB;
          // } else if (opcode == 0b0000011 && funct3 == 0b100) {
          //   // LBU
          //   op[i] <= LBU;
          // } else if (opcode == 0b0000011 && funct3 == 0b001) {
          //   // LH
          //   op[i] <= LH;
          // } else if (opcode == 0b0000011 && funct3 == 0b101) {
          //   // LHU
          //   op[i] <= LHU;
          // } else if (opcode == 0b0000011 && funct3 == 0b010) {
          //   // LW
          //   op[i] <= LW;
          // } else if (opcode == 0b0100011 && funct3 == 0b000) {
          //   // SB
          //   op[i] <= SB;
          // } else if (opcode == 0b0100011 && funct3 == 0b001) {
          //   // SH
          //   op[i] <= SH;
          // } else if (opcode == 0b0100011 && funct3 == 0b010) {
          //   // SW
          //   op[i] <= SW;
        } else if (opcode == 0b1100011 && funct3 == 0b000) {
          // BEQ
          op[i] <= BEQ;
        } else if (opcode == 0b1100011 && funct3 == 0b101) {
          // BGE
          op[i] <= BGE;
        } else if (opcode == 0b1100011 && funct3 == 0b111) {
          // BGEU
          op[i] <= BGEU;
        } else if (opcode == 0b1100011 && funct3 == 0b100) {
          // BLT
          op[i] <= BLT;
        } else if (opcode == 0b1100011 && funct3 == 0b110) {
          // BLTU
          op[i] <= BLTU;
        } else if (opcode == 0b1100011 && funct3 == 0b001) {
          // BNE
          op[i] <= BNE;
        } else if (opcode == 0b1101111 && funct3 == 0b000) {
          // JALR
          op[i] <= JALR;
        } else if (opcode == 0b1100011) {
          // JAL
          op[i] <= JAL;
        } else if (opcode == 0b0010111) {
          // AUIPC
          op[i] <= AUIPC;
        } else if (opcode == 0b0110111) {
          // LUI
          op[i] <= LUI;
        }
        break;
      }
    }
  }
  bool flag = 0;

  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 0 and i != tmp) {
      flag = 1;
      break;
    }
  }
  to_memory = [flag]() { return flag; };
}