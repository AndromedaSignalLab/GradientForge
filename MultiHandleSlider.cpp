#include "MultiHandleSlider.hpp"
#include <QPainter>
#include <QDebug>
#include "MathUtil.hpp"
#include "Sorters.hpp"

MultiHandleSlider::MultiHandleSlider(QWidget* parent, Qt::Orientation orientation) : QWidget(parent)
{
    this->orientation = orientation;
    setContentsMargins(0,0,0,0);
    colorChooseDialog = new QColorDialog();
    handleProperties.width = 24;
    handleProperties.capHeight = 8;
    handleProperties.height = 16;
}

MultiHandleSlider::~MultiHandleSlider()
{
   if(colorChooseDialog)
       delete(colorChooseDialog);
   for(QUuid id : sliderHandles.keys()) {
       delete sliderHandles[id];
   }
   sliderHandles.clear();
}

bool MultiHandleSlider::isVertical() const
{
    return orientation == Qt::Orientation::Vertical;
}

void MultiHandleSlider::setVertical(const bool &vertical)
{
    orientation = vertical ? Qt::Orientation::Vertical : Qt::Orientation::Horizontal;
}

void MultiHandleSlider::setColorChoose(QColorDialog* coldlg)
{
    colorChooseDialog = coldlg;
}

int MultiHandleSlider::getBoundarySpace()
{
    return handleProperties.width/2;
}

SliderHandle * MultiHandleSlider::addSlider(const QPoint &position, const QColor &color, bool skipIfExists) {
    if(skipIfExists) {
        for(QUuid id : sliderHandles.keys()) {
            QRect srec = sliderHandles[id]->geometry();
            if (srec.contains(position, true ))
            {
                return nullptr;
            }
        }
    }
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
    sliderHandles[sl->id] = sl;
    if (orientation==Qt::Horizontal)
    {
            sl->move(position.x(),0);
    }
    else
    {
        sl->move(0,position.y());
    }
    //updateValue(sl);
    sl->show();
    //std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit sliderChanged();
    return sl;
}

SliderHandle * MultiHandleSlider::addSlider(const double &value, const QColor &color)
{
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
    sliderHandles[sl->id] = sl;
    QPoint position = getPositionForValue(value);
    sl->move(position);
    //updateValue(sl);
    sl->show();
    //std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit sliderChanged();
    return sl;
}

void MultiHandleSlider::setSliderColor(QUuid sliderId, QColor color) {
    if(!sliderHandles.contains(sliderId))
        return;
    sliderHandles[sliderId]->setColor(color);
}

ColorRamp MultiHandleSlider::getColorRamp() {
    // create output
    ColorRamp ret;
    for(QUuid id : sliderHandles.keys()) {
        QColor col = sliderHandles[id]->getColor();
        ret.push_back(QPair<qreal, QColor>(sliderHandles[id]->getValue(),col));
    }
    // sort the slider list
    std::sort(ret.begin(), ret.end(), Sorters::colorRampSort);

    return ret;
}

void MultiHandleSlider::setRamp(ColorRamp ramp) {
    if (ramp.size()<1) return;

    // sort the slider list
    std::sort(ramp.begin(), ramp.end(), Sorters::colorRampSort);

    for(QUuid id : sliderHandles.keys()) {
        //SliderHandle *sl = sliderHandles[id];
        //sliderHandles.remove(id);
        //delete sl;
        delete sliderHandles[id];
    }

    sliderHandles.clear();

    // create sliders
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    for (int i=0; i<ramp.size(); i++)
    {
        handleProperties.color = ramp[i].second;
        SliderHandle* sl = new SliderHandle(handleProperties, this);
        //sl->value = ramp[i].first;
        sliderHandles[sl->id] = sl;
        sl->setValue(ramp[i].first);
        sl->show();
    }

    emit sliderChanged();
    update();
}

qreal MultiHandleSlider::getNormalizedValue(qreal value) {
    return MathUtil::getNormalizedValue(value, 0, 1);
}

void MultiHandleSlider::resizeEvent (QResizeEvent*) {
    for(QUuid id : sliderHandles.keys()) {
        SliderHandle* sl = sliderHandles[id];
        sl->update();
    }
}

void MultiHandleSlider::removeActiveSlider() {
    if(!activeSliderId.isNull() && sliderHandles.contains(activeSliderId)) {
        delete sliderHandles[activeSliderId];
        sliderHandles.remove(activeSliderId);
    }
    QUuid nullId;
    activeSliderId = nullId;
    emit sliderChanged();
}

void MultiHandleSlider::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        if (sliderHandles.size()<1) return;

        for(QUuid id : sliderHandles.keys()) {
            QRect srec = sliderHandles[id]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                activeSliderId = id;
                break;
            }
        }
    }

    //for slider addition by single mouse click
    //addSlider(e->pos());
    //emit colorRampChanged(getRamp());
}

void MultiHandleSlider::mouseMoveEvent(QMouseEvent* e) {
    if (!activeSliderId.isNull())
    {
        qreal activeSliderValue = getValueFromPosition(e->pos());

        if (orientation==Qt::Horizontal)
        {
            if(activeSliderValue >=0 && activeSliderValue <=1) {
                //sliderHandles[activeSlider]->value = activeSliderValue;
                sliderHandles[activeSliderId]->move(e->pos().x(), 0);
                //qDebug()<<"Active slider value: " << activeSliderValue;
                emit sliderValueChanged(sliderHandles[activeSliderId]->id, activeSliderValue);
                //updateValue(sliderHandles[activeSlider]);
            }
            if(activeSliderValue < 0.0) {

                if(activeSliderValue<=-0.1) {
                    if(sliderHandles.count() > 1)
                        removeActiveSlider();
                }
                else {
                    sliderHandles[activeSliderId]->setValue(0);
                }
            }
            else if(activeSliderValue > 1.0) {
                if(activeSliderValue>=1.1) {
                    if(sliderHandles.count() > 1)
                    removeActiveSlider();
                }
                else {
                    sliderHandles[activeSliderId]->setValue(1);
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
        emit sliderChanged();
    }
}

void MultiHandleSlider::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton) {
        QUuid nullId;
        activeSliderId = nullId;
        emit sliderChanged();
    }
}

void MultiHandleSlider::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        QUuid index;

        for(QUuid id : sliderHandles.keys()) {
            QRect srec = sliderHandles[id]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                index = id;
                break;
            }
        }

        for(QUuid id : sliderHandles.keys()) {
            QRect srec = sliderHandles[id]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                index = id;
                break;
            }
        }
        if (!index.isNull())
        {
            if (colorChooseDialog)
            {
                colorChooseDialog->exec();
                sliderHandles[index]->setColor(colorChooseDialog->selectedColor());
                emit sliderChanged();
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
    qreal value = MathUtil::getNormalizedValue(position, crec, boundarySpace, orientation);
    //qreal value = orientation == Qt::Horizontal ? 1.0*(position.x()-boundarySpace)/(crec.width() - boundarySpace*2) : 1.0*(position.y()-boundarySpace)/(crec.height() - boundarySpace*2);
    return value;
}

QPoint MultiHandleSlider::getPositionForValue(qreal value)
{
    return MathUtil::getPositionForNormalizedValue(value, contentsRect(), getBoundarySpace(), orientation);
}

int MultiHandleSlider::getSliderAmount()
{
    return sliderHandles.size();
}
