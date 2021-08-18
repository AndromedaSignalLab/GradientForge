#include "QSlidersWidget.hpp"
#include <QPainter>
#include <QDebug>
#include "MathUtil.hpp"

class Sorters {
public:
    /// sort the slider list
    static bool SliderSort(const SliderHandleWidget* a1, const SliderHandleWidget* a2);
    /// sort the color ramp
    static bool colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2);
};

// -----------------------------------------------------------
bool Sorters::colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2)
{
    return a1.first < a2.first;
}

// -----------------------------------------------------------
bool Sorters::SliderSort(const SliderHandleWidget* a1, const SliderHandleWidget* a2)
{
    return a1->value < a2->value;
}

// -----------------------------------------------------------
// QSlidersWidget --------------------------------------------
// -----------------------------------------------------------
MultiHandleSliderWidget::MultiHandleSliderWidget(Qt::Orientation orientation, QWidget* parent) : QWidget(parent),
    activeSlider(-1),
    boundarySpace(5)
{
    this->orientation = orientation;
    setContentsMargins(0,0,0,0);
    colorChooseDialog = new QColorDialog();
}

MultiHandleSliderWidget::~MultiHandleSliderWidget()
{
   if(colorChooseDialog)
       delete(colorChooseDialog);
   for (int i=0; i<sliderHandles.size(); i++) delete(sliderHandles[i]);
}

// -----------------------------------------------------------
void MultiHandleSliderWidget::setColorChoose(QColorDialog* coldlg)
{
    colorChooseDialog = coldlg;
}

int MultiHandleSliderWidget::getBoundSpace()
{
    return boundarySpace;
}

void MultiHandleSliderWidget::addSlider(const QPoint &position, const QColor &color, bool skipIfExists) {
    if(skipIfExists) {
        for (int i=0; i<sliderHandles.size(); i++)
        {
            QRect srec = sliderHandles[i]->geometry();
            if (srec.contains(position, true ))
            {
                return;
            }
        }
    }
    SliderHandleWidget* sl = new SliderHandleWidget(orientation, color, this);
    sliderHandles.push_back(sl);
    if (orientation==Qt::Horizontal)
    {
            sl->move(position.x(),0);
    }
    else
    {
        sl->move(0,position.y());
    }
    updateValue(sl);
    sl->show();
    std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit colorRampChanged(getRamp());
}

void MultiHandleSliderWidget::addSlider(const double value, const QColor &color)
{
    SliderHandleWidget* sl = new SliderHandleWidget(orientation, color, this);
    sliderHandles.push_back(sl);
    QPoint position = getPositionForValue(value, sl->width(), sl->height());
    sl->move(position);
    updateValue(sl);
    sl->show();
    std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit colorRampChanged(getRamp());
}

void MultiHandleSliderWidget::setSliderColor(int index, QColor col) {
    if (index<0 || index>=sliderHandles.size()) return;
    sliderHandles[index]->setColor(col);
}

ColorRamp MultiHandleSliderWidget::getRamp() {
    // create output
    ColorRamp ret;
    for (int i=0; i<sliderHandles.size(); i++)
    {
        QColor col = sliderHandles[i]->getColor();
        ret.push_back(QPair<qreal, QColor>(sliderHandles[i]->value,col));
    }
    // sort the slider list
    std::sort(ret.begin(), ret.end(), Sorters::colorRampSort);

    return ret;
}

void MultiHandleSliderWidget::setRamp(ColorRamp ramp) {
    if (ramp.size()<1) return;

    // sort the slider list
    std::sort(ramp.begin(), ramp.end(), Sorters::colorRampSort);

    for (int i=0; i<sliderHandles.size(); i++) delete(sliderHandles[i]);
    sliderHandles.clear();

    // create sliders
    for (int i=0; i<ramp.size(); i++)
    {
        SliderHandleWidget* sl = new SliderHandleWidget(orientation,ramp[i].second, this);
        sl->value = ramp[i].first;
        sliderHandles.push_back(sl);
        updatePos(sl);
        sl->show();
    }

    emit colorRampChanged(ramp);
    update();
}

qreal MultiHandleSliderWidget::updateValue(SliderHandleWidget* sl) {
    QRect crec = contentsRect();
    if (orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        sl->value = (1.0*sl->pos().x()-boundarySpace)/crec.width();
    }
    else
    {
        crec.adjust(0,boundarySpace,0,-boundarySpace);
        sl->value = (1.0*sl->pos().y()-boundarySpace)/crec.height();
    }
    return sl->value;
}

int MultiHandleSliderWidget::updatePos(SliderHandleWidget* sl) {
    QRect crec = contentsRect();
    qreal pos;
    if (orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (sl->value)*crec.width();
        pos -= sl->width()/2;
        pos += boundarySpace;
        sl->move(pos,0);
    }
    else
    {
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (sl->value)*crec.height();
        pos -= sl->height()/2;
        pos += boundarySpace;
        sl->move(0,pos);
    }
    return pos;
}

qreal MultiHandleSliderWidget::getNormalizedValue(qreal value) {
    return MathUtil::getNormalizedValue(value, 0, 1);
}

void MultiHandleSliderWidget::resizeEvent (QResizeEvent*) {
    for (int i=0; i<sliderHandles.size(); i++)
    {
        SliderHandleWidget* sl = sliderHandles[i];
        updatePos(sl);
    }
}

void MultiHandleSliderWidget::removeActiveSlider() {
    delete(sliderHandles[activeSlider]);
    sliderHandles.removeAt(activeSlider);
    activeSlider = -1;
    emit colorRampChanged(getRamp());
}

void MultiHandleSliderWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        if (sliderHandles.size()<1) return;
        for (int i=0; i<sliderHandles.size(); i++)
        {
            QRect srec = sliderHandles[i]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                activeSlider = i;
                break;
            }
        }
    }

    //for slider addition by single mouse click
    //addSlider(e->pos());
    //emit colorRampChanged(getRamp());
}

void MultiHandleSliderWidget::mouseMoveEvent(QMouseEvent* e) {
    if (activeSlider>=0)
    {
        qreal activeSliderValue = getValueFromPosition(e->pos());
        if (orientation==Qt::Horizontal)
        {
            if(activeSliderValue < 0.0) {
                if(activeSliderValue>=-0.08) {
                    if(activeSlider >= 0)
                       sliderHandles[activeSlider]->move(0,0);
                }
                else {
                    if(sliderHandles.size() > 1)
                        removeActiveSlider();
                    else
                        sliderHandles[activeSlider]->move(0,0);
                }
            }
            else if(activeSliderValue > 1.0) {
                if(activeSliderValue<=1.08) {
                    if(activeSlider >= 0)
                        sliderHandles[activeSlider]->move(getContentsRectangle().width(),0);
                }
                else {
                    if(sliderHandles.size() > 1)
                        removeActiveSlider();
                    else
                        sliderHandles[activeSlider]->move(getContentsRectangle().width(),0);
                }
            }
        }
        else
        {
        }

        if (activeSliderValue<0.0 || activeSliderValue>1.0)
        {
            //removeActiveSlider();
        }
        else
        {
            if (orientation==Qt::Horizontal)
                sliderHandles[activeSlider]->move(e->pos().x(), 0);
            else
                sliderHandles[activeSlider]->move(0,e->pos().y());

            updateValue(sliderHandles[activeSlider]);
            //std::sort(rampEditor->sliders.begin(), rampEditor->sliders.end(), SliderSort); //Enable to make sliders can't pass each other
            //if (rampeditor_->slideUpdate_) rampeditor_->updateRamp();
        }
        update();
        emit colorRampChanged(getRamp());
    }
}

void MultiHandleSliderWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton) {
        activeSlider = -1;
        emit colorRampChanged(getRamp());
    }
}
// -----------------------------------------------------------
void MultiHandleSliderWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        int index = -1;
        for (int i=0; i<sliderHandles.size(); i++)
        {
            QRect srec = sliderHandles[i]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                index = i;
                break;
            }
        }
        if (index>=0)
        {
            if (colorChooseDialog)
            {
                colorChooseDialog->exec();
                sliderHandles[index]->setColor(colorChooseDialog->selectedColor());
                emit colorRampChanged(getRamp());
            }
        }

        addSlider(e->pos(), Qt::white);
    }
}

QRect MultiHandleSliderWidget::getContentsRectangle()
{
    QRect crec = contentsRect();

    if (orientation==Qt::Horizontal) {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
    }
    else {
        crec.adjust(0,boundarySpace,0,-boundarySpace);
    }
    return crec;
}

qreal MultiHandleSliderWidget::getValueFromPosition(const QPoint &position)
{
    QRect crec = getContentsRectangle();
    qreal pos = orientation == Qt::Horizontal ? 1.0*(position.x()-boundarySpace)/(crec.width()) : 1.0*(position.y()-boundarySpace)/(crec.height());
    return pos;
}

QPoint MultiHandleSliderWidget::getPositionForValue(qreal value, qreal sliderWidth, qreal sliderHeight)
{
    QPoint position;
    QRect crec = contentsRect();
    qreal pos;
    if (orientation==Qt::Horizontal) {
        position.setY(0);
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (value)*crec.width();
        pos -= sliderWidth/2;
        pos += boundarySpace;
        position.setX(pos);
    }
    else
    {
        position.setX(0);
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (value)*crec.height();
        pos -= sliderHeight/2;
        pos += boundarySpace;
        position.setY(pos);
    }

    return position;
}

// -----------------------------------------------------------
int MultiHandleSliderWidget::getSliderNum()
{
    return sliderHandles.size();
}

// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
SliderHandleWidget::SliderHandleWidget(Qt::Orientation orientation, QColor col, QWidget* parent) : QWidget(parent),
    color(col)
{
    this->orientation = orientation;
    if (orientation==Qt::Horizontal)
        setFixedSize(9, 16);
    else
        setFixedSize(16,9);
}
// -----------------------------------------------------------
void SliderHandleWidget::setColor(QColor col)
{
    color = col;
}
// -----------------------------------------------------------
QColor SliderHandleWidget::getColor()
{
    return color;
}

void SliderHandleWidget::move(int ax, int ay)
{
    if (orientation==Qt::Horizontal)
        QWidget::move(ax - geometry().width()/2, ay);
    else
        QWidget::move(ax, ay - geometry().height()/2);
}

void SliderHandleWidget::move(const QPoint &position) {
    move(position.x(), position.y());
}

// -----------------------------------------------------------
void SliderHandleWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(color);
    if (orientation==Qt::Horizontal)
    {
        //QRect rec(0,7,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(0,7) << QPoint(4,0) << QPoint(8,7) << QPoint(8, 15) << QPoint(0, 15);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }
    else
    {
        //QRect rec(7,0,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(7,0) << QPoint(0,4) << QPoint(7,8) << QPoint(15, 8) << QPoint(15,0);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }

}
