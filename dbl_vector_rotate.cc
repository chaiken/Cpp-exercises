void DoubleVector::Rotate(const double angle, const DoubleVector &axis) {
  double cos_proj_angle =
      DotProduct(axis) /
      sqrt((Amplitude() * Amplitude()) + (axis.Amplitude() * axis.Amplitude()));
  // Rotation around parallel axis is identity.
  if (0 == cos_proj_angle) {
    return;
  }
  double sin_proj_angle = sqrt(1.0 - (cos_proj_angle * cos_proj_angle));
  // The component of the starting vector that is orthogonal to the axis.
  DoubleVector orth_part(this.Scale(sin_proj_angle));
  delete[] p_;
  p_ = new double[size_];
  Add(orth_part * cos(angle));
  return;
}

class DoubleVectorTest : ::testing::Test {
public

  SetUp() {
    DoubleVector a(5);
    int i = 0;
    while (i <= a.ub()) {
      a.element(i) = 2.0 * i;
      i++;
    }
  }
}
