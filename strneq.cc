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

// Find the first occurrence of 'c' after the cursor position in the input
// and return its offset from the start of the input string.
size_t string_next(const string &str1, int cursor, char c) {
  // The two arguments to substr() are start char and number of chars.
  //      basic_string substr (size_type __pos=0, size_type __n=npos) const
  //        Get a substring.
  string str2 = str1.substr(cursor, (str1.length() - cursor));
  return (str2.find(c) + cursor);
}

int main(int argc, char **argv) {
  cout << "Enter two strings, separated by a comma, followed by an integer, or "
          "'.' to terminate:"
       << endl;
  string inputstr, str[3];
  do {
    int i = 0;
    size_t pos = 0, start = 0;
    getline(cin, inputstr);
    if ((inputstr.length()) && (inputstr != ".")) {
      while ((pos != ::std::string::npos) && (i < 3)) {
        pos = string_next(inputstr, start, ',');
        if ((i < 2) && (pos == ::std::string::npos)) {
          cerr << "Insufficient input" << endl;
          exit(-EINVAL);
        }
        // Take the characters between the start and the comma.
        str[i] = inputstr.substr(start, pos - start);
        cout << "str[" << i << "] is " << str[i] << endl;
        // Advance the position after the i'th comma.
        start = pos + 1;
        i++;
      }
    } else {
      exit(0);
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
