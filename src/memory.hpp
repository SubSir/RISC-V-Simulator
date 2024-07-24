#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "../include/tools.h"
#include <fstream>
struct Memory {
  dark::Register<32> pc;
  dark::Register<32> reg;
  Wire<32> mem[2 * 4096];
  void initialize(std::fstream &file);
};
#endif