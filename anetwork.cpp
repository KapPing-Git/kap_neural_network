#include "anetwork.h"

using namespace std;

ANetwork::ANetwork(double learning_rate, uint batch_size) : m_learning_rate(learning_rate), m_batch_size(batch_size)
{

}

void ANetwork::fit(QVector<AFeatures> &samples_train, QVector<uint> y_train,
                   QVector<AFeatures> &samples_test,  QVector<uint> y_test, double target_accuracy)
{
  ATrain_step_statistic statistic;
  const uint mini_loss_size = 10000.0;
  double sum_loss {0};
  double sum_loss_mini {0};
  while(m_curr_accuracy < target_accuracy)
    {
      static uint i = 0;
      double loss {0};
      for (long long i = 0; i < samples_train.size(); ++i)
        {
          loss = train_step(samples_train[i],y_train[i]);
          sum_loss_mini += loss;
          sum_loss += loss;
          if (i % mini_loss_size == 0)
            {
//              cout << sum_loss_mini / double(mini_loss_size) << endl;
              statistic.descriptions = to_string(sum_loss_mini / double(mini_loss_size));
              //отправляем статистику получателю
              if (m_train_statistic_resiever)
                m_train_statistic_resiever(statistic);

              sum_loss_mini = 0;
            }
        }
      m_curr_accuracy = calk_accuracy(samples_test,y_test);


      ostringstream val_stream;

      //формируем строку статистики
      val_stream << setw(10) << left
           << "num step=" << setw(15) << left << i
           << "  mean_loss=" << setw(15) << left << sum_loss / samples_train.size()
           << "  accuracy=" << setw(15) << left << m_curr_accuracy;
      statistic.descriptions = val_stream.str();

      //отправляем статистику получателю
      if (m_train_statistic_resiever)
        m_train_statistic_resiever(statistic);

      sum_loss = 0;
      ++i;
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

void ANetwork::set_statistic_resiever(std::function<void (ATrain_step_statistic)> train_statistic_resiever)
{
  m_train_statistic_resiever = train_statistic_resiever;
}

// шаг обучения возвращает значение функции потерь на данном шаге, так же формирует его среднее значение за batch_size
double ANetwork::train_step(const AFeatures &X, uint y)
{
  // указываем признаки
  m_layers.front()->set_features(X);

  forward();

  // рассчитываем функцию потерь как softmax от логитов
  QVector<double> logits = m_layers.back()->logits();
  double loss = calk_loss(logits,y);

  // рассчитываем производную от функции потерь
  QVector<double> d_loss = calk_d_loss(logits,y);

  // устанавливаем полученные значения производных для каждого нейрона последнего слоя
  QVector<double *> d_y = m_layers.back()->dy();
  for (long long i = 0; i < d_loss.size(); ++i)
    {
      *d_y[i] = d_loss[i];
    }
  backward();

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
