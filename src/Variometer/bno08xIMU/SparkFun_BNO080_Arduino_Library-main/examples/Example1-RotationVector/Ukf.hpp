
#ifndef UKF_HPP
#define UKF_HPP

#include "Matrix.hpp"
#include "MerweScaledSigmaPoints.hpp"

template <uint32_t DIM_X, uint32_t DIM_Z>
class UnscentedKalmanFilter {
public:
  using StateVector = Matrix<DIM_X, 1>;
  using MeasurementVector = Matrix<DIM_Z, 1>;
  using ProcessStateCovarianceMatrix = Matrix<DIM_X, DIM_X>;
  using ProcessNoiseMatrix = Matrix<DIM_X, DIM_X>;
  using MeasurementCovarianceMatrix = Matrix<DIM_Z, DIM_Z>;
  using KalmanGain = Matrix<DIM_X, DIM_Z>;
  using SystemUncertainty = Matrix<DIM_Z, DIM_Z>;
  using StateSigmaPoints = typename MerweScaledSigmaPoints<DIM_X>::SigmaPoints;
  using MeasSigmaPoints = Matrix<DIM_Z, MerweScaledSigmaPoints<DIM_X>::NUM_OF_SIGMAS>;

  using StateTransitionFunction = const StateVector (*)(const StateVector &, double);
  using MeasurementFunction = MeasurementVector (*)(const StateVector &);

  StateVector x;
  ProcessStateCovarianceMatrix P;
  StateVector x_prior;
  ProcessStateCovarianceMatrix P_prior;
  ProcessNoiseMatrix Q;
  MeasurementCovarianceMatrix R;
  KalmanGain K;
  SystemUncertainty S;
  StateSigmaPoints f_sigmas;
  MeasSigmaPoints h_sigmas;

  StateTransitionFunction fx;
  MeasurementFunction hx;

  MerweScaledSigmaPoints<DIM_X> &merwesigmas;

  UnscentedKalmanFilter(StateTransitionFunction fx,
                        MeasurementFunction hx,
                        MerweScaledSigmaPoints<DIM_X> &sigmas) :
      fx(fx), hx(hx), merwesigmas{sigmas}
  {
  }

  void predict(double dt);

  void update(const MeasurementVector &z);

  template <typename State, typename Covariance, typename Sigmas, typename Noise>
  void transform(State &state, Covariance &covariance, Sigmas &sigmapoints, Noise &noise);
};

template <uint32_t DIM_X, uint32_t DIM_Z>
void UnscentedKalmanFilter<DIM_X, DIM_Z>::predict(double dt)
{
  // Generate sigma points
  auto sigmapoints = merwesigmas.generateSigmas(x, P);

  // Compute the prior using the state transition function
  for (size_t i = 0; i < f_sigmas.cols(); ++i) {
    f_sigmas.col(i) = fx(sigmapoints.col(i), dt);
  }

  // Do the unscented transformation to calculate prior.
  transform(x_prior, P_prior, f_sigmas, Q);
}

template <uint32_t DIM_X, uint32_t DIM_Z>
void UnscentedKalmanFilter<DIM_X, DIM_Z>::update(const MeasurementVector &z)
{
  // Calculate measurement sigmas by passing prior sigmas through the
  // measurement function
  for (size_t i = 0; i < h_sigmas.cols(); ++i) {
    h_sigmas.col(i) = hx(f_sigmas.col(i));
  }

  // Calculate the mean and covariance of these points using unscented
  // transformation
  MeasurementVector z_mean{};
  transform(z_mean, S, h_sigmas, R);

  // Compute cross variance of state x and measurement z.
  Matrix<DIM_X, DIM_Z> Pxz{};
  for (size_t i = 0; i < f_sigmas.cols(); ++i) {
    auto dx = f_sigmas.col(i) - x_prior;
    auto dz = h_sigmas.col(i) - z_mean;
    auto outer = dx * dz.transpose();
    auto Pxzi = outer * merwesigmas.Wc[i][0];
    Pxz = Pxz + Pxzi;
  }

  // Calculate Kalman gain and residul
  K = Pxz * S.inv();
  auto y = z - z_mean;

  x = x_prior + (K * y);
  P = P_prior - (K * (S * K.transpose()));
}

template <uint32_t DIM_X, uint32_t DIM_Z>
template <typename State, typename Covariance, typename Sigmas, typename Noise>
void UnscentedKalmanFilter<DIM_X, DIM_Z>::transform(State &state,
                                                    Covariance &covariance,
                                                    Sigmas &sigmapoints,
                                                    Noise &noise)
{
  // The mean is the dot product of the sigma points and the weight.
  for (size_t i = 0; i < sigmapoints.rows(); ++i) {
    state[i][0] = sigmapoints.row(i) * merwesigmas.Wm;
  }

  // Calculate the state covariance matrix
  covariance.fill(0.0);
  for (size_t i = 0; i < sigmapoints.cols(); ++i) {
    auto y = sigmapoints.col(i) - state;
    auto outer = y * y.transpose();
    auto Pi = outer * merwesigmas.Wc[i][0];
    covariance = covariance + Pi;
  }

  // Add noise
  covariance = covariance + noise;
}

#endif
