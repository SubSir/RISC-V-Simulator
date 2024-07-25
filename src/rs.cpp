#include "rs.hpp"
void RS::work() {
  int tmp = -1;
  if (from_memory) {
    from_memory = [&]() { return 0; };
    for (int i = 0; i < RS_SIZE; i++) {
      if (busy[i] == 0) {
        busy[i] <= 1;
        tmp = i;
        // 没读指令
        break;
      }
    }
  }
  bool flag = 0;

  for (int i = 0; i < RS_SIZE; i++) {
    if (busy[i] == 0 and i != tmp) {
      flag = 1;
      break;
    }
  }
  to_memory = [flag]() { return flag; };
}