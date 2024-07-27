#include "../include/tools.h"
#include "cpu.h"
#include "register.h"
#include "rob.hpp"
#include "rs.hpp"
#include <fstream>
#include <iostream>

using namespace std;
int main() {
  dark::CPU cpu;
  RS rs;
  RoB rob;
  cpu.add_module(&rs);
  cpu.add_module(&rob);
  fstream f("input.txt");
  if (!f.is_open()) {
    cout << "File not found!";
    return 0;
  }
  rob.memory.initialize(f);
  rob.memory.from_rs = [&]() -> auto & { return rs.to_memory; };
  rs.from_memory_wire = [&]() -> auto & { return rob.memory.to_rs_wire; };
  rs.pc_wire = [&]() -> auto & { return rob.memory.pc_past; };
  rs.from_rob = [&]() -> auto & { return rob.to_rs; };
  rob.rob_get_in = [&]() -> auto & { return rs.rob_get_out; };
  rs.rob_error = [&]() -> auto & { return rob.rob_error; };
  rs.from_rob_wire_i = [&]() -> auto & { return rob.to_rs_wire_i; };
  rs.from_rob_wire_value = [&]() -> auto & { return rob.to_rs_wire_value; };
  rs.rs_get_in = [&]() -> auto & { return rob.memory.rs_get_out; };
  rs.rob_rs_get_in = [&]() -> auto & { return rob.rs_get_out; };
  rob.from_rs_wire_a = [&]() -> auto & { return rs.to_rob_wire_a; };
  rob.from_rs_wire_dest = [&]() -> auto & { return rs.to_rob_wire_dest; };
  rob.from_rs_wire_i = [&]() -> auto & { return rs.to_rob_wire_i; };
  rob.from_rs_wire_op = [&]() -> auto & { return rs.to_rob_wire_op; };
  rob.from_rs_wire_pc = [&]() -> auto & { return rs.to_rob_wire_pc; };
  rob.from_rs_wire_rs1 = [&]() -> auto & { return rs.to_rob_wire_rs1; };
  rob.from_rs_wire_rs2 = [&]() -> auto & { return rs.to_rob_wire_rs2; };
  rob.from_rs_wire_time = [&]() -> auto & { return rs.to_rob_wire_time; };
  cpu.run(20000, true);
}