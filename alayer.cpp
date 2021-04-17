#include "alayer.h"

ALayer::ALayer(ulong neuron_counts, AAbstructActivationFunction &activationFunction, QVector<double*> dendrits,
               QVector<double*> dx)
{
  for (ulong i = 0; i < neuron_counts; ++i)
    {
      m_neurons.push_back(ANeuron(activationFunction, dendrits, dx));
    }
}

QVector<double *> ALayer::acsons()
{
  QVector<double *> result;
  result.reserve(m_neurons.size());
  for ( long long i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(&m_neurons[i].m_y);
//      result[i] = &m_neurons[i].m_y;
    }
  return result;
}

QVector<double> ALayer::logits()
{
  QVector<double> result;
  result.reserve(m_neurons.size());
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(m_neurons[i].m_logit);
//      result[i] = m_neurons[i].m_logit;
    }
  return result;
}

QVector<double *> ALayer::dy()
{
  QVector<double *> result;
  result.reserve(m_neurons.size());
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(&m_neurons[i].m_dy);
//      result[i] = &m_neurons[i].m_dy;
    }
  return result;
}

// создаём полносвязный слой
void ALayer::setPreviosLayerAcsons(QVector<double *> acsons, QVector<double *> dX)
{
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].set_dendrits(acsons,dX);
    }
}

void ALayer::set_features(AFeatures &features)
{
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].set_features(features);
    }
}

void ALayer::forward()
{
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].forward();
    }
}

void ALayer::backward()
{
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].backward();
    }
}

void ALayer::setLearning_rate(double learning_rate)
{
  for (long long i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].setLearning_rate(learning_rate);
    }
}
