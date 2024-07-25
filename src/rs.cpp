#include "rs.hpp"
#include "concept.h"
#include "opcode.hpp"
#include "tools.h"
void RS::work() {
  int tmp = -1;
  max_size_t des = 0;
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 1) {
      des = std::max(des, to_unsigned(dest[i]));
    }
  }
  if (rs_get) {
    rs_get = []() { return 0; };
    des++;
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 0 && from_memory_wire != 0) {
        busy[i] <= 1;
        tmp = i;
        Bit<32> ins = from_memory_wire;
        Bit<7> funct7 = ins.range<31, 25>();
        Bit<3> funct3 = ins.range<14, 12>();
        Bit<7> opcode = ins.range<6, 0>();
        Bit<5> rds = ins.range<11, 7>();
        Bit<5> rs1 = ins.range<19, 15>();
        Bit<5> rs2 = ins.range<24, 20>();
        bool use1 = 1, use2 = 1;
        a[i] <= to_signed(funct7);
        dest[i] <= des;
        rd[i] <= to_signed(rds);
        pc[i] <= pc_wire;
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
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b111) {
          // ANDI
          op[i] <= ANDI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b110) {
          // ORI
          op[i] <= ORI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b100) {
          // XORI
          op[i] <= XORI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLLI
          op[i] <= SLLI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRLI
          op[i] <= SRLI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRAI
          op[i] <= SRAI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b010) {
          // SLTI
          op[i] <= SLTI;
          use2 = 0;
        } else if (opcode == 0b0010011 && funct3 == 0b011) {
          // SLTIU
          op[i] <= SLTIU;
          use2 = 0;
        } else if (opcode == 0b0000011 && funct3 == 0b000) {
          // LB
          op[i] <= LB;
          use2 = 0;
        } else if (opcode == 0b0000011 && funct3 == 0b100) {
          // LBU
          op[i] <= LBU;
          use2 = 0;
        } else if (opcode == 0b0000011 && funct3 == 0b001) {
          // LH
          op[i] <= LH;
          use2 = 0;
        } else if (opcode == 0b0000011 && funct3 == 0b101) {
          // LHU
          op[i] <= LHU;
          use2 = 0;
        } else if (opcode == 0b0000011 && funct3 == 0b010) {
          // LW
          op[i] <= LW;
          use2 = 0;
        } else if (opcode == 0b0100011 && funct3 == 0b000) {
          // SB
          op[i] <= SB;
          use2 = 0;
        } else if (opcode == 0b0100011 && funct3 == 0b001) {
          // SH
          op[i] <= SH;
          use2 = 0;
        } else if (opcode == 0b0100011 && funct3 == 0b010) {
          // SW
          op[i] <= SW;
          use2 = 0;
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
          use2 = 0;
        } else if (opcode == 0b1100011) {
          // JAL
          op[i] <= JAL;
          use1 = 0;
          use2 = 0;
        } else if (opcode == 0b0010111) {
          // AUIPC
          op[i] <= AUIPC;
          use1 = 0;
          use2 = 0;
        } else if (opcode == 0b0110111) {
          // LUI
          op[i] <= LUI;
          use1 = 0;
          use2 = 0;
        } else {
          use1 = 0;
          use2 = 0;
        }
        if (use1 && reorder_busy[to_unsigned(rs1)]) {
          vj[i] <= 0;
          qj[i] <= reorder[to_unsigned(rs1)];
        } else if (use1) {
          vj[i] <= regs[to_unsigned(rs1)];
          qj[i] <= 0;
        } else {
          vj[i] <= 0;
          qj[i] <= 0;
        }
        if (use2 && reorder_busy[to_unsigned(rs2)]) {
          vk[i] <= 0;
          qk[i] <= reorder[to_unsigned(rs2)];
        } else if (use2) {
          vk[i] <= regs[to_unsigned(rs2)];
          qk[i] <= 0;
        } else {
          vk[i] <= 0;
          qk[i] <= 0;
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
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] && qj[i] == 0 && qk[i] == 0) {
      if (from_rob) {
        if (rob_get == 0) {
          busy[i] <= 0;
          rob_get = []() { return 1; };
          to_rob_wire_op = [this, i]() { return +op[i]; };
          to_rob_wire_rd = [this, i]() { return +rd[i]; };
          to_rob_wire_rs1 = [this, i]() { return +vj[i]; };
          to_rob_wire_rs2 = [this, i]() { return +vk[i]; };
          to_rob_wire_dest = [this, i]() { return +dest[i]; };
          to_rob_wire_a = [this, i]() { return +a[i]; };
          to_rob_wire_pc = [this, i]() { return +pc[i]; };
          flag = 1;
        }
      }
    }
  }

  if (rob_error) {
    rs_get = []() { return 0; };
    rob_get = []() { return 0; };
    for (int i = 0; i < RS_SIZE; i++) {
      busy[i] <= 0;
    }
    flag = 1;
  }
  to_memory = [flag]() { return flag; };
}