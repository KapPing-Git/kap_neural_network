#ifndef AMNIST_PICTURE_CREATOR_H
#define AMNIST_PICTURE_CREATOR_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>

class AMnist_picture_creator : public QWidget
{
  Q_OBJECT
public:
  explicit AMnist_picture_creator(QWidget *parent = nullptr);

  QVector<double> pic_as_features();
  void clear();

signals:

private:
  static const int PIC_WIDTH = 28;
  static const int PIC_HEIGHT = 28;
  uint8_t m_picture[PIC_WIDTH][PIC_HEIGHT];

  void reduce(uint x, uint y,uint value);

  // QWidget interface
protected:
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *) override;

  // QObject interface
protected:
  virtual void timerEvent(QTimerEvent *) override;
};

#endif // AMNIST_PICTURE_CREATOR_H
