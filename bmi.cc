#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

constexpr double inches_per_meter = 39.37;
constexpr double lbs_per_kg = 2.205;

const string category[3]{
    "normal", "overweight", "obese",
};

double inches_to_m(double length) {
  assert(length > 0.0);
  return (length / inches_per_meter);
}

double lbs_to_kg(double weight) {
  assert(weight > 0.0);
  return (weight / lbs_per_kg);
}

double calc_bmi(double height, double weight) {
  assert(height > 0);
  return (weight / (height * height));
}

int main(int argc, char **argv) {
  double height, weight;
  string name, type;

  if (argc != 4) {
    cout << "Enter name, height, weight." << endl;
    exit(EXIT_FAILURE);
  }
  name = argv[1];
  if ((sscanf(argv[2], "%lf", &height) != 1) ||
      (sscanf(argv[3], "%lf", &weight) != 1)) {
    cout << "Enter name, height (inches), weight (lbs)." << endl;
    exit(EXIT_FAILURE);
  }
  double bmi = calc_bmi(inches_to_m(height), lbs_to_kg(weight));
  if (bmi <= 25)
    type = category[0];
  else if (bmi >= 40)
    type = category[2];
  else
    type = category[1];
  cout << "Name: " << name << " Weight: " << weight << " Height: " << height
       << " BMI: " << bmi << " Category: " << type << endl;

  exit(EXIT_SUCCESS);
}
