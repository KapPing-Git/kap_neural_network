#ifndef ANEURON_H
#define ANEURON_H

#include <vector>
#include <random>
#include <chrono>

#include "network_main.h"
#include "aactivationfunction.h"

using uint = unsigned int;

class ANeuron
{
public:
  ANeuron( AAbstructActivationFunction &activationFunction, std::vector<double*> dendrits = {}, std::vector<double*> dx = {});
  double m_y {0};
  double m_dy {0};
  double m_logit {0};
  void set_dendrits(std::vector<double*> dendrits, std::vector<double *> dX = {});
  void set_features(AFeatures &features);
  void forward();
  void backward();
  void setLearning_rate(double learning_rate);

private:
  AAbstructActivationFunction &m_activationFunction;
  double m_learning_rate;
  AFeatures m_fx;
  std::vector<double*> m_x;
  std::vector<double*> m_dx;
  std::vector<double> m_w;
  double m_b {0};


  void create_random_weights();
};

#endif // ANEURON_H
