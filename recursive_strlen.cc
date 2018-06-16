#include <iostream>
#include <sstream>

using namespace std;

size_t recurlen(string str) {
  static size_t cnt = 0u;
  if (str.length()) {
    cnt++;
    string substr = str.substr(1, (str.length() - 1));
    recurlen(substr);
  }
  return cnt;
}

int main(int argc, char **argv) {
  string inputstr;

  cout << "Enter a string: ";
  getline(cin, inputstr);
  cout << "Length: " << recurlen(inputstr) << endl;

  exit(0);
}
