#pragma once
#include "module.h"
#include "register.h"
#include "tools.h"
#include <array>
const int RS_SIZE = 1024;
struct RS_Input {
  Wire<1> rob_rs_get_in; // rs 获得了值设置为0
  Wire<1> rob_error;
  Wire<1> from_rob; // 1 表示 rob 有空缺
  Wire<32> from_rob_wire_i;
  Wire<32> from_rob_wire_value;
  Wire<32> from_memory_wire;
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
  dark::Register<32> to_rob_wire_time;
  dark::Register<1> rob_get_out; // rob 获得了值设置为0
};

struct RS_Private {
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
  std::array<dark::Register<32>, RS_SIZE> pc;
  std::array<dark::Register<32>, RS_SIZE> time;
  std::array<dark::Register<32>, 8> regs;
  std::array<dark::Register<32>, 8> reorder;
  std::array<dark::Register<1>, 8> reorder_busy;
};

struct RS : dark::Module<RS_Input, RS_Output, RS_Private> {
  void work() override final;
};