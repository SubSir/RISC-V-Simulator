#include "memory.hpp"
#include <bitset>
#include <sstream>
#include <string>

void Memory::initialize(std::fstream &file) {
  while (!file.eof()) {
    int pointer = 0;
    std::string line;
    max_size_t value[2 * 4096];
    for (int i = 0; i < 2 * 4096; i++) {
      mem[i] = [&]() { return value[i]; };
    }
    file >> line;
    if (line[0] == '@') {
      pointer = std::stoi(line.substr(1, line.length() - 1));
    } else {
      std::bitset<16> bits(line);
      for (int i = 0; i < 16; i++) {
        value[pointer++] = bits[i];
      }
    }
  }
}