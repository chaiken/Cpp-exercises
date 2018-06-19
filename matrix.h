#include <vector>

namespace matrix {

enum transform { copy, transpose, negative, upper };

class Matrix {
public:
  Matrix(int d1, int d2);
  Matrix(int d1, int d2, ::std::vector<double> inputs);
  Matrix(const Matrix &a, transform t);
  ~Matrix();
  int ub1() const { return (size1_ - 1); }
  int ub2() const { return (size2_ - 1); }
  double &Element(int i, int j) const;

private:
  double **p_;
  int size1_, size2_;
};
}
