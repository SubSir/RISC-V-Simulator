#include "rs.hpp"
#include "bit.h"
#include "concept.h"
#include "opcode.hpp"
#include "tools.h"
#include <iostream>
void RS::work() {
  int free_rd = -1;
  Bit<32> free_rd_value = 0;
  if (rob_rs_get_in) {
    int order = to_unsigned(from_rob_wire_i);
    if (rob_error == 0) {
      head <= order;
      busy[order] <= 0;
    }
    if (from_rob_wire_update) {
      if (op[order] == ADDI && rd[order] == 10 && vj[order] == 0 &&
          a[order] == 255) {
        print();
      }
      Bit<32> value = from_rob_wire_value;
      if (rd[order] == 0) {
        value = 0;
      }
      // std::cout << "         " << to_unsigned(rd[order]) << " : "
      // << to_signed(value) << std::endl;
      for (int i = to_unsigned(head); i < to_unsigned(pos) + 2; i++) {
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
  }
  if (rob_error) {
    rob_get_out <= 0;
    head <= 0;
    pos <= 0;
    for (int i = to_unsigned(head); i < to_unsigned(pos) + 2; i++) {
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
  if (rs_get_in && from_memory_op != ELSE) {
    pos <= pos + 1;
    // if (pc_wire == 0x113c) {
    //   std::cout << "error" << std::endl;
    // }
    int i = to_unsigned(pos);
    for (; i < RS_SIZE; i++) {
      if (busy[i] == 0) {
        break;
      }
    }
    busy[i] <= 1;
    tmp = i;
    int opcode = to_unsigned(from_memory_op);
    int rds = to_unsigned(from_memory_rd);
    int rs1 = to_unsigned(from_memory_rs1);
    int rs2 = to_unsigned(from_memory_rs2);
    bool use1 = 1, use2 = 1, userd = 1;
    dest[i] <= des;
    pc[i] <= pc_wire;
    commited[i] <= 0;
    if (opcode == ADD) {
      // ADD
      op[i] <= ADD;
      time[i] <= 0;
    } else if (opcode == SUB) {
      // SUB
      op[i] <= SUB;
      time[i] <= 0;
    } else if (opcode == AND) {
      // AND
      op[i] <= AND;
      time[i] <= 0;
    } else if (opcode == OR) {
      // OR
      op[i] <= OR;
      time[i] <= 0;
    } else if (opcode == XOR) {
      // XOR
      op[i] <= XOR;
      time[i] <= 0;
    } else if (opcode == SLL) {
      // SLL
      op[i] <= SLL;
      time[i] <= 0;
    } else if (opcode == SRL) {
      // SRL
      op[i] <= SRL;
      time[i] <= 0;
    } else if (opcode == SRA) {
      // SRA
      op[i] <= SRA;
      time[i] <= 0;
    } else if (opcode == SLT) {
      // SLT
      op[i] <= SLT;
      time[i] <= 0;
    } else if (opcode == SLTU) {
      // SLTU
      op[i] <= SLTU;
      time[i] <= 0;
    } else if (opcode == ADDI) {
      // ADDI
      op[i] <= ADDI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;
    } else if (opcode == ANDI) {
      // ANDI
      op[i] <= ANDI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == ORI) {
      // ORI
      op[i] <= ORI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == XORI) {
      // XORI
      op[i] <= XORI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == SLLI) {
      // SLLI
      op[i] <= SLLI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == SRLI) {
      // SRLI
      op[i] <= SRLI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == SRAI) {
      // SRAI
      op[i] <= SRAI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == SLTI) {
      // SLTI
      op[i] <= SLTI;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == SLTIU) {
      // SLTIU
      op[i] <= SLTIU;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == LB) {
      // LB
      op[i] <= LB;
      use2 = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == LBU) {
      // LBU
      op[i] <= LBU;
      use2 = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == LH) {
      // LH
      op[i] <= LH;
      use2 = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == LHU) {
      // LHU
      op[i] <= LHU;
      use2 = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == LW) {
      // LW
      op[i] <= LW;
      use2 = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

      // std::cout << "lw,rd = " << rds
      //           << " ,rs1 = " << rs1 << " ,i = " << i
      //           << std::endl;
    } else if (opcode == SB) {
      // SB
      op[i] <= SB;
      userd = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == SH) {
      // SH
      op[i] <= SH;
      userd = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == SW) {
      // SW
      op[i] <= SW;
      userd = 0;
      time[i] <= 3;
      a[i] <= from_memory_a;

    } else if (opcode == BEQ) {
      // BEQ
      op[i] <= BEQ;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == BGE) {
      // BGE
      op[i] <= BGE;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == BGEU) {
      // BGEU
      op[i] <= BGEU;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == BLT) {
      // BLT
      op[i] <= BLT;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == BLTU) {
      // BLTU
      op[i] <= BLTU;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == BNE) {
      // BNE
      op[i] <= BNE;
      userd = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == JALR) {
      // JALR
      op[i] <= JALR;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == JAL) {
      // JAL
      op[i] <= JAL;
      use1 = 0;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == AUIPC) {
      // AUIPC
      op[i] <= AUIPC;
      use1 = 0;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else if (opcode == LUI) {
      // LUI
      op[i] <= LUI;
      use1 = 0;
      use2 = 0;
      time[i] <= 0;
      a[i] <= from_memory_a;

    } else {
      op[i] <= ELSE;
      use1 = 0;
      use2 = 0;
      userd = 0;
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
  }
  if (!twice and free_rd != -1) {
    reorder_busy[free_rd] <= 0;
  }
  int cnt = 0, rob_get_out_flag = 0;
  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 0 and i != tmp) {
      cnt++;
      if (cnt > 2) {
        break;
      }
    }
  }
  for (int i = to_unsigned(head); i < to_unsigned(pos) + 2; i++) {
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
  to_memory <= (cnt > 1);
}

void RS::print() {
  Bit<32> a = regs[10];
  std::cout << std::dec << to_unsigned(a.range<7, 0>()) << std::endl;
  exit(0);
}