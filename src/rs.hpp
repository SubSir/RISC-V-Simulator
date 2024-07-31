#pragma once
#include "module.h"
#include "register.h"
#include "tools.h"
#include <array>
const int RS_SIZE = 128;
struct RS_Input {
  Wire<1> rob_rs_get_in; // rs 获得了值设置为0
  Wire<1> rob_error;
  Wire<32> from_rob_pos;
  Wire<32> from_rob_wire_i;
  Wire<32> from_rob_wire_value;
  Wire<32> from_memory_op;
  Wire<32> from_memory_rd;
  Wire<32> from_memory_rs1;
  Wire<32> from_memory_rs2;
  Wire<32> from_memory_a;
  Wire<1> from_memory_jump;
  Wire<1> from_rob_wire_update;
  Wire<32> pc_wire;
  Wire<1> rs_get_in; // rs 获得了值设置为0
};

struct RS_Output {
  dark::Register<1> to_memory; // 初始化为 1
  dark::Register<32> to_rob_wire_op;
  dark::Register<32> to_rob_wire_rs1;
  dark::Register<32> to_rob_wire_rs2;
  dark::Register<32> to_rob_wire_dest;
  dark::Register<32> to_rob_wire_a;
  dark::Register<32> to_rob_wire_pc;
  dark::Register<32> to_rob_wire_i;
  dark::Register<1> to_rob_wire_ready;
  dark::Register<1> to_rob_wire_jump;
  dark::Register<1> rob_get_out; // rob 获得了值设置为0
  dark::Register<1> to_lsb_in;
  dark::Register<32> to_lsb_wire_op;
  dark::Register<32> to_lsb_wire_rs1;
  dark::Register<32> to_lsb_wire_rs2;
  dark::Register<32> to_lsb_wire_dest;
  dark::Register<32> to_lsb_wire_a;
  dark::Register<32> to_lsb_wire_pos;
};

struct RS_Private {
  Register<32> pos;
  std::array<dark::Register<1>, RS_SIZE> busy;
  std::array<dark::Register<1>, RS_SIZE> commited;
  std::array<dark::Register<32>, RS_SIZE> op;
  std::array<dark::Register<32>, RS_SIZE> rd;
  std::array<dark::Register<32>, RS_SIZE> vj;
  std::array<dark::Register<32>, RS_SIZE> qj;
  std::array<dark::Register<32>, RS_SIZE> vk;
  std::array<dark::Register<32>, RS_SIZE> qk;
  std::array<dark::Register<32>, RS_SIZE> dest;
  std::array<dark::Register<32>, RS_SIZE> a;
  std::array<dark::Register<32>, RS_SIZE> lsb_poses;
  std::array<dark::Register<1>, RS_SIZE> jump;
  std::array<dark::Register<32>, RS_SIZE> pc;
  std::array<dark::Register<1>, RS_SIZE> ready;
  std::array<dark::Register<32>, 32> regs;
  std::array<dark::Register<32>, 32> reorder;
  std::array<dark::Register<1>, 32> reorder_busy;
};

struct RS : dark::Module<RS_Input, RS_Output, RS_Private> {
  int lsb_pos = 0;
  void work() override final;
  void print();
};