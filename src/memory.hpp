#pragma once
#include "../include/tools.h"
#include "module.h"
#include "register.h"
#include "synchronize.h"
#include <fstream>
const int MEM_SIZE = 0x11000;
struct Memory_Input {
  Wire<1> from_rs;      // rs 有剩余
  Wire<1> from_rob;     // rob 返回转移指令
  Wire<32> from_rob_pc; // rob 给出的pc
  Wire<32> from_rob_predict;
  Wire<1> from_rob_jump; // 是否跳转
};

struct Memory_Output {
  dark::Register<32> pc;
  dark::Register<32> pc_past;
  Register<1> rs_get_out;
  Register<32> to_rs_op;
  Register<32> to_rs_rd;
  Register<32> to_rs_rs1;
  Register<32> to_rs_rs2;
  Register<32> to_rs_a;
  Register<1> to_rs_jump;
};
struct Memory_Private {
  int mem[MEM_SIZE] = {};
  bool jump[MEM_SIZE] = {};
  int predict[MEM_SIZE] = {};
};

struct Memory : dark::Module<Memory_Input, Memory_Output> {
  int mem[MEM_SIZE] = {};
  bool jump[MEM_SIZE] = {};
  int predict[MEM_SIZE] = {};
  void initialize();
  void work() override final;
  Bit<32> read_a_word(int address);
  Bit<16> read_half_word(int address);
  Bit<8> read_byte(int address);
  void store_a_word(int address, Bit<32> value);
  void store_half_word(int address, Bit<16> value);
  void store_byte(int address, Bit<8> value);
};