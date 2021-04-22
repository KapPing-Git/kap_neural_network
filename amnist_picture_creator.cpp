#include "amnist_picture_creator.h"

AMnist_picture_creator::AMnist_picture_creator(QWidget *parent) : QWidget(parent)
{
  clear();
  startTimer(50);
}

QVector<double> AMnist_picture_creator::pic_as_features()
{
  QVector<double> result(PIC_WIDTH*PIC_HEIGHT);
  for ( int x = 0; x < PIC_WIDTH; ++x)
    {
      for ( int y = 0; y < PIC_HEIGHT; ++y)
        {
          result.append(double(255 - m_picture[x][y]) / 255.0);
        }
    }
  return result;
}

void AMnist_picture_creator::clear()
{
  for ( int x = 0; x < PIC_WIDTH; ++x)
    {
      for ( int y = 0; y < PIC_HEIGHT; ++y)
        {
          m_picture[x][y] = 255;
        }
    }
}

void AMnist_picture_creator::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() == Qt::LeftButton)
    {
      double x_koef = width() / PIC_WIDTH;
      double y_koef = height() / PIC_HEIGHT;
      m_picture[int(event->pos().x()/x_koef)][int(event->pos().y()/y_koef)] = 0;
    }
}

void AMnist_picture_creator::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  double x_koef = width() / PIC_WIDTH;
  double y_koef = height() / PIC_HEIGHT;
  for ( int x = 0; x < PIC_WIDTH; ++x)
    {
      for ( int y = 0; y < PIC_HEIGHT; ++y)
        {
          QRect rect(x*x_koef,y*y_koef,x_koef,y_koef);
          QColor color(m_picture[x][y],m_picture[x][y],m_picture[x][y]);
          painter.fillRect(rect,color);
//          painter.drawRect(rect);
        }
    }
}

void AMnist_picture_creator::timerEvent(QTimerEvent *)
{
  update();
}
