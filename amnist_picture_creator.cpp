#include "amnist_picture_creator.h"

AMnist_picture_creator::AMnist_picture_creator(QWidget *parent) : QWidget(parent)
{
  clear();
  startTimer(50);
}

QVector<double> AMnist_picture_creator::pic_as_features()
{
  QVector<double> result;
  for ( int y = 0; y < PIC_HEIGHT; ++y)
    {
      for ( int x = 0; x < PIC_WIDTH; ++x)
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

void AMnist_picture_creator::reduce(uint x, uint y, uint value)
{
  if ((x < PIC_WIDTH) && (y < PIC_HEIGHT))
    {
      if (m_picture[x][y] > value)
        m_picture[x][y] -= value;
      else
        m_picture[x][y] = 0;
    }

}

void AMnist_picture_creator::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() == Qt::LeftButton)
    {
      double x_koef = width() / PIC_WIDTH;
      double y_koef = height() / PIC_HEIGHT;
      uint x = int(event->pos().x()/x_koef);
      uint y = int(event->pos().y()/y_koef);

      const uint8_t BIG_REDUCE = 70;
      const uint8_t SMALL_REDUCE = 20;

      reduce(x,y,BIG_REDUCE);
      reduce(x-1,y-1,SMALL_REDUCE);
      reduce(x-1,y+1,SMALL_REDUCE);
      reduce(x+1,y-1,SMALL_REDUCE);
      reduce(x+1,y+1,SMALL_REDUCE);
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
