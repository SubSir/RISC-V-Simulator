#pragma once
#include "../include/tools.h"
#include "register.h"
#include <fstream>
struct Memory {
  Wire<1> from_rs;
  Wire<1> send_to_rs;
  Wire<1> rs_get;
  Wire<32> to_rs_wire;
  dark::Register<32> pc;
  dark::Register<32> reg;
  std::array<dark::Register<32>, 2 * 4096> ins;
  std::array<dark::Register<32>, 2 * 4096> mem;
  void initialize(std::fstream &file);
  void work();
};