#include "MultiHandleSlider.hpp"
#include <QPainter>
#include <QDebug>
#include "MathUtil.hpp"
#include "Sorters.hpp"

MultiHandleSlider::MultiHandleSlider(Qt::Orientation orientation, QWidget* parent) : QWidget(parent),
    activeSlider(-1)
{
    this->orientation = orientation;
    setContentsMargins(0,0,0,0);
    colorChooseDialog = new QColorDialog();
    handleProperties.width = 24;
    handleProperties.capHeight = 4;
    handleProperties.height = 16;
}

MultiHandleSlider::~MultiHandleSlider()
{
   if(colorChooseDialog)
       delete(colorChooseDialog);
   for (int i=0; i<sliderHandles.size(); i++) delete(sliderHandles[i]);
}

// -----------------------------------------------------------
void MultiHandleSlider::setColorChoose(QColorDialog* coldlg)
{
    colorChooseDialog = coldlg;
}

int MultiHandleSlider::getBoundarySpace()
{
    return handleProperties.width/2;
}

void MultiHandleSlider::addSlider(const QPoint &position, const QColor &color, bool skipIfExists) {
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
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
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

void MultiHandleSlider::addSlider(const double &value, const QColor &color)
{
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
    sliderHandles.push_back(sl);
    QPoint position = getPositionForValue(value, sl->width(), sl->height());
    sl->move(position);
    updateValue(sl);
    sl->show();
    std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit colorRampChanged(getRamp());
}

void MultiHandleSlider::setSliderColor(int index, QColor col) {
    if (index<0 || index>=sliderHandles.size()) return;
    sliderHandles[index]->setColor(col);
}

ColorRamp MultiHandleSlider::getRamp() {
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

void MultiHandleSlider::setRamp(ColorRamp ramp) {
    if (ramp.size()<1) return;

    // sort the slider list
    std::sort(ramp.begin(), ramp.end(), Sorters::colorRampSort);

    for (int i=0; i<sliderHandles.size(); i++) delete(sliderHandles[i]);
    sliderHandles.clear();

    // create sliders
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    for (int i=0; i<ramp.size(); i++)
    {
        handleProperties.color = ramp[i].second;
        SliderHandle* sl = new SliderHandle(handleProperties, this);
        sl->value = ramp[i].first;
        sliderHandles.push_back(sl);
        updatePos(sl);
        sl->show();
    }

    emit colorRampChanged(ramp);
    update();
}

qreal MultiHandleSlider::updateValue(SliderHandle* sl) {
    QRect crec = contentsRect();
    int boundarySpace = getBoundarySpace();
    if (orientation==Qt::Horizontal)
    {
        //crec.adjust(boundarySpace,0,-boundarySpace,0);
        //sl->value = (1.0*sl->pos().x()-boundarySpace)/crec.width();

    }
    else
    {
        //crec.adjust(0,boundarySpace,0,-boundarySpace);
        //sl->value = (1.0*sl->pos().y()-boundarySpace)/crec.height();
    }
    sl->value = getValueFromPosition(sl->pos());
    return sl->value;
}

int MultiHandleSlider::updatePos(SliderHandle* sl) {
    QRect crec = contentsRect();
    qreal pos;
    int boundarySpace = getBoundarySpace();
    if (orientation==Qt::Horizontal)
    {
        //crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (sl->value)*crec.width();
        //pos -= getBoundarySpace();
        //pos += boundarySpace;
        //sl->move(pos,0);
    }
    else
    {
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (sl->value)*crec.height();
        //pos -= getBoundarySpace();
        pos += boundarySpace;
        sl->move(0,pos);
    }
    return pos;
}

qreal MultiHandleSlider::getNormalizedValue(qreal value) {
    return MathUtil::getNormalizedValue(value, 0, 1);
}

void MultiHandleSlider::resizeEvent (QResizeEvent*) {
    for (int i=0; i<sliderHandles.size(); i++)
    {
        SliderHandle* sl = sliderHandles[i];
        updatePos(sl);
    }
}

void MultiHandleSlider::removeActiveSlider() {
    delete(sliderHandles[activeSlider]);
    sliderHandles.removeAt(activeSlider);
    activeSlider = -1;
    emit colorRampChanged(getRamp());
}

void MultiHandleSlider::mousePressEvent(QMouseEvent* e) {
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

void MultiHandleSlider::mouseMoveEvent(QMouseEvent* e) {
    if (activeSlider>=0)
    {
        qreal activeSliderValue = getValueFromPosition(e->pos());

        if (orientation==Qt::Horizontal)
        {
            if(activeSliderValue >=0 && activeSliderValue <=1) {
                sliderHandles[activeSlider]->value = activeSliderValue;
                sliderHandles[activeSlider]->move(e->pos().x(), 0);
                qDebug()<<"Active slider value: " << activeSliderValue;
                //updateValue(sliderHandles[activeSlider]);
            }
            if(activeSliderValue < 0.0) {

                if(activeSliderValue<=-0.1) {
                    if(sliderHandles.count() > 1)
                        removeActiveSlider();
                }
            }
            else if(activeSliderValue > 1.0) {
                if(activeSliderValue>=1.1) {
                    if(sliderHandles.count() > 1)
                    removeActiveSlider();
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
            /*
            if (orientation==Qt::Horizontal) {
                sliderHandles[activeSlider]->move(e->pos().x(), 0);
                qDebug()<<"Moving to " << e->pos().x();
            }
            else
                sliderHandles[activeSlider]->move(0,e->pos().y());
            */

            //std::sort(rampEditor->sliders.begin(), rampEditor->sliders.end(), SliderSort); //Enable to make sliders can't pass each other
            //if (rampeditor_->slideUpdate_) rampeditor_->updateRamp();
        }
        update();
        emit colorRampChanged(getRamp());
    }
}

void MultiHandleSlider::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton) {
        activeSlider = -1;
        emit colorRampChanged(getRamp());
    }
}
// -----------------------------------------------------------
void MultiHandleSlider::mouseDoubleClickEvent(QMouseEvent* e)
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

QRect MultiHandleSlider::getContentsRectangle()
{
    QRect crec = contentsRect();
    int boundarySpace = getBoundarySpace();
    if (orientation==Qt::Horizontal) {
        //crec.adjust(boundarySpace,0,-boundarySpace,0);
    }
    else {
        //crec.adjust(0,boundarySpace,0,-boundarySpace);
    }
    return crec;
}

qreal MultiHandleSlider::getValueFromPosition(const QPoint &position)
{
    int boundarySpace = getBoundarySpace();
    QRect crec = getContentsRectangle();
    qreal pos = orientation == Qt::Horizontal ? 1.0*(position.x()-boundarySpace)/(crec.width() - boundarySpace*2) : 1.0*(position.y()-boundarySpace)/(crec.height() - boundarySpace*2);
    return pos;
}

QPoint MultiHandleSlider::getPositionForValue(qreal value, qreal sliderWidth, qreal sliderHeight)
{
    QPoint position;
    QRect crec = contentsRect();
    qreal pos;
    int boundarySpace = getBoundarySpace();
    if (orientation==Qt::Horizontal) {
        position.setY(0);
        //crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (value)*crec.width();
        //pos -= sliderWidth;
        pos += boundarySpace;
        position.setX(pos);
    }
    else
    {
        position.setX(0);
        //crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (value)*crec.height();
        //pos -= sliderHeight/2;
        pos += boundarySpace;
        position.setY(pos);
    }

    return position;
}

int MultiHandleSlider::getSliderNum()
{
    return sliderHandles.size();
}
