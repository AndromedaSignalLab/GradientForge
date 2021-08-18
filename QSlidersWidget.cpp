#include "QSlidersWidget.hpp"
#include <QPainter>
#include <QDebug>
class Sorters {
public:
    /// sort the slider list
    static bool SliderSort(const QSlidersHandleWidget* a1, const QSlidersHandleWidget* a2);
    /// sort the color ramp
    static bool colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2);
};

// -----------------------------------------------------------
bool Sorters::colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2)
{
    return a1.first < a2.first;
}

// -----------------------------------------------------------
bool Sorters::SliderSort(const QSlidersHandleWidget* a1, const QSlidersHandleWidget* a2)
{
    return a1->value < a2->value;
}

// -----------------------------------------------------------
// QSlidersWidget --------------------------------------------
// -----------------------------------------------------------
QSlidersWidget::QSlidersWidget(Qt::Orientation orientation, QWidget* parent) : QWidget(parent),
    activeSlider(-1),
    boundarySpace(5)
{
    this->orientation = orientation;
    setContentsMargins(0,0,0,0);
    colorChooseDialog = new QColorDialog();
}

QSlidersWidget::~QSlidersWidget()
{
   if(colorChooseDialog)
       delete(colorChooseDialog);
   for (int i=0; i<sliders.size(); i++) delete(sliders[i]);
}

// -----------------------------------------------------------
void QSlidersWidget::setColorChoose(QColorDialog* coldlg)
{
    colorChooseDialog = coldlg;
}

int QSlidersWidget::getBoundSpace()
{
    return boundarySpace;
}

void QSlidersWidget::addSlider(const QPoint &position, const QColor &color, bool skipIfExists) {
    if(skipIfExists) {
        for (int i=1; i<sliders.size()-1; i++)
        {
            QRect srec = sliders[i]->geometry();
            if (srec.contains(position, true ))
            {
                return;
            }
        }
    }
    QSlidersHandleWidget* sl = new QSlidersHandleWidget(orientation, color, this);
    sliders.push_back(sl);
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
    std::sort(sliders.begin(), sliders.end(), Sorters::SliderSort);
    emit colorRampChanged(getRamp());
}

void QSlidersWidget::setSliderColor(int index, QColor col) {
    if (index<0 || index>=sliders.size()) return;
    sliders[index]->setColor(col);
}

ColorRamp QSlidersWidget::getRamp() {
    // create output
    ColorRamp ret;
    for (int i=0; i<sliders.size(); i++)
    {
        QColor col = sliders[i]->getColor();
        ret.push_back(QPair<qreal, QColor>(sliders[i]->value,col));
    }
    // sort the slider list
    std::sort(ret.begin(), ret.end(), Sorters::colorRampSort);

    return ret;
}

void QSlidersWidget::setRamp(ColorRamp ramp) {
    if (ramp.size()<1) return;

    // sort the slider list
    std::sort(ramp.begin(), ramp.end(), Sorters::colorRampSort);

    for (int i=0; i<sliders.size(); i++) delete(sliders[i]);
    sliders.clear();

    // find min/max
    mi_=ramp.first().first;
    ma_=ramp.last().first;

    // create sliders
    for (int i=0; i<ramp.size(); i++)
    {
        QSlidersHandleWidget* sl = new QSlidersHandleWidget(orientation,ramp[i].second, this);
        sl->value = ramp[i].first;
        sliders.push_back(sl);
        updatePos(sl);
        sl->show();
    }

    emit colorRampChanged(ramp);
    update();
}

qreal QSlidersWidget::updateValue(QSlidersHandleWidget* sl) {
    QRect crec = contentsRect();
    if (orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        sl->value = mi_ + (1.0*sl->pos().x()-boundarySpace)/crec.width()*(ma_-mi_);
    }
    else
    {
        crec.adjust(0,boundarySpace,0,-boundarySpace);
        sl->value = mi_ + (1.0*sl->pos().y()-boundarySpace)/crec.height()*(ma_-mi_);
    }
    return sl->value;
}

int QSlidersWidget::updatePos(QSlidersHandleWidget* sl) {
    QRect crec = contentsRect();
    qreal pos;
    if (orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (sl->value- mi_)/(ma_-mi_)*crec.width();
        pos -= sl->width()/2;
        pos += boundarySpace;
        sl->move(pos,0);
    }
    else
    {
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (sl->value- mi_)/(ma_-mi_)*crec.height();
        pos -= sl->height()/2;
        pos += boundarySpace;
        sl->move(0,pos);
    }
    return pos;
}

qreal QSlidersWidget::getNormalizedValue(qreal value) {
    return (value - mi_)/(ma_-mi_);
}

void QSlidersWidget::resizeEvent (QResizeEvent*) {
    for (int i=0; i<sliders.size(); i++)
    {
        QSlidersHandleWidget* sl = sliders[i];
        updatePos(sl);
    }
}

void QSlidersWidget::removeActiveSlider() {
    delete(sliders[activeSlider]);
    sliders.removeAt(activeSlider);
    activeSlider = -1;
    emit colorRampChanged(getRamp());
}

void QSlidersWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        if (sliders.size()<2) return;
        for (int i=1; i<sliders.size()-1; i++)
        {
            QRect srec = sliders[i]->geometry();
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

void QSlidersWidget::mouseMoveEvent(QMouseEvent* e) {
    if (activeSlider>=0)
    {
        QRect crec = contentsRect();

        qreal pos;
        if (orientation==Qt::Horizontal)
        {
            crec.adjust(boundarySpace,0,-boundarySpace,0);
            pos = 1.0*(e->pos().x()-boundarySpace)/(crec.width());
            if(pos < 0.0) {
                if(pos>=-0.08) {
                    if(activeSlider >= 0)
                       sliders[activeSlider]->move(0,0);
                }
                else {
                    removeActiveSlider();
                }
            }
            else if(pos > 1.0) {
                if(pos<=1.08) {
                    if(activeSlider >= 0)
                        sliders[activeSlider]->move(crec.width(),0);
                }
                else {
                    removeActiveSlider();
                }
            }
        }
        else
        {
            crec.adjust(0,boundarySpace,0,-boundarySpace);
            pos = 1.0*(e->pos().y()-boundarySpace)/(crec.height());
        }

        if (pos<0.0 || pos>1.0)
        {
            //removeActiveSlider();
        }
        else
        {
            if (orientation==Qt::Horizontal)
                sliders[activeSlider]->move(e->pos().x(), 0);
            else
                sliders[activeSlider]->move(0,e->pos().y());

            updateValue(sliders[activeSlider]);
            //std::sort(rampEditor->sliders.begin(), rampEditor->sliders.end(), SliderSort); //Enable to make sliders can't pass each other
            //if (rampeditor_->slideUpdate_) rampeditor_->updateRamp();
        }
        update();
        emit colorRampChanged(getRamp());
    }
}

void QSlidersWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton) {
        activeSlider = -1;
        emit colorRampChanged(getRamp());
    }
}
// -----------------------------------------------------------
void QSlidersWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        int index = -1;
        for (int i=0; i<sliders.size(); i++)
        {
            QRect srec = sliders[i]->geometry();
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
                sliders[index]->setColor(colorChooseDialog->selectedColor());
                emit colorRampChanged(getRamp());
            }
        }

        addSlider(e->pos(), Qt::white);
    }
}

// -----------------------------------------------------------
int QSlidersWidget::getSliderNum()
{
    return sliders.size();
}

// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
QSlidersHandleWidget::QSlidersHandleWidget(Qt::Orientation orientation, QColor col, QWidget* parent) : QWidget(parent),
    color(col)
{
    this->orientation = orientation;
    if (orientation==Qt::Horizontal)
        setFixedSize(9, 16);
    else
        setFixedSize(16,9);
}
// -----------------------------------------------------------
void QSlidersHandleWidget::setColor(QColor col)
{
    color = col;
}
// -----------------------------------------------------------
QColor QSlidersHandleWidget::getColor()
{
    return color;
}

void QSlidersHandleWidget::move(int ax, int ay)
{
    if (orientation==Qt::Horizontal)
        QWidget::move(ax - geometry().width()/2, ay);
    else
        QWidget::move(ax, ay - geometry().height()/2);
}

// -----------------------------------------------------------
void QSlidersHandleWidget::paintEvent(QPaintEvent* e)
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
