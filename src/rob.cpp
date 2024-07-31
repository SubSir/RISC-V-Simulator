#include "rob.hpp"
#include "bit.h"
#include "opcode.hpp"
#include "tools.h"
#include <iostream>

void RoB::work() {
  // terms++;
  // // // std::cout << "ROB Term is" << terms << std::endl;
  // if (terms == 8) {
  //   // // std::cout << "ROB Term is" << terms << std::endl;
  // }
  bool twice = 0, error = 0, head_twice = 0;
  int head = pos % ROB_SIZE;
  int to_rs_flag = 0, to_memory_flag = 0;
  std::cout << std::hex;
  if (busy[head]) {
    if (time[head] == 0) {
      int value = -1;
      busy[head] = 0;
      head_twice = 1;
      pos++;
      to_rs_flag = 1;
      std::cout << "PC = " << pc[head] << '\n';
      if (op[head] == ADD) {
        to_rs_wire_value <= (rs1[head] + rs2[head]);
        // std::cout << "ADD " << to_unsigned(rs1[head]) << " + "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SUB) {
        to_rs_wire_value <= (rs1[head] - rs2[head]);
        // std::cout << "SUB " << to_unsigned(rs1[head]) << " - "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == AND) {
        to_rs_wire_value <= (rs1[head] & rs2[head]);
        // std::cout << "AND " << to_unsigned(rs1[head]) << " & "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == OR) {
        to_rs_wire_value <= (rs1[head] | rs2[head]);
        // std::cout << "OR " << to_unsigned(rs1[head]) << " | "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == XOR) {
        to_rs_wire_value <= (rs1[head] ^ rs2[head]);
        // std::cout << "XOR " << to_unsigned(rs1[head]) << " ^ "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLL) {
        int shift = rs2[head];
        int d = rs1[head];
        int b = d << shift;
        if (shift > 31) {
          b = 0;
        }
        to_rs_wire_value <= b;
        // std::cout << "SLL " << to_unsigned(rs1[head]) << " << "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SRA) {
        int shift = rs2[head];
        int d = rs1[head];
        int b = d >> shift;
        if (shift > 31) {
          if (d < 0) {
            b = 0xffffffff;
          } else {
            b = 0;
          }
        }
        to_rs_wire_value <= b;
        // std::cout << "SRA " << to_unsigned(rs1[head]) << " >> "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SRL) {
        int shift = rs2[head];
        unsigned int d = rs1[head];
        int b = d >> shift;
        if (shift > 31) {
          b = 0;
        }
        to_rs_wire_value <= b;
        // std::cout << "SRL " << to_unsigned(rs1[head]) << " >> "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLT) {
        to_rs_wire_value <= (rs1[head] < rs2[head]);
        // std::cout << "SLT " << to_unsigned(rs1[head]) << " < "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == SLTU) {
        to_rs_wire_value <= ((unsigned)rs1[head] < (unsigned)rs2[head]);
        // std::cout << "SLTU " << to_unsigned(rs1[head]) << " < "
        //<< to_unsigned(rs2[head]) << std::endl;
      } else if (op[head] == ADDI) {
        to_rs_wire_value <= (rs1[head] + a[head]);
        // std::cout << "ADDI " << to_unsigned(rs1[head]) << " + "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == ANDI) {
        to_rs_wire_value <= (rs1[head] & a[head]);
        // std::cout << "ANDI " << to_unsigned(rs1[head]) << " & "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == ORI) {
        to_rs_wire_value <= (rs1[head] | a[head]);
        // std::cout << "ORI " << to_unsigned(rs1[head]) << " | "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == XORI) {
        to_rs_wire_value <= (rs1[head] ^ a[head]);
        // std::cout << "XORI " << to_unsigned(rs1[head]) << " ^ "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLLI) {
        int shift = a[head];
        int c = rs1[head];
        int b = c << shift;
        if (shift > 31) {
          b = 0;
        }
        to_rs_wire_value <= b;
        // std::cout << "SLLI " << to_unsigned(rs1[head]) << " << "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SRLI) {
        int shift = a[head];
        unsigned int c = rs1[head];
        int b = c >> shift;
        if (shift > 31) {
          b = 0;
        }
        to_rs_wire_value <= b;
        // std::cout << "SRLI " << to_unsigned(rs1[head]) << " >> "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SRAI) {
        int shift = a[head];
        int c = rs1[head];
        int b = c >> shift;
        if (shift > 31) {
          if (c < 0) {
            b = 0xffffffff;
          } else {
            b = 0;
          }
        }
        to_rs_wire_value <= b;
        // std::cout << "SRAI " << to_unsigned(rs1[head]) << " >> "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLTI) {
        to_rs_wire_value <= (rs1[head] < a[head]);
        // std::cout << "SLTI " << to_unsigned(rs1[head]) << " < "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == SLTIU) {
        to_rs_wire_value <= ((unsigned)rs1[head] < (unsigned)a[head]);
        // std::cout << "SLTIU " << to_unsigned(rs1[head]) << " < "
        // << to_unsigned(a[head]) << std::endl;
      } else if (op[head] == LB) {
        Bit b = memory.read_byte(rs1[head] + a[head]);
        to_rs_wire_value <= to_signed(b);
        // std::cout << "LB " << to_unsigned(rs1[head]) << " + "
        // <<  to_signed(a[head]) << std::endl;
      } else if (op[head] == LBU) {
        Bit b = memory.read_byte(rs1[head] + a[head]);
        to_rs_wire_value <= to_unsigned(b);
        // std::cout << "LBU " << to_unsigned(rs1[head]) << " + "
        // <<  to_signed(a[head]) << std::endl;
      } else if (op[head] == LH) {
        Bit b = memory.read_half_word(rs1[head] + a[head]);

        to_rs_wire_value <= to_signed(b);
        // std::cout << "LH " << to_unsigned(rs1[head]) << " + "
        // <<  to_signed(a[head]) << std::endl;
      } else if (op[head] == LHU) {
        Bit b = memory.read_half_word(rs1[head] + a[head]);

        to_rs_wire_value <= to_unsigned(b);
        // std::cout << "LHU " << to_unsigned(rs1[head]) << " + "
        // <<  to_signed(a[head]) << std::endl;
      } else if (op[head] == LW) {
        Bit b = memory.read_a_word(rs1[head] + a[head]);

        to_rs_wire_value <= to_signed(b);
        // std::cout << "LW " << to_unsigned(rs1[head]) << " + "
        // << to_signed(a[head]) << std::endl;
      } else if (op[head] == SB) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_byte(rs1[head] + a[head], b.range<7, 0>());
        // std::cout << "SB " << to_unsigned(rs1[head]) << " + "
        // << to_signed(a[head]) << " : " << to_unsigned(rs2[head]) <<
        // std::endl;
      } else if (op[head] == SH) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_half_word(rs1[head] + a[head], b.range<15, 0>());
        // std::cout << "SH " << to_unsigned(rs1[head]) << " + "
        // << to_signed(a[head]) << " : " << to_unsigned(rs2[head]) <<
        // std::endl;
      } else if (op[head] == SW) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_a_word(rs1[head] + a[head], b);
        // std::cout << "SW " << to_unsigned(rs1[head]) << " + "
        // << to_signed(a[head]) << " : " << to_unsigned(rs2[head]) <<
        // std::endl;
      } else if (op[head] == BEQ) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        // std::cout << "BEQ " << to_unsigned(rs1[head])
        // << " == " << to_unsigned(rs2[head]) << std::endl;
        if (rs1[head] == rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == BGE) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        // std::cout << "BGE " << to_signed(rs1[head])
        // << " >= " << to_signed(rs2[head]) << std::endl;
        if (rs1[head] >= rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == BGEU) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        // std::cout << "BGEU " << to_unsigned(rs1[head])
        // << " >= " << to_unsigned(rs2[head]) << std::endl;
        if ((unsigned)rs1[head] >= (unsigned)rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == BLTU) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        // std::cout << "BLTU " << to_unsigned(rs1[head]) << " < "
        //<< to_unsigned(rs2[head]) << std::endl;
        if ((unsigned)rs1[head] < (unsigned)rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == BLT) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        // std::cout << "BLT " << to_signed(rs1[head]) << " < "
        // << to_signed(rs2[head]) << std::endl;
        if (rs1[head] < rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == BNE) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        to_rs_flag = 0;
        if (rs1[head] != rs2[head]) {
          if (jump[head] == 0) {
            error = 1;
          }
          to_memory_jump <= 1;
          to_memory_pc <= pc[head] + a[head];
          // std::cout << "Branch to " << to_unsigned(pc[head] + a[head])
          // << std::endl;
        } else {
          if (jump[head] == 1) {
            error = 1;
          }
          to_memory_jump <= 0;
          to_memory_pc <= pc[head] + 4;
        }
      } else if (op[head] == JAL) {
        to_rs_wire_value <= pc[head] + 4;
        // std::cout << "JAL " << to_unsigned(pc[head]) << " + "
        // << // to_signed(a[head]) << std::endl;
      } else if (op[head] == JALR) {
        to_memory_flag = 1;
        to_memory_predict <= pc[head];
        error = 1;
        to_memory_jump <= 1;
        to_memory_pc <= rs1[head] + a[head];
        to_rs_wire_value <= pc[head] + 4;
        // std::cout << "JALR " << to_unsigned(rs1[head]) << " + "
        // << // to_signed(a[head]) << std::endl;
      } else if (op[head] == AUIPC) {
        Bit<32> b = a[head];
        to_rs_wire_value <= pc[head] + b;
        // std::cout << "AUIPC " << to_unsigned(pc[head]) << " + " <<
        // to_signed(b)
        // << std::endl;
      } else if (op[head] == LUI) {
        Bit<32> b = a[head];
        to_rs_wire_value <= b;
        // std::cout << "LUI " << to_unsigned(b) << std::endl;
      } else {
        to_rs_flag = 0;
      }
      rs_get_out <= 1;
      to_rs_wire_i <= i[head];
      if (!to_rs_flag) {
        to_rs_flag = 1;
        update <= 0;
      } else {
        update <= 1;
      }
    }
  }
  if (!to_rs_flag) {
    rs_get_out <= 0;
  }
  if (!to_memory_flag) {
    to_memory <= 0;
  } else {
    to_memory <= 1;
  }
  if (error) {
    rob_error <= 1;
    for (int i = 0; i < ROB_SIZE; i++) {
      if (head_twice && i == head)
        continue;
      busy[i] = 0;
    }
    pos = 0;
    return;
  }
  rob_error <= 0;
  to_rs_pos <= pos;
  if (rob_get_in && rob_error != 1) {
    int ii = to_unsigned(from_rs_wire_dest) % ROB_SIZE;
    busy[ii] = 1;
    a[ii] = to_signed(from_rs_wire_a);
    i[ii] = to_signed(from_rs_wire_i);
    op[ii] = to_signed(from_rs_wire_op);
    pc[ii] = to_signed(from_rs_wire_pc);
    rs1[ii] = to_signed(from_rs_wire_rs1);
    rs2[ii] = to_signed(from_rs_wire_rs2);
    time[ii] = to_signed(from_rs_wire_time);
    jump[ii] = to_signed(from_rs_wire_jump);
  }
  for (int i = 0; i < ROB_SIZE; i++) {
    if (busy[i]) {
      if (time[i] != 0) {
        time[i] = time[i] - 1;
      }
    }
  }
}