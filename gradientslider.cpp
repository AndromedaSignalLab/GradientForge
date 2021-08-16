//Project
#include "gradientslider.hpp"

//Qt
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QWidgetAction>
#include <QApplication>
#include <QPixmap>
#include <QCursor>
#include <QGuiApplication>
#include <QDir>
#include <QProxyStyle>

#include <QDial>

QString ImagesPath(QString path) {
    return "/Users/volkan/" + path;
}

class SliderProxy : public QProxyStyle
{
public:
  int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
  {
    switch(metric) {
    case PM_SliderThickness  : return 25;
    case PM_SliderLength     : return 25;
    default                  : return (QProxyStyle::pixelMetric(metric,option,widget));
    }
  }
};

GradientSlider::GradientSlider(QWidget *parent)
  : QSlider(parent)
{
  //styling
  setOrientation(Qt::Horizontal);
  setAcceptDrops(true);
  SliderProxy *aSliderProxy = new SliderProxy();

  //hard coded path to image :/ sorry
  QString path = QDir::fromNativeSeparators(ImagesPath("handle.png"));
  setStyleSheet("QSlider::handle { image: url(" + path + "); }");
  setStyle(aSliderProxy);

  //setting up the alternate handle
  alt_handle = new GradientSliderHandle(this);
  addAction(new QWidgetAction(alt_handle));
  alt_handle->move(this->pos().x() + this->width()- alt_handle->width(), this->pos().y() );

}

GradientSliderHandle::GradientSliderHandle(GradientSlider *_parent)
  : QLabel(_parent)
{
  parent = _parent;
  filter = new SliderEventFilter(parent);

  //styling
  setAcceptDrops(true);
  //hard coded path to image :/ sorry
  QPixmap pix = QPixmap(ImagesPath("handle.png"));
  pix =  pix.scaled(25, 25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  setPixmap(pix);
}

int GradientSlider::alt_value()
{
  return alt_handle->value();
}

void GradientSlider::alt_setValue(int value)
{
  alt_handle->setValue(value);
}

void GradientSlider::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
  if (mouseEvent->button() == Qt::LeftButton)
  {
    alt_handle->show();
    alt_handle->handleActivated = false;
  }
  mouseEvent->accept();
}

void GradientSlider::alt_update()
{
  QPoint posCursor(QCursor::pos());
  QPoint posParent(mapToParent(mapToGlobal(pos())));
  QPoint point(alt_handle->mapToParent(alt_handle->mapFromGlobal(QCursor::pos())).x(),alt_handle->y());
  int horBuffer = (alt_handle->width());
  bool lessThanMax = mapToParent(point).x() < pos().x()+ width() - horBuffer;
  bool greaterThanMin = mapToParent(point).x() > pos().x();
  if(lessThanMax && greaterThanMin)
    alt_handle->move(point);
  emit alt_valueChanged(alt_value());
}

void GradientSliderHandle::mousePressEvent(QMouseEvent *mouseEvent)
{
  qGuiApp->installEventFilter(filter);
  parent->clearFocus();
}

bool SliderEventFilter::eventFilter(QObject* obj, QEvent* event)
{
  switch(event->type())
  {
  case QEvent::MouseButtonRelease:
    qGuiApp->removeEventFilter(this);
    return true;
    break;
  case QEvent::MouseMove:
    grandParent->alt_update();
    return true;
    break;
  default:
    return QObject::eventFilter(obj, event);
  }
  return false;
}

void GradientSliderHandle::setValue(double value)
{
  double width = parent->width(), position = pos().x();
  double range = parent->maximum() - parent->minimum();
  int location = (value - parent->minimum())/range;
  location = location *width;
  move(y(),location);
}

int GradientSliderHandle::value()
{
  double width = parent->width(), position = pos().x();
  double value = position/width;
  double range = parent->maximum() - parent->minimum();
  return parent->minimum() + (value * range);
}
void GradientSlider::Reset()
{
  int horBuffer = (alt_handle->width());
  QPoint myPos = mapToGlobal(pos());
  QPoint point(myPos.x() + width() - horBuffer, myPos.y()- alt_handle->height());
  point = alt_handle->mapFromParent(point);

  alt_handle->move(point);
  alt_handle->show();
  alt_handle->raise();

}
