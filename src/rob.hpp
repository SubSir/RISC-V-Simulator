#pragma once
#include "memory.hpp"
#include "module.h"
#include "register.h"
#include "tools.h"
#include "wire.h"

const int ROB_SIZE = 128;
struct RoB_Input {
  Wire<1> rob_get_in; // rob 获得了值设置为0
  Wire<32> from_rs_wire_op;
  Wire<32> from_rs_wire_rs1;
  Wire<32> from_rs_wire_rs2;
  Wire<32> from_rs_wire_dest;
  Wire<32> from_rs_wire_a;
  Wire<32> from_rs_wire_pc;
  Wire<32> from_rs_wire_i;
  Wire<1> from_rs_wire_ready;
  Wire<1> from_rs_wire_jump;
  Wire<1> lsb_get_in;
  Wire<32> from_lsb_dest;
  Wire<32> from_lsb_wire_value;
};

struct RoB_Output {
  dark::Register<1> rs_get_out;
  dark::Register<1> rob_error;
  dark::Register<32> to_rs_pos;
  dark::Register<32> to_rs_wire_i;
  dark::Register<32> to_rs_wire_value;
  dark::Register<1> update;
  dark::Register<1> to_memory;
  dark::Register<1> to_memory_jump;
  dark::Register<32> to_memory_pc;
  dark::Register<32> to_memory_predict;
  dark::Register<1> to_lsb;
  dark::Register<32> to_lsb_dest;
};

struct RoB : dark::Module<RoB_Input, RoB_Output> {
  // Wire<1> memory_get; // memory 获得了值设置为0
  // Wire<32> to_memory_wire_op;
  // Wire<32> to_memory_wire_address;
  // Wire<32> to_memory_wire_value;
  int pos = {};
  bool busy[ROB_SIZE] = {};
  int dest[ROB_SIZE] = {};
  int i[ROB_SIZE] = {};
  int op[ROB_SIZE] = {};
  int pc[ROB_SIZE] = {};
  bool ready[ROB_SIZE] = {};
  int rs1[ROB_SIZE] = {};
  int rs2[ROB_SIZE] = {};
  int a[ROB_SIZE] = {};
  bool jump[ROB_SIZE] = {};
  void work() override final;
};
