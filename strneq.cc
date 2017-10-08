#include <cerrno>
#include <cstring>
#include <iostream>

using namespace std;

bool strneq(const char *s1, const char *s2, int n) {
  if (((n < strlen(s1)) && (n > strlen(s2))) ||
      ((n > strlen(s2)) && (n < strlen(s1)))) {
    return false;
  }

  int i = 0;
  while (i++ < n) {
    if (*s1++ != *s2++)
      return false;
  }
  return true;
}

// Find the first occurrence of 'c' after the cursor position in the input.
size_t string_next(const string &str1, int cursor, char c) {
  string str2 = str1.substr(cursor, str1.length());
  return (str2.find(c));
}

int main(int argc, char **argv) {
  cout << "Enter two strings, separated by a comma, followed by an integer, or "
          "'.' to terminate:"
       << endl;
  int i = 0;
  size_t pos = 0, start = 0;
  string inputstr, str[3];
  do {
    getline(cin, inputstr);
    if ((inputstr.length()) && (inputstr != ".")) {
      while ((pos != ::std::string::npos) && (i < 3)) {
        pos = string_next(inputstr, start, ',');
        str[i] = inputstr.substr(start, pos);
        // Advance the position where the next search starts after the comma.
        start += pos + 1;
        i++;
      }
    } else {
      exit(0);
    }
    if ((i < 3) && (pos == ::std::string::npos)) {
      cerr << "Insufficient input" << endl;
      exit(-EINVAL);
    } else {
      for (i = 0; i < 2; i++)
        cout << "str[" << i << "] is " << str[i] << endl;
    }

    int charnum = strtol(str[2].c_str(), nullptr, 10);
    if (charnum) {
      bool ans = strneq(str[0].c_str(), str[1].c_str(), charnum);
      cout << ((ans) ? "true" : "false") << endl << endl;
    } else {
      cerr << str[2] << " cannot be converted to an integer." << endl;
      exit(-EINVAL);
    }
  } while (1);
}
