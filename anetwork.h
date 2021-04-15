#ifndef ANETWORK_H
#define ANETWORK_H

#include <vector>
#include <memory>
#include <queue>
#include <iostream>

#include "alayer.h"

struct AAnswer
{
  uint class_num;
  std::vector<double> chances;
};

class ANetwork
{
public:
  ANetwork(double learning_rate, uint batch_size);
  void fit(std::vector<AFeatures> &samples, std::vector<uint> y);
  AAnswer predict(AFeatures X);
  void addLayer(std::unique_ptr<ALayer> layer);
  double mean_loss();

private:
  std::vector<std::unique_ptr<ALayer>> m_layers;
  double m_learning_rate;
  uint m_batch_size;
  double m_sum_batch_loss {0};
  std::queue<double> m_batch_losses;

  double train_step(AFeatures &X, uint y);
  void forward();
  void backward();
  void add_to_mean_batch_loss(double loss);
  double calk_loss(std::vector<double> logits, uint y);
  std::vector<double> calk_d_loss(std::vector<double> logits, uint y);
};

#endif // ANETWORK_H
