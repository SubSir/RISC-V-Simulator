#include "rs.hpp"
#include "bit.h"
#include "concept.h"
#include "opcode.hpp"
#include "tools.h"
void RS::work() {
  if (rob_rs_get_in) {
    int order = to_unsigned(from_rob_wire_i);
    busy[order] <= 0;
    if (dest[order] == 33) {
      print();
    }
    Bit value = from_rob_wire_value;
    if (reorder_busy[0] && reorder[0] == dest[order]) {
      value = 0;
    }
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] && qj[i] == dest[order]) {
        qj[i] <= 0;
        vj[i] <= value;
      }
      if (busy[i] && qk[i] == dest[order]) {
        qk[i] <= 0;
        vk[i] <= value;
      }
    }
    for (int i = 0; i < 32; i++) {
      if (reorder_busy[i] && reorder[i] == dest[order]) {
        reorder_busy[i] <= 0;
        regs[i] <= value;
      }
    }
  }
  if (rob_error) {
    rob_get_out <= 0;
    for (int i = 0; i < RS_SIZE; i++) {
      busy[i] <= 0;
    }
    for (int i = 0; i < 32; i++) {
      reorder_busy[i] <= 0;
    }
    to_memory <= 1;
    return;
  }
  int tmp = -1;
  max_size_t des = 0;
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 1) {
      des = std::max(des, to_unsigned(dest[i]));
    }
  }
  if (rs_get_in && from_memory_wire != 0) {
    des++;
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 0) {
        busy[i] <= 1;
        tmp = i;
        Bit<32> ins = from_memory_wire;
        Bit<7> funct7 = ins.range<31, 25>();
        Bit<3> funct3 = ins.range<14, 12>();
        Bit<7> opcode = ins.range<6, 0>();
        Bit<5> rds = ins.range<11, 7>();
        Bit<5> rs1 = ins.range<19, 15>();
        Bit<5> rs2 = ins.range<24, 20>();
        bool use1 = 1, use2 = 1, userd = 1;
        dest[i] <= des;
        pc[i] <= pc_wire;
        commited[i] <= 0;
        if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0000000) {
          // ADD
          op[i] <= ADD;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b000 &&
                   funct7 == 0b0100000) {
          // SUB
          op[i] <= SUB;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b111 &&
                   funct7 == 0b0000000) {
          // AND
          op[i] <= AND;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b110 &&
                   funct7 == 0b0000000) {
          // OR
          op[i] <= OR;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b100 &&
                   funct7 == 0b0000000) {
          // XOR
          op[i] <= XOR;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLL
          op[i] <= SLL;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRL
          op[i] <= SRL;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRA
          op[i] <= SRA;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b010 &&
                   funct7 == 0b0000000) {
          // SLT
          op[i] <= SLT;
          time[i] <= 1;
        } else if (opcode == 0b0110011 && funct3 == 0b011 &&
                   funct7 == 0b0000000) {
          // SLTU
          op[i] <= SLTU;
          time[i] <= 1;
        } else if (opcode == 0b0010011 && funct3 == 0b000) {
          // ADDI
          op[i] <= ADDI;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b111) {
          // ANDI
          op[i] <= ANDI;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b110) {
          // ORI
          op[i] <= ORI;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b100) {
          // XORI
          op[i] <= XORI;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLLI
          op[i] <= SLLI;
          use2 = 0;
          time[i] <= 1;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRLI
          op[i] <= SRLI;
          use2 = 0;
          time[i] <= 1;
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRAI
          op[i] <= SRAI;
          use2 = 0;
          time[i] <= 1;
        } else if (opcode == 0b0010011 && funct3 == 0b010) {
          // SLTI
          op[i] <= SLTI;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b011) {
          // SLTIU
          op[i] <= SLTIU;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0000011 && funct3 == 0b000) {
          // LB
          op[i] <= LB;
          use2 = 0;
          time[i] <= 3;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0000011 && funct3 == 0b100) {
          // LBU
          op[i] <= LBU;
          use2 = 0;
          time[i] <= 3;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0000011 && funct3 == 0b001) {
          // LH
          op[i] <= LH;
          use2 = 0;
          time[i] <= 3;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0000011 && funct3 == 0b101) {
          // LHU
          op[i] <= LHU;
          use2 = 0;
          time[i] <= 3;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0000011 && funct3 == 0b010) {
          // LW
          op[i] <= LW;
          use2 = 0;
          time[i] <= 3;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0100011 && funct3 == 0b000) {
          // SB
          op[i] <= SB;
          userd = 0;
          time[i] <= 3;
          Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b0100011 && funct3 == 0b001) {
          // SH
          op[i] <= SH;
          userd = 0;
          time[i] <= 3;
          Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b0100011 && funct3 == 0b010) {
          // SW
          op[i] <= SW;
          userd = 0;
          time[i] <= 3;
          Bit imm = {ins.range<31, 25>(), ins.range<11, 7>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b000) {
          // BEQ
          op[i] <= BEQ;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b101) {
          // BGE
          op[i] <= BGE;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b111) {
          // BGEU
          op[i] <= BGEU;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b100) {
          // BLT
          op[i] <= BLT;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b110) {
          // BLTU
          op[i] <= BLTU;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b001) {
          // BNE
          op[i] <= BNE;
          userd = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1101111 && funct3 == 0b000) {
          // JALR
          op[i] <= JALR;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b1101111) {
          // JAL
          op[i] <= JAL;
          use1 = 0;
          use2 = 0;
          time[i] <= 1;
          Bit imm = {ins[31], ins.range<19, 12>(), ins[20], ins.range<30, 21>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b0010111) {
          // AUIPC
          op[i] <= AUIPC;
          use1 = 0;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 12>());
        } else if (opcode == 0b0110111) {
          // LUI
          op[i] <= LUI;
          use1 = 0;
          use2 = 0;
          time[i] <= 1;
          a[i] <= to_signed(ins.range<31, 12>());
        } else if (ins == 0x0ff00513) {
          op[i] <= PRINT;
          use1 = 0;
          use2 = 0;
          userd = 0;
          time[i] <= 0;
        } else {
          op[i] <= ELSE;
          use1 = 0;
          use2 = 0;
          userd = 1;
          rds = 33;
          time[i] <= 0;
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
        if (userd == 0) {
          rd[i] <= 32;
        } else {
          rd[i] <= to_unsigned(rds);
        }
        break;
      }
    }
  }
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] && op[i] == ELSE) {
      busy[i] <= 0;
    }
  }
  bool flag = 0, rob_get_out_flag = 0;
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 0 and i != tmp) {
      flag = 1;
      break;
    }
  }
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] && qj[i] == 0 && qk[i] == 0 && commited[i] == 0 &&
        op[i] != ELSE) {
      if (from_rob) {
        commited[i] <= 1;
        rob_get_out <= 1;
        rob_get_out_flag = 1;
        to_rob_wire_op <= +op[i];
        to_rob_wire_rs1 <= +vj[i];
        to_rob_wire_rs2 <= +vk[i];
        to_rob_wire_dest <= +dest[i];
        to_rob_wire_a <= +a[i];
        to_rob_wire_pc <= +pc[i];
        to_rob_wire_i <= i;
        to_rob_wire_time <= +time[i];

        if (to_unsigned(rd[i]) < 32) {
          reorder_busy[to_unsigned(rd[i])] <= 1;
          reorder[to_unsigned(rd[i])] <= dest[i];
        }
        break;
      }
    }
  }
  if (!rob_get_out_flag) {
    rob_get_out <= 0;
  }
  to_memory <= flag;
}

void RS::print() {
  Bit a = regs[10];
  std::cout << to_unsigned(a.range<7, 0>()) << std::endl;
}