#ifndef ANETWORK_H
#define ANETWORK_H

//#include <vector>
#include <QVector>
#include <memory>
#include <queue>
#include <iostream>
#include <iomanip>

#include "alayer.h"

struct AAnswer
{
  uint class_num;
  QVector<double> chances;
};

class ANetwork
{
public:
  ANetwork(double learning_rate, uint batch_size);
  void fit(QVector<AFeatures> &samples_train, QVector<uint> y_train,
           QVector<AFeatures> &samples_test,  QVector<uint> y_test);
  AAnswer predict(AFeatures X);
  void addLayer(const std::shared_ptr<ALayer> &layer);
  double mean_loss();

private:
  QVector<std::shared_ptr<ALayer>> m_layers;
  double m_learning_rate;
  uint m_batch_size;
  double m_sum_batch_loss {0};
  std::queue<double> m_batch_losses;
  double m_curr_accuracy {0};

  double train_step(AFeatures &X, uint y);
  void forward();
  void backward();
  void add_to_mean_batch_loss(double loss);
  double calk_loss(QVector<double> logits, uint y);
  QVector<double> calk_d_loss(QVector<double> logits, uint y);
  double calk_accuracy(QVector<AFeatures> &samples, QVector<uint> y);
};

#endif // ANETWORK_H
