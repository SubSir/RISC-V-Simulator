#include "lsb.hpp"
#include "bit.h"
#include "opcode.hpp"
#include "rob.hpp"
#include "tools.h"
#include <iostream>

void LSB::work() {
  if (from_rob_in) {
    pos = to_unsigned(from_rob_wire_dest);
  }
  if (rob_error) {
    while (busy[head]) {
      if (pos >= dest[head]) {
        busy[head] = 0;
        if (op[head] == SB) {
          memory.store_byte(address[head], value[head]);
        } else if (op[head] == SH) {
          memory.store_half_word(address[head], value[head]);
        } else {
          memory.store_a_word(address[head], value[head]);
        }
      }
      head = (head + 1) % LSB_SIZE;
    }
    head = 0;
    pos = -1;
    for (int i = 0; i < LSB_SIZE; i++) {
      busy[i] = 0;
    }
    rob_get_out <= 0;
    return;
  }
  int rob_get_out_flag = 0;
  if (busy[head]) {
    if (ticker == 3) {
      ticker = 0;
      if (op[head] == LB || op[head] == LH || op[head] == LW ||
          op[head] == LBU || op[head] == LHU) {
        busy[head] = 0;
        rob_get_out_flag = 1;
        rob_get_out <= 1;
        to_rob_dest <= dest[head];
        if (op[head] == LB)
          to_rob_wire_value <= to_signed(memory.read_byte(address[head]));
        else if (op[head] == LH)
          to_rob_wire_value <= to_signed(memory.read_half_word(address[head]));
        else if (op[head] == LW)
          to_rob_wire_value <= memory.read_a_word(address[head]);
        else if (op[head] == LBU)
          to_rob_wire_value <= to_unsigned(memory.read_byte(address[head]));
        else if (op[head] == LHU)
          to_rob_wire_value <=
              to_unsigned(memory.read_half_word(address[head]));
        head++;
      } else {
        if (pos >= dest[head]) {
          busy[head] = 0;
          if (op[head] == SB) {
            memory.store_byte(address[head], value[head]);
          } else if (op[head] == SH) {
            memory.store_half_word(address[head], value[head]);
          } else {
            memory.store_a_word(address[head], value[head]);
          }
          head++;
        }
      }
    } else {
      ticker++;
    }
  }
  if (!rob_get_out_flag) {
    rob_get_out <= 0;
  }
  head %= LSB_SIZE;
  if (from_rs_in) {
    int ops = to_unsigned(from_rs_wire_op);
    int i = to_unsigned(from_rs_wire_pos) % LSB_SIZE;
    busy[i] = 1;
    op[i] = ops;
    dest[i] = to_unsigned(from_rs_wire_dest);
    if (ops == SB) {
      address[i] = to_unsigned(from_rs_wire_rs1 + from_rs_wire_a);
      value[i] = from_rs_wire_rs2;
    } else if (ops == SH) {
      address[i] = to_unsigned(from_rs_wire_rs1 + from_rs_wire_a);
      value[i] = from_rs_wire_rs2;
    } else if (ops == SW) {
      address[i] = to_unsigned(from_rs_wire_rs1 + from_rs_wire_a);
      value[i] = from_rs_wire_rs2;
    } else if (ops == LB || ops == LH || ops == LW || ops == LBU ||
               ops == LHU) {
      address[i] = to_unsigned(from_rs_wire_rs1 + from_rs_wire_a);
    }
  }
}