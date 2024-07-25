#include "memory.hpp"
#include "tools.h"
#include <sstream>
#include <string>

void Memory::initialize(std::fstream &file) {
  while (!file.eof()) {
    int pointer = 0;
    std::string line;
    file >> line;
    if (line[0] == '@') {
      pointer = std::stoi(line.substr(1, line.length() - 1)) / 32;
    } else {
      std::stringstream ss;
      ss << std::hex << line;
      unsigned int bit;
      ss >> bit;
      file >> line;
      ss << std::hex << line;
      unsigned int bit2;
      ss >> bit2;
      bit += bit2 << 8;
      file >> line;
      ss << std::hex << line;
      ss >> bit2;
      bit += bit2 << 16;
      file >> line;
      ss << std::hex << line;
      ss >> bit2;
      bit += bit2 << 24;
      ins[pointer++] <= bit;
    }
  }
}

void Memory::work() {
  if (rs_get == 0) {
    if (from_rs) {
      if (pc * 4 < ins.size()) {
        pc <= (pc + 1) * 4;
        pc_wire = [this]() { return to_unsigned(pc) + 4; };
        to_rs_wire = [this]() { return +ins[to_unsigned(pc) + 4]; };
        rs_get = []() { return 1; };
        return;
      }
    }
  }
}