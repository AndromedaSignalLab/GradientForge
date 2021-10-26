#include "MultiHandleSlider.hpp"
#include <QPainter>
#include <QDebug>
#include "MathUtil.hpp"
#include "Sorters.hpp"

MultiHandleSlider::MultiHandleSlider(QWidget* parent, Qt::Orientation orientation) : QWidget(parent)
{
    this->orientation = orientation;
    setContentsMargins(0,0,0,0);
    handleProperties.width = 24;
    handleProperties.capHeight = 8;
    handleProperties.height = 16;
}

MultiHandleSlider::~MultiHandleSlider()
{
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
    for(SliderHandle *sliderHandle : sliderHandles.values()) {
        sliderHandle->setOrientation(orientation);
    }
}

void MultiHandleSlider::setValue(const QUuid & sliderHandleId, const qreal & value)
{
    sliderHandles[sliderHandleId]->setValue(value);
    updatePosition(sliderHandleId);
    emit sliderValueChanged(sliderHandleId, value);
    emit sliderChanged();
}

qreal MultiHandleSlider::calculateValue(const QUuid & sliderHandleId)
{
    QRect crec = contentsRect();
    return MathUtil::getNormalizedValue(sliderHandles[sliderHandleId]->pos(), crec, handleProperties.width/2, handleProperties.orientation);
}

qreal MultiHandleSlider::getValue(const QUuid & sliderHandleId)
{
    return sliderHandles[sliderHandleId]->getValue();
}

void MultiHandleSlider::updatePosition(const QUuid & sliderHandleId)
{
    QRect crec = contentsRect();
    QPoint pos = MathUtil::getPositionForNormalizedValue(sliderHandles[sliderHandleId]->getValue(), crec, getBoundarySpace(), orientation);
    sliderHandles[sliderHandleId]->move(pos);
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

    qreal value = getValueFromPosition(position);

    if(value < 0)
        return addSlider(0, color);

    if(value > 1)
        return addSlider(1, color);

    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
    addSliderHandle(sl);
    if (orientation==Qt::Horizontal) {
            sl->move(position.x(),0);
    }
    else {
        sl->move(0,position.y());
    }
    sl->setValue(value);
    //updateValue(sl);
    sl->show();
    //std::sort(sliderHandles.begin(), sliderHandles.end(), Sorters::SliderSort);
    emit sliderAdded(sl->id, sl->getColor(), sl->getValue());
    return sl;
}

SliderHandle * MultiHandleSlider::addSlider(const double &value, const QColor &color)
{
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    handleProperties.color = color;
    SliderHandle* sl = new SliderHandle(handleProperties, this);
    addSliderHandle(sl);
    setValue(sl->id, value);
    sl->show();
    emit sliderAdded(sl->id, sl->getColor(), sl->getValue());
    return sl;
}

void MultiHandleSlider::setSliderColor(QUuid sliderId, QColor color) {
    if(!sliderHandles.contains(sliderId))
        return;
    sliderHandles[sliderId]->setColor(color);
}

ColorRamp MultiHandleSlider::getColorRamp() {
    // create output
    ColorRamp colorRamp;
    for(QUuid id : sliderHandles.keys()) {
        QColor col = sliderHandles[id]->getColor();
        colorRamp.push_back(QPair<qreal, QColor>(sliderHandles[id]->getValue(),col));
    }
    // sort the slider list
    std::sort(colorRamp.begin(), colorRamp.end(), Sorters::colorRampSort);

    return colorRamp;
}

void MultiHandleSlider::setColorRamp(const ColorRamp &colorRamp) {
    if (colorRamp.size()<1) return;

    // sort the slider list
    std::sort(colorRamp.begin(), colorRamp.end(), Sorters::colorRampSort);
    sliderHandles.clear();
    sliderHandleStack.clear();

    // create sliders
    SliderHandleProperties handleProperties = this->handleProperties;
    handleProperties.orientation = orientation;
    for (int i=0; i<colorRamp.size(); i++)
    {
        handleProperties.color = colorRamp[i].second;
        SliderHandle* sl = new SliderHandle(handleProperties, this);
        addSliderHandle(sl);
        setValue(sl->id, colorRamp[i].first);
        sl->show();
    }

    emit sliderChanged();
    update();
}

qreal MultiHandleSlider::getNormalizedValue(qreal value) {
    return MathUtil::getNormalizedValue(value, 0, 1);
}

void MultiHandleSlider::resizeEvent (QResizeEvent*) {
    for(const QUuid &id : sliderHandles.keys()) {
        updatePosition(id);
    }
}

void MultiHandleSlider::removeActiveSlider() {
    if(!activeSliderId.isNull() && sliderHandles.contains(activeSliderId)) {
        removeSliderHandle(sliderHandles[activeSliderId]);
    }
    QUuid nullId;
    activeSliderId = nullId;
    emit sliderChanged();
}

void MultiHandleSlider::addSliderHandle(SliderHandle * sliderHandle)
{
    if(sliderHandle == nullptr)
        return;
    if(!sliderHandles.contains(sliderHandle->id))
        sliderHandles[sliderHandle->id] = sliderHandle;
    if(!sliderHandleStack.contains(sliderHandle)){
        sliderHandleStack.push(sliderHandle);
    }
    emit sliderAdded(sliderHandle->id, sliderHandle->getColor(), sliderHandle->getValue());
}

void MultiHandleSlider::removeSliderHandle(SliderHandle * sliderHandle)
{
    if(sliderHandle == nullptr)
        return;
    if(sliderHandleStack.contains(sliderHandle)) {
        qsizetype index = sliderHandleStack.indexOf(sliderHandle);
        sliderHandleStack.remove(index);
    }
    if(sliderHandles.contains(sliderHandle->id))
        sliderHandles.remove(sliderHandle->id);
    emit sliderRemoved(sliderHandle->id);
    delete sliderHandle;
}

void MultiHandleSlider::raiseSliderHandle(SliderHandle * sliderHandle)
{
    if(!sliderHandleStack.contains(sliderHandle))
        return;
    qsizetype index = sliderHandleStack.indexOf(sliderHandle);
    sliderHandleStack.remove(index);
    sliderHandleStack.push(sliderHandle);
    sliderHandle->raise();
}

void MultiHandleSlider::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        if (sliderHandles.size()<1) return;

        for(QUuid id : sliderHandles.keys()) {
            QRect srec = sliderHandles[id]->geometry();
            if (srec.contains(e->pos(), true))
            {
                activeSliderId = id;
                SliderHandle * sliderHandle = sliderHandles[id];

                raiseSliderHandle(sliderHandle);
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

        if(activeSliderValue >=0 && activeSliderValue <=1) {
            //if(orientation == Qt::Vertical)
             //   activeSliderValue = 1 - activeSliderValue;
            //sliderHandles[activeSlider]->value = activeSliderValue;
            sliderHandles[activeSliderId]->setValue(activeSliderValue);
            if (orientation==Qt::Horizontal)
                sliderHandles[activeSliderId]->move(e->pos().x(), 0);
            else
                sliderHandles[activeSliderId]->move(0, e->pos().y());
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
                setValue(activeSliderId, 0);
            }
        }
        else if(activeSliderValue > 1.0) {
            if(activeSliderValue>=1.1) {
                if(sliderHandles.count() > 1)
                removeActiveSlider();
            }
            else {
                setValue(activeSliderId, 1);
            }
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

        if (!index.isNull())
        {
            QColorDialog colorChooseDialog(this);
            colorChooseDialog.setCurrentColor(sliderHandles[index]->getColor());
            if(colorChooseDialog.exec() == QColorDialog::Accepted) {
                QColor selectedColor = colorChooseDialog.selectedColor();
                if(selectedColor.isValid()) {
                    sliderHandles[index]->setColor(selectedColor);
                    emit sliderColorChanged(index, selectedColor);
                }
            }
            else
                return;
        }
        else {
            addSlider(e->pos(), Qt::white);
        }
        emit sliderChanged();
    }
}

qreal MultiHandleSlider::getValueFromPosition(const QPoint &position)
{
    int boundarySpace = getBoundarySpace();
    QRect crec = contentsRect();
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
