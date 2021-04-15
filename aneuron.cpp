#include "aneuron.h"

using namespace std;
using namespace std::chrono;

ANeuron::ANeuron(AAbstructActivationFunction &activationFunction, std::vector<double *> dendrits, std::vector<double*> dx)
  : m_activationFunction(activationFunction), m_x(dendrits), m_dx(dx)
{
  create_random_weights();
}

void ANeuron::set_dendrits(std::vector<double *> dendrits, std::vector<double *> dX)
{
  m_x = dendrits;
  m_dx = dX;
  m_fx = {}; // если это не первый слой то используются m_x
  create_random_weights();
}

void ANeuron::set_features(AFeatures &features)
{
  m_fx = features;
  m_x = {}; // если это первый слой то используются m_fx
  create_random_weights();
}

void ANeuron::forward()
{
  m_logit = 0;
  // если это нейрон первого слоя
  if (!m_fx.empty())
    {
      for (ulong i = 0; i < m_fx.size(); ++i)
        {
          m_logit += m_fx[i] * m_w[i];
        }
    }
  else if (!m_x.empty())
    {
      for (ulong i = 0; i < m_x.size(); ++i)
        {
          m_logit += *m_x[i] * m_w[i];
        }
    }

  m_logit += m_b;
  m_y = m_activationFunction.forward(m_logit);
  m_dy = 0;
}

void ANeuron::backward()
{
  double dy = m_dy * m_activationFunction.backward(m_logit) * m_learning_rate;
  m_b -= dy;
  // если это нейрон первого слоя
  if (!m_fx.empty())
    {
      for (ulong i = 0; i < m_x.size(); ++i)
        {
          m_w[i] -= m_fx[i] * dy;
        }
    }
  else if (!m_x.empty())
    {
      for (ulong i = 0; i < m_x.size(); ++i)
        {
          m_w[i] -= *m_x[i] * dy;
        }
    }
  // если это нейрон первого слоя то m_dx не существует
  if (!m_dx.empty())
    {
      for (ulong i = 0; i < m_x.size(); ++i)
        {
          *m_dx[i] += m_w[i] * dy;
        }
    }

}

void ANeuron::setLearning_rate(double learning_rate)
{
  m_learning_rate = learning_rate;
}

void ANeuron::create_random_weights()
{
  //создаём массив весов со случайными числами
  default_random_engine dre(static_cast<ulong>(system_clock::now().time_since_epoch().count()));
  uniform_real_distribution<double> rd(-0.001,0.001);
  ulong size = 0;
  if (!m_fx.empty())
    size = m_fx.size();
  else if (!m_x.empty())
    size = m_x.size();
  for(ulong i = 0; i < size; ++i)
    m_w.push_back(rd(dre));
}
