#pragma once
#include "memory.hpp"
#include "module.h"
#include "register.h"
#include "tools.h"
#include "wire.h"

const int ROB_SIZE = 1024;
struct RoB_Input {
  Wire<1> rob_get_in; // rob 获得了值设置为0
  Wire<32> from_rs_wire_op;
  Wire<32> from_rs_wire_rs1;
  Wire<32> from_rs_wire_rs2;
  Wire<32> from_rs_wire_dest;
  Wire<32> from_rs_wire_a;
  Wire<32> from_rs_wire_pc;
  Wire<32> from_rs_wire_i;
  Wire<32> from_rs_wire_time;
};

struct RoB_Output {
  dark::Register<1> rs_get_out;
  dark::Register<1> rob_error;
  dark::Register<1> to_rs;
  dark::Register<32> to_rs_wire_i;
  dark::Register<32> to_rs_wire_value;
};

struct RoB_Private {
  dark::Register<32> pos;
  std::array<dark::Register<1>, ROB_SIZE> busy;
  std::array<dark::Register<32>, ROB_SIZE> i;
  std::array<dark::Register<32>, ROB_SIZE> op;
  std::array<dark::Register<32>, ROB_SIZE> pc;
  std::array<dark::Register<32>, ROB_SIZE> time;
  std::array<dark::Register<32>, ROB_SIZE> rs1;
  std::array<dark::Register<32>, ROB_SIZE> rs2;
  std::array<dark::Register<32>, ROB_SIZE> a;
  Memory memory;
};
struct RoB : dark::Module<RoB_Input, RoB_Output, RoB_Private> {
  // Wire<1> memory_get; // memory 获得了值设置为0
  // Wire<32> to_memory_wire_op;
  // Wire<32> to_memory_wire_address;
  // Wire<32> to_memory_wire_value;
  using RoB_Private::memory;
  void work() override final;
};
