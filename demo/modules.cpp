#include "../include/tools.h"
#include <iostream>

struct wb {
  Wire<5> wb_index;  // Writeback
  Wire<1> wb_enable; // Writeback enabled?
  Wire<32> wb_data;  // Data to writeback
};

struct RegFile_Input {
  Wire<5> rs1_index; // Read
  Wire<5> rs2_index; // Read
  wb wb;
};

struct regout {
  Register<32> rs1_data; // Read
  Register<32> rs2_data; // Read
};
struct RegFile_Output {
  regout reg;
  Wire<1> rs1_valid = []() { return 1; };
};

struct RegFile_Private {
  std::array<Register<32>, 32> regs;
};

struct RegFile : dark::Module<RegFile_Input, RegFile_Output, RegFile_Private> {
  void work() override final {
    std::cout << "wb_data is " << to_unsigned(wb.wb_data) << std::endl;
    // reg.rs1_data <= regs[to_unsigned(rs1_index)];
    // reg.rs2_data <= regs[to_unsigned(rs2_index)];

    // if (wb.wb_enable && wb.wb_index != 0) {
    //   regs[to_unsigned(wb.wb_index)] <= wb.wb_data;
    // }
  }
};

struct InsDecode_Input {
  Wire<32> rs1_data;
  Wire<32> rs2_data;
};

struct InsDecode_Output {
  Wire<1> rs1_valid;
  Register<5> rs1_index;
  Register<5> rs2_index;
  Register<5> wb_index;
  Register<32> wb_data;
  Register<1> wb_enable;
};

struct InsDecode : dark::Module<InsDecode_Input, InsDecode_Output> {
  void work() override final {
    wb_data <= wb_data + 1;
    std::cout << "wb_data is " << to_unsigned(wb_data) << std::endl;
    //   if (rs1_valid) {
    //     std::cout << "rs1_valid " << std::endl;
    //   }
    //   char c;
    //   max_size_t x;
    //   max_size_t y;
    //   std::cin >> c >> x >> y;
    //   if (c == 'r') {
    //     rs1_index <= x;
    //     rs2_index <= y;
    //     wb_index <= 0;
    //     wb_data <= 0;
    //     wb_enable <= 0;
    //   } else {
    //     rs1_index <= 0;
    //     rs2_index <= 0;
    //     wb_index <= x;
    //     wb_data <= y;
    //     wb_enable <= 1;
    //   }

    //   std::cout << "rs1_data: " << to_unsigned(rs1_data) << std::endl;
    //   std::cout << "rs2_data: " << to_unsigned(rs2_data) << std::endl;
  }
};

signed main() {
  InsDecode ins_decode;
  RegFile reg_file;

  dark::CPU cpu;

  cpu.add_module(&ins_decode);
  cpu.add_module(&reg_file);
  reg_file.rs1_valid = [&]() -> auto & { return ins_decode.rs1_valid; };
  ins_decode.rs1_valid = [&]() -> auto & { return reg_file.rs1_valid; };

  reg_file.rs1_index = [&]() -> auto & { return ins_decode.rs1_index; };
  reg_file.rs2_index = [&]() -> auto & { return ins_decode.rs2_index; };
  reg_file.wb.wb_index = [&]() -> auto & { return ins_decode.wb_index; };
  reg_file.wb.wb_enable = [&]() -> auto & { return ins_decode.wb_enable; };
  reg_file.wb.wb_data = [&]() -> auto & { return ins_decode.wb_data; };

  ins_decode.rs1_data = [&]() -> auto & { return reg_file.reg.rs1_data; };
  ins_decode.rs2_data = [&]() -> auto & { return reg_file.reg.rs2_data; };

  cpu.run(114514, true);

  // Demo input:
  // w 1 2	(output 0 0)
  // r 1 2	(output 0 0)
  // w 2 3	(output 0 0)
  // r 1 2	(output 2 0)
  // r 1 2	(output 0 0)
  // r 1 2	(output 2 3)

  return 0;
}
