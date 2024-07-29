#include "rs.hpp"
#include "bit.h"
#include "concept.h"
#include "opcode.hpp"
#include "tools.h"
void RS::work() {
  int free_rd = -1;
  Bit<32> free_rd_value = 0;
  if (rob_rs_get_in) {
    int order = to_unsigned(from_rob_wire_i);
    if (rob_error == 0)
      busy[order] <= 0;
    if (op[order] == ADDI && rd[order] == 10 && vj[order] == 0 &&
        a[order] == 255) {
      print();
    }
    Bit value = from_rob_wire_value;
    if (rd[order] == 0) {
      value = 0;
    }
    // std::cout << "         " << to_unsigned(rd[order]) << " : "
    // << to_signed(value) << std::endl;
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
        if (rob_error == 0)
          free_rd = i;
      }
    }
    regs[to_unsigned(rd[order])] <= value;
    free_rd_value = value;
  }
  if (rob_error) {
    rob_get_out <= 0;
    pos <= 0;
    for (int i = 0; i < RS_SIZE; i++) {
      busy[i] <= 0;
    }
    for (int i = 0; i < 32; i++) {
      reorder_busy[i] <= 0;
    }
    to_memory <= 1;
    return;
  }
  int tmp = -1, twice = 0;
  max_size_t des = to_unsigned(pos + 1);
  if (rs_get_in && from_memory_wire != 0) {
    pos <= pos + 1;
    // if (pc_wire == 0x113c) {
    //   std::cout << "error" << std::endl;
    // }
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 0) {
        busy[i] <= 1;
        tmp = i;
        Bit<32> ins = from_memory_wire;
        int funct7 = to_unsigned(ins.range<31, 25>());
        int funct3 = to_unsigned(ins.range<14, 12>());
        int opcode = to_unsigned(ins.range<6, 0>());
        int rds = to_unsigned(ins.range<11, 7>());
        int rs1 = to_unsigned(ins.range<19, 15>());
        int rs2 = to_unsigned(ins.range<24, 20>());
        bool use1 = 1, use2 = 1, userd = 1;
        dest[i] <= des;
        pc[i] <= pc_wire;
        commited[i] <= 0;
        if (opcode == 0b0110011 && funct3 == 0b000 && funct7 == 0b0000000) {
          // ADD
          op[i] <= ADD;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b000 &&
                   funct7 == 0b0100000) {
          // SUB
          op[i] <= SUB;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b111 &&
                   funct7 == 0b0000000) {
          // AND
          op[i] <= AND;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b110 &&
                   funct7 == 0b0000000) {
          // OR
          op[i] <= OR;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b100 &&
                   funct7 == 0b0000000) {
          // XOR
          op[i] <= XOR;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLL
          op[i] <= SLL;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRL
          op[i] <= SRL;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRA
          op[i] <= SRA;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b010 &&
                   funct7 == 0b0000000) {
          // SLT
          op[i] <= SLT;
          time[i] <= 0;
        } else if (opcode == 0b0110011 && funct3 == 0b011 &&
                   funct7 == 0b0000000) {
          // SLTU
          op[i] <= SLTU;
          time[i] <= 0;
        } else if (opcode == 0b0010011 && funct3 == 0b000) {
          // ADDI
          op[i] <= ADDI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b111) {
          // ANDI
          op[i] <= ANDI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b110) {
          // ORI
          op[i] <= ORI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b100) {
          // XORI
          op[i] <= XORI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b001 &&
                   funct7 == 0b0000000) {
          // SLLI
          op[i] <= SLLI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_unsigned(ins.range<24, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0000000) {
          // SRLI
          op[i] <= SRLI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_unsigned(ins.range<24, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b101 &&
                   funct7 == 0b0100000) {
          // SRAI
          op[i] <= SRAI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_unsigned(ins.range<24, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b010) {
          // SLTI
          op[i] <= SLTI;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b0010011 && funct3 == 0b011) {
          // SLTIU
          op[i] <= SLTIU;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_unsigned(ins.range<31, 20>());
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
          // std::cout << "lw,rd = " << rds
          //           << " ,rs1 = " << rs1 << " ,i = " << i
          //           << std::endl;
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
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b101) {
          // BGE
          op[i] <= BGE;
          userd = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b111) {
          // BGEU
          op[i] <= BGEU;
          userd = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b100) {
          // BLT
          op[i] <= BLT;
          userd = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b110) {
          // BLTU
          op[i] <= BLTU;
          userd = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100011 && funct3 == 0b001) {
          // BNE
          op[i] <= BNE;
          userd = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins[7], ins.range<30, 25>(), ins.range<11, 8>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b1100111 && funct3 == 0b000) {
          // JALR
          op[i] <= JALR;
          use2 = 0;
          time[i] <= 0;
          a[i] <= to_signed(ins.range<31, 20>());
        } else if (opcode == 0b1101111) {
          // JAL
          op[i] <= JAL;
          use1 = 0;
          use2 = 0;
          time[i] <= 0;
          Bit imm = {ins[31], ins.range<19, 12>(), ins[20], ins.range<30, 21>(),
                     Bit<1>()};
          a[i] <= to_signed(imm);
        } else if (opcode == 0b0010111) {
          // AUIPC
          op[i] <= AUIPC;
          use1 = 0;
          use2 = 0;
          time[i] <= 0;
          Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
          a[i] <= to_signed(c);
        } else if (opcode == 0b0110111) {
          // LUI
          op[i] <= LUI;
          use1 = 0;
          use2 = 0;
          time[i] <= 0;
          Bit<32> c = {ins.range<31, 12>(), Bit<12>()};
          a[i] <= to_signed(c);
        } else {
          op[i] <= ELSE;
          use1 = 0;
          use2 = 0;
          userd = 1;
          rds = 33;
          time[i] <= 0;
        }
        if (use1 && (reorder_busy[rs1] && free_rd != rs1)) {
          vj[i] <= 0;
          qj[i] <= reorder[rs1];
        } else if (use1 && free_rd == rs1) {
          vj[i] <= free_rd_value;
          qj[i] <= 0;
        } else if (use1) {
          vj[i] <= regs[rs1];
          qj[i] <= 0;
        } else {
          vj[i] <= 0;
          qj[i] <= 0;
        }
        if (use2 && (reorder_busy[rs2] && free_rd != rs2)) {
          vk[i] <= 0;
          qk[i] <= reorder[rs2];
        } else if (use2 && free_rd == rs2) {
          vk[i] <= free_rd_value;
          qk[i] <= 0;
        } else if (use2) {
          vk[i] <= regs[rs2];
          qk[i] <= 0;
        } else {
          vk[i] <= 0;
          qk[i] <= 0;
        }
        if (userd) {
          reorder_busy[rds] <= 1;
          reorder[rds] <= des;
          rd[i] <= rds;
          if (rds == free_rd) {
            twice = 1;
          }
        } else {
          rd[i] <= 32;
        }
        break;
      }
    }
  }
  if (!twice and free_rd != -1) {
    reorder_busy[free_rd] <= 0;
  }
  bool flag = 0, rob_get_out_flag = 0;
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 0 and i != tmp) {
      flag = 1;
      break;
    }
  }
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] && qj[i] == 0 && qk[i] == 0 && commited[i] == 0) {
      if (from_rob) {
        // if (op[i] == LW) {
        //   std::cout << "lw: i = " << i << std::endl;
        // }
        // std::cout << std::dec << "          commited: i = " << i
        //           << " op = " << to_unsigned(op[i]) << std::endl;
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
  std::cout << std::dec << to_unsigned(a.range<7, 0>()) << std::endl;
  exit(0);
}