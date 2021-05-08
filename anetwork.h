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

struct ATrain_step_statistic
{
  std::string descriptions;
};

class ANetwork
{
public:
  ANetwork(double learning_rate, uint batch_size);
  void fit(QVector<AFeatures> &samples_train, QVector<uint> y_train,
           QVector<AFeatures> &samples_test,  QVector<uint> y_test, double target_accuracy);
  AAnswer predict(AFeatures X);
  void addLayer(const std::shared_ptr<ALayer> &layer);
  void set_statistic_resiever(std::function<void(ATrain_step_statistic)> train_statistic_resiever);
  friend QDataStream &operator<<(QDataStream &stream, const ANetwork &network);
  friend QDataStream &operator>>(QDataStream &stream, ANetwork &network);

private:
  QVector<std::shared_ptr<ALayer>> m_layers;
  double m_learning_rate;
  uint m_batch_size;
  double m_sum_batch_loss {0};
  double m_curr_accuracy {0};
  std::function<void(ATrain_step_statistic)> m_train_statistic_resiever;

  double train_step(const AFeatures &X, uint y);
  void forward();
  void backward();
  double calk_loss(QVector<double> logits, uint y);
  QVector<double> calk_d_loss(QVector<double> logits, uint y);
  double calk_accuracy(QVector<AFeatures> &samples, QVector<uint> y);
};

QDataStream &operator<<(QDataStream &stream, const ANetwork &network);
QDataStream &operator>>(QDataStream &stream, ANetwork &network);

#endif // ANETWORK_H
