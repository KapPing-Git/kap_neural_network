#include "alayer.h"

ALayer::ALayer(ulong neuron_counts, AAbstructActivationFunction &activationFunction, std::vector<double*> dendrits,
               std::vector<double*> dx)
{
  for (ulong i = 0; i < neuron_counts; ++i)
    {
      m_neurons.push_back(ANeuron(activationFunction, dendrits, dx));
    }
}

std::vector<double *> ALayer::acsons()
{
  std::vector<double *> result;
  result.reserve(m_neurons.size());
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(&m_neurons[i].m_y);
//      result[i] = &m_neurons[i].m_y;
    }
  return result;
}

std::vector<double> ALayer::logits()
{
  std::vector<double> result;
  result.reserve(m_neurons.size());
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(m_neurons[i].m_logit);
//      result[i] = m_neurons[i].m_logit;
    }
  return result;
}

std::vector<double *> ALayer::dy()
{
  std::vector<double *> result;
  result.reserve(m_neurons.size());
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      result.push_back(&m_neurons[i].m_dy);
//      result[i] = &m_neurons[i].m_dy;
    }
  return result;
}

// создаём полносвязный слой
void ALayer::setPreviosLayerAcsons(std::vector<double *> acsons, std::vector<double *> dX)
{
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].set_dendrits(acsons,dX);
    }
}

void ALayer::set_features(AFeatures &features)
{
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].set_features(features);
    }
}

void ALayer::forward()
{
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].forward();
    }
}

void ALayer::backward()
{
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].backward();
    }
}

void ALayer::setLearning_rate(double learning_rate)
{
  for (ulong i = 0; i < m_neurons.size(); ++i)
    {
      m_neurons[i].setLearning_rate(learning_rate);
    }
}
