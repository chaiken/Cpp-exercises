#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

float toFahrenheit(float Celsius) {
  return (Celsius * (9.0/5.0) + 32.0);
}

float toCelsius(float Fahrenheit) {
  return ((Fahrenheit - 32.0) * (5.0/9.0));
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Invoke with one numeric parameter." << endl;
    exit(EXIT_FAILURE);
  }
  string input(argv[1]);
  float temperature = strtof(argv[1], nullptr);
  if ((input.find("F") != string::npos)) {
    cout << "Fahrenheit: " << ceil(temperature) << "; Celsius: " << ceil(toCelsius(temperature)) << "C" << endl;
  } else if ((input.find("C") != string::npos)) {
    cout << "Celsius: " << ceil(temperature) << "; Fahrenheit: " << ceil(toFahrenheit(temperature)) << "F" << endl;
  } else {
    cout << "Illegal input." << endl;
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
