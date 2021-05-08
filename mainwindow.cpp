#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_network.reset(new ANetwork(0.005,100));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(100,m_ReLU)));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(200,m_ReLU)));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(10,m_none)));
  m_network->set_statistic_resiever(bind(&MainWindow::on_network_requested,this,_1));
}

MainWindow::~MainWindow()
{
  delete ui;
}

ADataset MainWindow::read_mnist_from_csv(string file_name)
{
  ADataset result;

  // загружаем данные mnist из csv файла
  ifstream train_file(file_name);
  string str;
  train_file >> str; // первая строчка это заголовок нам не нужна
  while(!train_file.eof())
    {
      train_file >> str;
      istringstream val_stream(str);
      string val;
      getline(val_stream,val,',');

      result.num_class.push_back(stoi(val));
      AFeatures features;
      while(!val_stream.eof())
        {
          getline(val_stream,val,',');
          features.push_back(stod(val)/255);
        }
      result.features.push_back(features);
    }
  return result;
}


void MainWindow::on_train_review_button_clicked()
{
  ui->train_edit->setText(QFileDialog::getOpenFileName());
}

void MainWindow::on_test_review_button_clicked()
{
  ui->test_edit->setText(QFileDialog::getOpenFileName());
}


void MainWindow::on_start_button_clicked()
{
   m_train = read_mnist_from_csv(ui->train_edit->text().toStdString());
   m_test = read_mnist_from_csv(ui->test_edit->text().toStdString());
   m_network->fit(m_train.features,m_train.num_class,m_test.features,m_test.num_class,ui->targetEdit->text().toDouble());
}

void MainWindow::on_network_requested(ATrain_step_statistic statistic)
{
  ui->statistic_edit->append(QString::fromStdString(statistic.descriptions));
  QApplication::processEvents();
}

void MainWindow::on_predictButton_clicked()
{
  AFeatures features = ui->pic_widget->pic_as_features();
  ui->predictedEdit->setText(QString::number(m_network->predict(features).class_num));
}

void MainWindow::on_pushButton_clicked()
{
  ui->pic_widget->clear();
}

void MainWindow::on_saveButton_clicked()
{
#ifdef Q_OS_ANDROID
  QFile file(":/network_data.net");
#else
  QFile file("network_data.net");
#endif
  if (file.open(QIODevice::WriteOnly))
    {
      QDataStream stream(&file);
      stream << *m_network.get();
    }
}

void MainWindow::on_loadButton_clicked()
{
  #ifdef Q_OS_ANDROID
  QFile file(":/network_data.net");
  #else
  QFile file("network_data.net");
  #endif
  if (file.open(QIODevice::ReadOnly))
    {
      QDataStream stream(&file);
      stream >> *m_network.get();
    }
}
