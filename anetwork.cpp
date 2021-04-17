#include "anetwork.h"

using namespace std;

ANetwork::ANetwork(double learning_rate, uint batch_size) : m_learning_rate(learning_rate), m_batch_size(batch_size)
{

}

void ANetwork::fit(QVector<AFeatures> &samples_train, QVector<uint> y_train,
                   QVector<AFeatures> &samples_test,  QVector<uint> y_test)
{
  m_batch_losses = {};

  for (long long i = 0; i < samples_train.size(); ++i)
    {
//      train_step(samples[i],y[i]);
      uint z = 0;//i % m_batch_size;

      double loss {0};
      for (z = i; z < i + m_batch_size; ++z)
        {
          loss = train_step(samples_train[z],y_train[z]);
        }
      if (i % 100 == 0)
        m_curr_accuracy = calk_accuracy(samples_test,y_test);
      //NOTE для отладки
      cout << setw(10) << left
           << "num step=" << setw(10) << left << i
           << "  loss=" << setw(10) << left << loss
           << "  mean_loss=" << setw(10) << left << mean_loss()
           << "  accuracy=" << setw(10) << left << m_curr_accuracy
           << endl;
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
  QVector<double *> acsons = m_layers.back()->acsons(); //TODO возможно следует вероятности подвергнуть softmax
  double max_chance = 0;
  for (long long i = 0; i < acsons.size(); ++i)
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

void ANetwork::addLayer(const std::shared_ptr<ALayer> &layer)
{
  if(!m_layers.empty())
    {
      // связываем новый последний слой с предыдущим
      layer->setPreviosLayerAcsons(m_layers.back()->acsons(), m_layers.back()->dy());
    }
  layer->setLearning_rate(m_learning_rate);

  // добавляем слой
  m_layers.push_back(layer);
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
  m_layers.front()->set_features(X);

  forward();

  // рассчитываем функцию потерь как softmax от логитов
  QVector<double> logits = m_layers.back()->logits();
  double loss = calk_loss(logits,y);
  add_to_mean_batch_loss(loss);

  // рассчитываем производную от функции потерь
  QVector<double> d_loss = calk_d_loss(logits,y);

  // устанавливаем полученные значения производных для каждого нейрона последнего слоя
  QVector<double *> d_y = m_layers.back()->dy();
  for (long long i = 0; i < d_loss.size(); ++i)
    {
      *d_y[i] = d_loss[i];
    }
  backward();

  //NOTE для отладки
//  static uint num_step = 0;
//  cout << setw(10) << left
//       << "num step=" << setw(10) << left << num_step
//       << "loss=" << setw(10) << left << loss
//       << "mean_loss=" << setw(10) << left << mean_loss()
//       << "accuracy=" << setw(10) << left << m_curr_accuracy
//       << endl;

  num_step++;

  return loss;
}

void ANetwork::forward()
{
  // запускаем работу всех нейронов
  for (long long i = 0; i < m_layers.size(); ++i)
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

double ANetwork::calk_loss(QVector<double> logits, uint y)
{
  long double sum_logits = 0;
  for (double logit : logits)
    {
      sum_logits += exp(logit);
    }
  return log(sum_logits) - logits[y];
}

QVector<double> ANetwork::calk_d_loss(QVector<double> logits, uint y)
{
  QVector<double> soft_max;
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

double ANetwork::calk_accuracy(QVector<AFeatures> &samples, QVector<uint> y)
{
  double true_count = 0;
  for (auto i = 0; i < samples.size(); ++i)
    {
      uint predicted_y = predict(samples[i]).class_num;
      true_count += predicted_y == y[i];
    }
  return true_count / double(samples.size());
}
