

#ifndef MERWE_SCALED_SIGMA_POINTS_HPP
#define MERWE_SCALED_SIGMA_POINTS_HPP
#include "Matrix.hpp"

template <uint32_t DIM_X>
class MerweScaledSigmaPoints {
public:
  static constexpr uint32_t NUM_OF_SIGMAS{2 * DIM_X + 1};

  using SigmaPoints = Matrix<DIM_X, NUM_OF_SIGMAS>;
  using StateVector = Matrix<DIM_X, 1>;
  using CovarianceMatrix = Matrix<DIM_X, DIM_X>;

  MerweScaledSigmaPoints(double a, double b, double k) :
      alpha(a), beta(b), kappa(k), lambda(alpha * alpha * (DIM_X + kappa) - DIM_X)
  {
    computeWeights();
  }

  SigmaPoints generateSigmas(const StateVector &x, const CovarianceMatrix &P);

private:
  const double alpha;
  const double beta;
  const double kappa;
  const double lambda;

public:
  Matrix<NUM_OF_SIGMAS, 1> Wm;
  Matrix<NUM_OF_SIGMAS, 1> Wc;

  void computeWeights(void);
};

template <uint32_t DIM_X>
typename MerweScaledSigmaPoints<DIM_X>::SigmaPoints
MerweScaledSigmaPoints<DIM_X>::generateSigmas(const StateVector &x, const CovarianceMatrix &P)
{
  SigmaPoints sigmas{};

  auto U = ((lambda + DIM_X) * P).sqrt();

  // The first col is the mean
  for (size_t i = 0; i < DIM_X; ++i) {
    sigmas[i][0] = x[i][0];
  }

  for (size_t k = 0; k < DIM_X; ++k) {
    // Xi_1..n
    for (size_t i = 0; i < DIM_X; ++i) {
      sigmas[i][k + 1] = x[i][0] + U[i][k];
    }

    // Xi_n+1..2n
    for (size_t i = 0; i < DIM_X; ++i) {
      sigmas[i][DIM_X + k + 1] = x[i][0] - U[i][k];
    }
  }

  return sigmas;
}

template <uint32_t DIM_X>
void MerweScaledSigmaPoints<DIM_X>::computeWeights(void)
{
  // Initialize weight matrix
  double Wi = 0.5 / (DIM_X + lambda);
  Wm.fill(Wi);
  Wc.fill(Wi);

  // Set weight for the mean[0]
  Wm[0][0] = lambda / (DIM_X + lambda);

  // Set weight for the covariance[0]
  Wc[0][0] = (lambda / (DIM_X + lambda)) + (1 - (alpha * alpha) + beta);
}

#endif
