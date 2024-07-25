#pragma once
#include "memory.hpp"
#include "register.h"
#include "tools.h"
#include <array>
const int RS_SIZE = 32;
struct RS {
  Wire<1> to_memory;
  Wire<1> from_memory;
  Wire<32> from_memory_wire;
  std::array<dark::Register<1>, RS_SIZE> busy;
  std::array<dark::Register<32>, RS_SIZE> instruction;
  std::array<dark::Register<32>, RS_SIZE> vi;
  std::array<dark::Register<32>, RS_SIZE> qi;
  std::array<dark::Register<32>, RS_SIZE> vj;
  std::array<dark::Register<32>, RS_SIZE> qj;
  std::array<dark::Register<32>, RS_SIZE> dest;
  std::array<dark::Register<32>, RS_SIZE> a;
  void work();
};