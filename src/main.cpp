#include "../include/tools.h"
#include <fstream>
#include <iostream>

using namespace std;
int main() {
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