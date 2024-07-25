#pragma once
#include "memory.hpp"
#include "register.h"
#include "tools.h"
#include "wire.h"
#include <array>
const int RS_SIZE = 1024;
struct RS {
  Wire<1> to_memory;
  Wire<1> from_memory;
  Wire<32> from_memory_wire;
  Wire<32> pc_wire;
  std::array<dark::Register<1>, RS_SIZE> busy;
  std::array<dark::Register<32>, RS_SIZE> op;
  std::array<dark::Register<32>, RS_SIZE> vj;
  std::array<dark::Register<32>, RS_SIZE> qj;
  std::array<dark::Register<32>, RS_SIZE> vk;
  std::array<dark::Register<32>, RS_SIZE> qk;
  std::array<dark::Register<32>, RS_SIZE> dest;
  std::array<dark::Register<32>, RS_SIZE> a;
  std::array<dark::Register<32>, 8> regs;
  std::array<dark::Register<32>, 8> reorder;
  std::array<dark::Register<1>, 8> reorder_busy;
  void work();
};