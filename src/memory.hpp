#pragma once
#include "../include/tools.h"
#include "register.h"
#include "synchronize.h"
#include <fstream>
struct Memory {
  Wire<1> from_rs; // rs 有剩余
  Register<1> rs_get_out;
  Register<32> to_rs_op;
  Register<32> to_rs_rd;
  Register<32> to_rs_rs1;
  Register<32> to_rs_rs2;
  Register<32> to_rs_a;
  dark::Register<32> pc;
  dark::Register<32> pc_past;
  dark::Register<32> reg;
  std::array<dark::Register<8>, 0x11000> mem;
  void initialize();
  void work();
  Bit<32> read_a_word(int address);
  Bit<16> read_half_word(int address);
  Bit<8> read_byte(int address);
  void store_a_word(int address, Bit<32> value);
  void store_half_word(int address, Bit<16> value);
  void store_byte(int address, Bit<8> value);
};