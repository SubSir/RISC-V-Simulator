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
  if (pc * 4 < ins.size()) {
    Bit<32> op = ins[to_unsigned(pc)];
    Bit<7> mem = op.range<6, 0>();

    pc_wire = [this]() { return to_unsigned(pc) + 4; };
    if ((mem == 0b0000011 || mem == 0b01000011) && ldb_get == 0 && from_ldb) {
      send_to_ldb = [&]() { return 1; };
      to_ldb_wire = [this]() { return +ins[to_unsigned(pc)]; };
      pc <= (pc + 1) * 4;
      return;
    } else if (rs_get == 0 && from_rs) {
      send_to_rs = [&]() { return 1; };
      to_rs_wire = [this]() { return +ins[to_unsigned(pc)]; };
      pc <= (pc + 1) * 4;
      return;
    }
  }
  send_to_rs = [&]() { return 0; };
}