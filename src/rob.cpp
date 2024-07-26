#include "rob.hpp"
#include "bit.h"
#include "opcode.hpp"
#include "tools.h"

void RoB::work() {
  if (rob_get) {
    int ii = to_unsigned(from_rs_wire_dest) % ROB_SIZE;
    busy[ii] <= 1;
    a[ii] <= from_rs_wire_a;
    i[ii] <= from_rs_wire_i;
    op[ii] <= from_rs_wire_op;
    a[ii] <= from_rs_wire_a;
    pc[ii] <= from_rs_wire_pc;
    i[ii] <= from_rs_wire_i;
    rs1[ii] <= from_rs_wire_rs1;
    rs2[ii] <= from_rs_wire_rs2;
    time[ii] <= from_rs_wire_time;
    if (ii == pos) {
      to_rs = []() { return 0; };
    }
    rob_get = []() { return 0; };
  }
  for (int i = 0; i < ROB_SIZE; i++) {
    if (busy[i]) {
      if (time[i] != 0) {
        time[i] <= time[i] - 1;
      }
    }
  }
  int head = (to_unsigned(pos) + 1) % ROB_SIZE;
  if (busy[head] && rs_get == 0) {
    if (time[head] == 0) {
      int value = -1;
      pos <= head;
      int to_rs_flag = 1;
      if (op[head] == ADD) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] + rs2[head]);
        };
      } else if (op[head] == SUB) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] - rs2[head]);
        };
      } else if (op[head] == AND) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] & rs2[head]);
        };
      } else if (op[head] == OR) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] | rs2[head]);
        };
      } else if (op[head] == XOR) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] ^ rs2[head]);
        };
      } else if (op[head] == SLL) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] << rs2[head]);
        };
      } else if (op[head] == SRA) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] >> rs2[head]);
        };
      } else if (op[head] == SRL) {
        to_rs_wire_value = [this, head]() {
          return to_unsigned(rs1[head] >> rs2[head]);
        };
      } else if (op[head] == SLT) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head]) < to_signed(rs2[head]);
        };
      } else if (op[head] == SLTU) {
        to_rs_wire_value = [this, head]() {
          return to_unsigned(rs1[head]) < to_unsigned(rs2[head]);
        };
      } else if (op[head] == ADDI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] + a[head]);
        };
      } else if (op[head] == ANDI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] & a[head]);
        };
      } else if (op[head] == ORI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] | a[head]);
        };
      } else if (op[head] == XORI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] ^ a[head]);
        };
      } else if (op[head] == SLLI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] << a[head]);
        };
      } else if (op[head] == SRLI) {
        to_rs_wire_value = [this, head]() {
          return to_unsigned(rs1[head] >> a[head]);
        };
      } else if (op[head] == SRAI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head] >> a[head]);
        };
      } else if (op[head] == SLTI) {
        to_rs_wire_value = [this, head]() {
          return to_signed(rs1[head]) < to_signed(a[head]);
        };
      } else if (op[head] == SLTIU) {
        to_rs_wire_value = [this, head]() {
          return to_unsigned(rs1[head]) < to_unsigned(a[head]);
        };
      } else if (op[head] == LB) {
        Bit b = memory.read_byte(to_unsigned(rs1[head] + to_signed(a[head])));
        to_rs_wire_value = [b]() { return to_signed(b); };
      } else if (op[head] == LBU) {
        Bit b = memory.read_byte(to_unsigned(rs1[head] + to_signed(a[head])));
        to_rs_wire_value = [b]() { return to_unsigned(b); };
      } else if (op[head] == LH) {
        Bit b =
            memory.read_half_word(to_unsigned(rs1[head] + to_signed(a[head])));

        to_rs_wire_value = [b]() { return to_signed(b); };
      } else if (op[head] == LHU) {
        Bit b =
            memory.read_half_word(to_unsigned(rs1[head] + to_signed(a[head])));

        to_rs_wire_value = [b]() { return to_unsigned(b); };
      } else if (op[head] == LW) {
        Bit b = memory.read_a_word(to_unsigned(rs1[head] + to_signed(a[head])));

        to_rs_wire_value = [b]() { return to_signed(b); };
      } else if (op[head] == SB) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_byte(to_unsigned(rs1[head] + to_signed(a[head])),
                          b.range<7, 0>());
      } else if (op[head] == SH) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_half_word(to_unsigned(rs1[head] + to_signed(a[head])),
                               b.range<15, 0>());
      } else if (op[head] == SW) {
        to_rs_flag = 0;
        Bit<32> b = rs2[head];
        memory.store_a_word(to_unsigned(rs1[head] + to_signed(a[head])), b);
      } else if (op[head] == BEQ) {
        to_rs_flag = 0;
        if (rs1[head] == rs2[head]) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == BGE) {
        to_rs_flag = 0;
        if (to_signed(rs1[head]) >= to_signed(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == BGEU) {
        to_rs_flag = 0;
        if (to_unsigned(rs1[head]) >= to_unsigned(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == BLTU) {
        to_rs_flag = 0;
        if (to_unsigned(rs1[head]) < to_unsigned(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == BLT) {
        to_rs_flag = 0;
        if (to_signed(rs1[head]) < to_signed(rs2[head])) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == BNE) {
        to_rs_flag = 0;
        if (rs1[head] != rs2[head]) {
          memory.pc <= pc[head] + a[head];
          rob_error = []() { return 1; };
        }
      } else if (op[head] == JAL) {
        to_rs_wire_value = [this]() { return to_unsigned(memory.pc) + 4; };
        memory.pc <= pc[head] + a[head];
      } else if (op[head] == JALR) {
        to_rs_wire_value = [this]() { return to_unsigned(memory.pc) + 4; };
        memory.pc <= rs1[head] + a[head];
      } else if (op[head] == AUIPC) {
        Bit<32> aa = a[head];
        Bit<12> c = 0;
        Bit<32> b = {aa.range<19, 0>(), c};
        to_rs_wire_value = [this, b]() { return to_unsigned(memory.pc) + b; };
      } else if (op[head] == LUI) {
        Bit<32> aa = a[head];
        Bit<12> c = 0;
        Bit<32> b = {aa.range<19, 0>(), c};
        to_rs_wire_value = [b]() { return b; };
      }
      if (to_rs_flag) {
        rs_get = []() { return 1; };
        to_rs_wire_i = [this, head]() { return +i[head]; };
      }
    }
  }
}