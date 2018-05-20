// Implementation of a safe array type dbl_vect.

namespace dbl_vect {

class DoubleVector {
public:
  explicit DoubleVector(int n = 10);
  DoubleVector(const DoubleVector &v);
  ~DoubleVector() { delete[] p_; }
  double &element(int i);
  int ub() const { return (size_ - 1); }
  void Print() const;
  double SumElements() const;
  void Add(const DoubleVector &);
  double DotProduct(const DoubleVector &v) const;
  double Amplitude() const;
  void Scale(const double scale);
  void Rotate(const double angle, const DoubleVector &axis);

private:
  double *p_;
  int size_;
};

DoubleVector SumVectors(const DoubleVector &, const DoubleVector &);
}
