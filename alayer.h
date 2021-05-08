#ifndef ALAYER_H
#define ALAYER_H

//#include <vector>
#include<QVector>

#include "aneuron.h"
#include "aactivationfunction.h"

using uint = unsigned int;
class ALayer
{
public:
  ALayer(ulong neuron_counts, AAbstructActivationFunction &activationFunction, QVector<double*> dendrits = {},
         QVector<double*> dx = {} );
  QVector<double *> acsons();
  QVector<double> logits();
  QVector<double *> dy();
  void setPreviosLayerAcsons(QVector<double*> acsons, QVector<double*> dX = {});
  void set_features(const AFeatures &features);
  void forward();
  void backward();
  void setLearning_rate(double learning_rate);
  friend QDataStream &operator<<(QDataStream &stream, const ALayer &layer);
  friend QDataStream &operator>>(QDataStream &stream, ALayer &layer);

private:
  QVector<ANeuron> m_neurons;
};

QDataStream &operator<<(QDataStream &stream, const ALayer &layer);
QDataStream &operator>>(QDataStream &stream, ALayer &layer);

#endif // ALAYER_H
