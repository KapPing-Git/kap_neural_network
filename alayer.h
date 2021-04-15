#ifndef ALAYER_H
#define ALAYER_H

#include <vector>

#include "aneuron.h"
#include "aactivationfunction.h"

using uint = unsigned int;
class ALayer
{
public:
  ALayer(ulong neuron_counts, AAbstructActivationFunction &activationFunction, std::vector<double*> dendrits = {},
         std::vector<double*> dx = {} );
  std::vector<double *> acsons();
  std::vector<double> logits();
  std::vector<double *> dy();
  void setPreviosLayerAcsons(std::vector<double*> acsons, std::vector<double*> dX = {});
  void set_features(AFeatures &features);
  void forward();
  void backward();
  void setLearning_rate(double learning_rate);

private:
  std::vector<ANeuron> m_neurons;
};

#endif // ALAYER_H
