#pragma once
#include "../include/tools.h"
#include "register.h"
#include <fstream>
struct Memory {
  Wire<1> from_rs; // rs 有剩余
  Wire<1> rs_get;  // rs 获得了值设置为0
  Wire<32> to_rs_wire;
  Wire<32> pc_wire;
  dark::Register<32> pc;
  dark::Register<32> reg;
  std::array<dark::Register<32>, 2 * 4096> ins;
  std::array<dark::Register<32>, 2 * 4096> mem;
  void initialize(std::fstream &file);
  void work();
};