#ifndef ANEURON_H
#define ANEURON_H

//#include <vector>
#include <random>
#include <chrono>
#include <QVector>
#include <QDataStream>

#include "network_main.h"
#include "aactivationfunction.h"

using uint = unsigned int;

class ANeuron
{
public:
  ANeuron(AAbstructActivationFunction &activationFunction, const QVector<double*> &dendrits = {}, const QVector<double*> &dx = {});
  double m_y {0};
  double m_dy {0};
  double m_logit {0};
  void set_dendrits(QVector<double*> dendrits, QVector<double *> dX = {});
  void set_features(const AFeatures &features);
  void forward();
  void backward();
  void setLearning_rate(double learning_rate);

private:
  AAbstructActivationFunction *m_activationFunction;
  double m_learning_rate;
  AFeatures m_fx;
  QVector<double*> m_x;
  QVector<double*> m_dx;
  QVector<double> m_w;
  double m_b {0};
  void create_random_weights();
};

#endif // ANEURON_H
