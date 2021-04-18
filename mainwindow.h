#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <QMainWindow>
#include <QFileDialog>

#include "anetwork.h"
#include "alayer.h"
#include "aactivationfunction.h"
#include "aneuron.h"
#include "network_main.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_train_review_button_clicked();
  void on_test_review_button_clicked();
  void on_start_button_clicked();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<ANetwork> m_network;
  A_ReLU_function m_ReLU;
  A_none_function m_none;

  ADataset m_train;
  ADataset m_test;

  ADataset read_mnist_from_csv(std::string file_name);

  void on_network_requested(ATrain_step_statistic statistic);
};
#endif // MAINWINDOW_H
