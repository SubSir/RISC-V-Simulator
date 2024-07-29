#include "memory.hpp"
#include "tools.h"
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <string>

void Memory::initialize() {
  int pointer = 0;
  while (!std::cin.eof()) {
    std::string line;
    std::cin >> line;
    if (line[0] == '@') {
      pointer = std::stoi(line.substr(1, line.length() - 1), nullptr, 16);
    } else {
      std::stringstream ss;
      ss << std::hex << line;
      unsigned int bit;
      ss >> bit;
      mem[pointer++] <= bit;
    }
  }
}

void Memory::work() {
  pc_past <= pc;
  if (from_rs) {
    if (pc < mem.size()) {
      // std::cout << "        Memread : " << to_unsigned(pc) << std::endl;
      to_rs_wire <= read_a_word(to_unsigned(pc));
      rs_get_out <= 1;
      return;
    }
  }
  rs_get_out <= 0;
}

Bit<8> Memory::read_byte(int address) {
  if (address >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  return mem[address];
}

Bit<16> Memory::read_half_word(int address) {
  if (address + 1 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  Bit<8> a = mem[address + 1];
  Bit<8> b = mem[address];
  Bit c = {a, b};
  return c;
}

Bit<32> Memory::read_a_word(int address) {
  if (address + 3 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return 0;
  }
  Bit<8> a = mem[address + 3];
  Bit<8> b = mem[address + 2];
  Bit<8> c = mem[address + 1];
  Bit<8> d = mem[address];
  Bit e = {a, b, c, d};
  return e;
}

void Memory::store_byte(int address, Bit<8> value) {
  if (address >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value;
}

void Memory::store_half_word(int address, Bit<16> value) {
  if (address + 1 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value.range<7, 0>();
  mem[address + 1] <= value.range<15, 8>();
}

void Memory::store_a_word(int address, Bit<32> value) {
  if (address + 3 >= mem.size()) {
    std::cout << "Error: Memory address out of range" << std::endl;
    return;
  }
  mem[address] <= value.range<7, 0>();
  mem[address + 1] <= value.range<15, 8>();
  mem[address + 2] <= value.range<23, 16>();
  mem[address + 3] <= value.range<31, 24>();
}