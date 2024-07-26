#include "../include/tools.h"
#include "register.h"
#include <fstream>
#include <iostream>

using namespace std;
int main() {
  Bit<2> a(0b11);
  Bit<3> b(0b00);
  Bit c = {a + 1, b};
  cout << to_signed(c);
  fstream f("input.txt");
  if (!f.is_open()) {
    cout << "File not found!";
    return 0;
  }
  while (!f.eof()) {
    string s;
    f >> s;
    if (s[0] == '@') {
    }
  }
}