#include <cstdlib>
#include <iostream>

using namespace std;

namespace {
// 25 g of American Cheese has 375 calories.
constexpr double kCaloriesPerGram = 15;
}

class DairyProduct {
public:
  unsigned GetCalories();
  void Initialize(string name, unsigned weight, unsigned calories,
                  unsigned protein, unsigned fat, unsigned carbohydrates);
  void show(void) {
    cout << weight_ << " g of " << name_ << " has " << GetCalories()
         << " calories." << endl;
  }

private:
  string name_;
  // All in grams.
  unsigned weight_, calories_, protein_, fat_, carbohydrates_;
};

unsigned DairyProduct::GetCalories() { return weight_ * kCaloriesPerGram; }

void DairyProduct::Initialize(string name, unsigned weight, unsigned calories,
                              unsigned protein, unsigned fat,
                              unsigned carbohydrates) {
  name_ = name;
  weight_ = weight;
  calories_ = calories;
  protein_ = protein;
  fat_ = fat;
  carbohydrates_ = carbohydrates;
}

int main(void) {
  DairyProduct MyCheese;
  MyCheese.Initialize("American Cheese", 25, 5, 8, 0, 9);
  MyCheese.show();
  DairyProduct MySandwich;
  MySandwich.Initialize("Cheese Sandwich", 150, 2, 1, 3, 4);
  MySandwich.show();
  exit(EXIT_SUCCESS);
}
