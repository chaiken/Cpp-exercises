#include <vector>

namespace matrix {

enum transform { copy, transpose, negative, upper };

class Matrix {
public:
  Matrix(int d1, int d2);
  Matrix(int d1, int d2, ::std::vector<double> inputs);
  Matrix(const Matrix &a, transform t);
  // square sub-matrix constructor
  Matrix(const Matrix &a, ::std::vector<int> rows, ::std::vector<int> cols);
  ~Matrix();
  int ub1() const { return (size1_ - 1); }
  int ub2() const { return (size2_ - 1); }
  double &Element(int i, int j) const;

private:
  double **p_;
  int size1_, size2_;
};

double Determinant(const Matrix &a, double sum);
void PrintMatrix(const Matrix &a);
}
