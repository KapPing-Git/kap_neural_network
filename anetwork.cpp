#include "anetwork.h"

using namespace std;

ANetwork::ANetwork(double learning_rate, uint batch_size) : m_learning_rate(learning_rate), m_batch_size(batch_size)
{

}

void ANetwork::fit(std::vector<AFeatures> &samples, std::vector<uint> y)
{
  m_batch_losses = {};

  for (ulong i = 0; i < samples.size(); ++i)
    {
      train_step(samples[i],y[i]);
    }
}

AAnswer ANetwork::predict(AFeatures X)
{
  AAnswer result;
  // устанавливаем признаки первому слою
  if (!m_layers.empty())
    {
      m_layers[0]->set_features(X);
    }

  forward();

  // результирующий класс определяем как класс с наибольшей вероятностью
  vector<double *> acsons = m_layers.back()->acsons(); //TODO возможно следует вероятности подвергнуть softmax
  double max_chance = 0;
  for (ulong i = 0; i < acsons.size(); ++i)
    {
      result.chances.push_back(*acsons[i]);
      if (*acsons[i] > max_chance)
        {
          max_chance = *acsons[i];
          result.class_num = i;
        }
    }
  return result;
}

void ANetwork::addLayer(std::unique_ptr<ALayer> layer)
{
  if(!m_layers.empty())
    {
      // связываем новый последний слой с предыдущим
      layer->setPreviosLayerAcsons(m_layers.back()->acsons(), m_layers.back()->dy());
    }
  layer->setLearning_rate(m_learning_rate);

  // добавляем слой
  m_layers.push_back(std::move(layer));
}

double ANetwork::mean_loss()
{
  return m_sum_batch_loss / m_batch_losses.size();
}

// шаг обучения возвращает значение функции потерь на данном шаге, так же формирует его среднее значение за batch_size
double ANetwork::train_step(AFeatures &X, uint y)
{
  // указываем признаки
  static uint num_step = 0;
//  if (num_step == 0)
  m_layers.front()->set_features(X);

  forward();

  // рассчитываем функцию потерь как softmax от логитов
  vector<double> logits = m_layers.back()->logits();
  double loss = calk_loss(logits,y);
  add_to_mean_batch_loss(loss);

  // рассчитываем производную от функции потерь
  vector<double> d_loss = calk_d_loss(logits,y);

  // устанавливаем полученные значения производных для каждого нейрона последнего слоя
  vector<double *> d_y = m_layers.back()->dy();
  for (ulong i = 0; i < d_loss.size(); ++i)
    {
      *d_y[i] = d_loss[i];
    }
  backward();

  //NOTE для отладки
//  static uint num_step = 0;
  cout << "num step=" << num_step << "   loss=" << loss << "   mean_loss=" << mean_loss() << endl;
  num_step++;

  return loss;
}

void ANetwork::forward()
{
  // запускаем работу всех нейронов
  for (ulong i = 0; i < m_layers.size(); ++i)
    {
      m_layers[i]->forward();
    }
}

void ANetwork::backward()
{
  for (int i = m_layers.size() - 1; i >= 0; --i)
    {
      m_layers[i]->backward();
    }
}

void ANetwork::add_to_mean_batch_loss(double loss)
{
  m_batch_losses.push(loss);
  m_sum_batch_loss += loss;
  if (m_batch_losses.size() > m_batch_size)
    {
      m_sum_batch_loss -= m_batch_losses.front();
      m_batch_losses.pop();
    }
}

double ANetwork::calk_loss(std::vector<double> logits, uint y)
{
  long double sum_logits = 0;
  for (double logit : logits)
    {
      sum_logits += exp(logit);
    }
  return log(sum_logits) - logits[y];
}

std::vector<double> ANetwork::calk_d_loss(std::vector<double> logits, uint y)
{
  std::vector<double> soft_max;
  long double sum_exp_logits = 0;
  for (auto logit : logits)
    {
      sum_exp_logits += exp(logit);
    }
  for (auto logit : logits)
    {
      soft_max.push_back(exp(logit) / sum_exp_logits);
    }
  soft_max[y] = soft_max[y] - 1;
  return soft_max;
}
