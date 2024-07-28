#include "rob.hpp"
#include "bit.h"
#include "opcode.hpp"
#include "tools.h"
#include <ostream>

void RoB::work() {
  // terms++;
  // std::cout << "ROB Term is" << terms << std::endl;
  // if (terms == 8) {
  //   std::cout << "ROB Term is" << terms << std::endl;
  // }
  memory.work();
  bool twice = 0, error = 0, head_twice = 0, pos_shift = 0;
  int head = (to_unsigned(pos) + 1) % ROB_SIZE;
  int to_rs_flag = 0;
  std::cout << std::hex;
  if (busy[head]) {
    if (time[head] == 0) {
      int value = -1;
      busy[head] <= 0;
      head_twice = 1;
      pos_shift = 1;
      to_rs_flag = 1;
      std::cout << "PC = " << to_unsigned(pc[head]) << ' ';
      if (op[head] == ADD) {
        to_rs_wire_value <= (rs1[head] + rs2[head]);
        std::cout << "ADD " << to_unsigned(rs1[head]) << " + "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SUB) {
        to_rs_wire_value <= (rs1[head] - rs2[head]);
        std::cout << "SUB " << to_unsigned(rs1[head]) << " - "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == AND) {
        to_rs_wire_value <= (rs1[head] & rs2[head]);
        std::cout << "AND " << to_unsigned(rs1[head]) << " & "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == OR) {
        to_rs_wire_value <= (rs1[head] | rs2[head]);
        std::cout << "OR " << to_unsigned(rs1[head]) << " | "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == XOR) {
        to_rs_wire_value <= (rs1[head] ^ rs2[head]);
        std::cout << "XOR " << to_unsigned(rs1[head]) << " ^ "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLL) {
        to_rs_wire_value <= (rs1[head] << rs2[head]);
        std::cout << "SLL " << to_unsigned(rs1[head]) << " << "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SRA) {
        to_rs_wire_value <= (rs1[head] >> rs2[head]);
        std::cout << "SRA " << to_unsigned(rs1[head]) << " >> "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SRL) {
        to_rs_wire_value <= (rs1[head] >> rs2[head]);
        std::cout << "SRL " << to_unsigned(rs1[head]) << " >> "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLT) {
        to_rs_wire_value <= (to_signed(rs1[head]) < to_signed(rs2[head]));
        std::cout << "SLT " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLTU) {
        to_rs_wire_value <= (to_unsigned(rs1[head]) < to_unsigned(rs2[head]));
        std::cout << "SLTU " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == ADDI) {
        to_rs_wire_value <= (rs1[head] + a[head]);
        std::cout << "ADDI " << to_unsigned(rs1[head]) << " + "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == ANDI) {
        to_rs_wire_value <= (rs1[head] & a[head]);
        std::cout << "ANDI " << to_unsigned(rs1[head]) << " & "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == ORI) {
        to_rs_wire_value <= (rs1[head] | a[head]);
        std::cout << "ORI " << to_unsigned(rs1[head]) << " | "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == XORI) {
        to_rs_wire_value <= (rs1[head] ^ a[head]);
        std::cout << "XORI " << to_unsigned(rs1[head]) << " ^ "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLLI) {
        to_rs_wire_value <= (rs1[head] << a[head]);
        std::cout << "SLLI " << to_unsigned(rs1[head]) << " << "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SRLI) {
        to_rs_wire_value <= (rs1[head] >> a[head]);
        std::cout << "SRLI " << to_unsigned(rs1[head]) << " >> "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SRAI) {
        to_rs_wire_value <= (rs1[head] >> a[head]);
        std::cout << "SRAI " << to_unsigned(rs1[head]) << " >> "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLTI) {
        to_rs_wire_value <= (to_signed(rs1[head]) < to_signed(a[head]));
        std::cout << "SLTI " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLTIU) {
        to_rs_wire_value <= (to_unsigned(rs1[head]) < to_unsigned(a[head]));
        std::cout << "SLTIU " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == LB) {
        Bit b = memory.read_byte(to_unsigned(rs1[head] + a[head]));
        to_rs_wire_value <= to_signed(b);
        std::cout << "LB " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == LBU) {
        Bit b = memory.read_byte(to_unsigned(rs1[head] + a[head]));
        to_rs_wire_value <= to_unsigned(b);
        std::cout << "LBU " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == LH) {
        Bit b = memory.read_half_word(to_unsigned(rs1[head] + a[head]));

        to_rs_wire_value <= to_signed(b);
        std::cout << "LH " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == LHU) {
        Bit b = memory.read_half_word(to_unsigned(rs1[head] + a[head]));

        to_rs_wire_value <= to_unsigned(b);
        std::cout << "LHU " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == LW) {
        Bit b = memory.read_a_word(to_unsigned(rs1[head] + a[head]));

        to_rs_wire_value <= to_signed(b);
        std::cout << "LW " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == SB) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_byte(to_unsigned(rs1[head] + a[head]), b.range<7, 0>());
        std::cout << "SB " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << " : " << to_unsigned(rs2[head])
                  << std::endl;
      } else if (op[head] == SH) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_half_word(to_unsigned(rs1[head] + a[head]),
                               b.range<15, 0>());
        std::cout << "SH " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << " : " << to_unsigned(rs2[head])
                  << std::endl;
      } else if (op[head] == SW) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_a_word(to_unsigned(rs1[head] + a[head]), b);
        std::cout << "SW " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << " : " << to_unsigned(rs2[head])
                  << std::endl;
      } else if (op[head] == BEQ) {
        to_rs_flag = 0;
        if (rs1[head] == rs2[head]) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BEQ " << to_unsigned(rs1[head])
                  << " == " << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == BGE) {
        to_rs_flag = 0;
        if (to_signed(rs1[head]) >= to_signed(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BGE " << to_unsigned(rs1[head])
                  << " >= " << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == BGEU) {
        to_rs_flag = 0;
        if (to_unsigned(rs1[head]) >= to_unsigned(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BGEU " << to_unsigned(rs1[head])
                  << " >= " << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == BLTU) {
        to_rs_flag = 0;
        if (to_unsigned(rs1[head]) < to_unsigned(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BLTU " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == BLT) {
        to_rs_flag = 0;
        if (to_signed(rs1[head]) < to_signed(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BLT " << to_unsigned(rs1[head]) << " < "
                  << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == BNE) {
        to_rs_flag = 0;
        if (rs1[head] != rs2[head]) {
          memory.pc <= pc[head] + a[head];
          error = 1;
        }
        std::cout << "BNE " << to_unsigned(rs1[head])
                  << " != " << to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == JAL) {
        to_rs_wire_value <= to_unsigned(pc[head]) + 4;
        memory.pc <= pc[head] + a[head];
        error = 1;
        std::cout << "JAL " << to_unsigned(pc[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == JALR) {
        to_rs_wire_value <= to_unsigned(pc[head]) + 4;
        memory.pc <= rs1[head] + a[head];
        error = 1;
        std::cout << "JALR " << to_unsigned(rs1[head]) << " + "
                  << to_signed(a[head]) << std::endl;
      } else if (op[head] == AUIPC) {
        Bit<32> b = a[head];
        to_rs_wire_value <= to_unsigned(pc[head]) + b;
        std::cout << "AUIPC " << to_unsigned(pc[head]) << " + " << to_signed(b)
                  << std::endl;
      } else if (op[head] == LUI) {
        Bit<32> b = a[head];
        to_rs_wire_value <= b;
        std::cout << "LUI " << to_unsigned(b) << std::endl;
      } else {
        to_rs_flag = 0;
      }
      if (to_rs_flag) {
        rs_get_out <= 1;
        to_rs_wire_i <= i[head];
      }
    }
  }
  if (!to_rs_flag) {
    rs_get_out <= 0;
  }
  if (error) {
    rob_error <= 1;
    for (int i = 1; i < ROB_SIZE; i++) {
      if (head_twice && i == head)
        continue;
      busy[i] <= 0;
    }
    to_rs <= 1;
    pos <= 0;
    return;
  }
  rob_error <= 0;
  if (pos_shift) {
    pos <= head;
  }
  if (rob_get_in) {
    int ii = to_unsigned(from_rs_wire_dest) % ROB_SIZE;
    if (ii == pos) {
      to_rs <= 0;
      twice = 1;
    }
    busy[ii] <= 1;
    a[ii] <= from_rs_wire_a;
    i[ii] <= from_rs_wire_i;
    op[ii] <= from_rs_wire_op;
    pc[ii] <= from_rs_wire_pc;
    rs1[ii] <= from_rs_wire_rs1;
    rs2[ii] <= from_rs_wire_rs2;
    time[ii] <= from_rs_wire_time;
  }
  if (!twice) {
    if (busy[to_unsigned(pos)]) {
      to_rs <= 0;
    } else {
      to_rs <= 1;
    }
  }
  for (int i = 1; i < ROB_SIZE; i++) {
    if (busy[i]) {
      if (time[i] != 0) {
        time[i] <= time[i] - 1;
      }
    }
  }

  if (memory.from_rs && memory.pc < memory.mem.size()) {
    memory.pc <= memory.pc + 4;
  }
}