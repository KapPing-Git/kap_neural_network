#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_network.reset(new ANetwork(0.01,100));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(100,m_ReLU)));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(200,m_ReLU)));
  m_network->addLayer(shared_ptr<ALayer>(new ALayer(10,m_none)));

 ADataset train = read_mnist_from_csv("/home/kap/source_code/II/mnist/mnist_train.csv");
 ADataset test = read_mnist_from_csv("/home/kap/source_code/II/mnist/mnist_test.csv");
 m_network->fit(train.features,train.num_class,test.features,test.num_class);

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
//          cout << value << endl;
        }
      result.features.push_back(features);
    }
  return result;
}

