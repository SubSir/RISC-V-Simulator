#pragma once
#include "memory.hpp"
#include "module.h"
#include "register.h"
#include "tools.h"
#include "wire.h"

const int LSB_SIZE = 128;
struct LSB_Input {
  Wire<1> rob_error;
  Wire<1> from_rob_in;
  Wire<32> from_rob_wire_dest;
  Wire<1> from_rs_in;
  Wire<32> from_rs_wire_op;
  Wire<32> from_rs_wire_rs1;
  Wire<32> from_rs_wire_rs2;
  Wire<32> from_rs_wire_dest;
  Wire<32> from_rs_wire_a;
  Wire<32> from_rs_wire_pos;
};

struct LSB_Output {
  dark::Register<1> rob_get_out;
  dark::Register<32> to_rob_dest;
  dark::Register<32> to_rob_wire_value;
};

struct LSB_Private {
  Memory memory;
};
struct LSB : dark::Module<LSB_Input, LSB_Output, LSB_Private> {
  // Wire<1> memory_get; // memory 获得了值设置为0
  // Wire<32> to_memory_wire_op;
  // Wire<32> to_memory_wire_address;
  // Wire<32> to_memory_wire_value;
  int pos = -1;
  int head = {};
  int ticker = {};
  bool busy[LSB_SIZE] = {};
  int dest[LSB_SIZE] = {};
  int op[LSB_SIZE] = {};
  int address[LSB_SIZE] = {};
  Bit<32> value[LSB_SIZE] = {};
  using LSB_Private::memory;
  void work() override final;
};
