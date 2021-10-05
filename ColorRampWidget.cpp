#include "ColorRampWidget.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <algorithm>
#include "MathUtil.hpp"
#include "ColorUtil.hpp"

ColorRampWidget::ColorRampWidget(Qt::Orientation orientation, QWidget* parent, int boundarySpace) : QWidget(parent) {
    this->orientation = orientation;
    this->boundarySpace = boundarySpace;
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setContentsMargins(0,0,0,0);
    setMinimumHeight(5);
    setMinimumWidth(5);
}

QColor ColorRampWidget::getColor(qreal key) const
{
    return ColorUtil::getColor(key, colorRamp);
}

void ColorRampWidget::onColorRampChanged(ColorRamp colorRamp)
{
    this->colorRamp = colorRamp;
    update();
}

void ColorRampWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        QRect crec = contentsRect();
        //crec.adjust(bspace_,0,-bspace_,0);
        QColor colorJustClicked;
        qreal normalizedValue;
        if (crec.contains(e->pos(), true )) // test mouse is in ramp
        {
            if(orientation == Qt::Horizontal) {
                crec.adjust(boundarySpace-1, 0, -boundarySpace+1, 0);
            }
            else {
                crec.adjust(0, boundarySpace-1, 0, -boundarySpace+1);
            }
            normalizedValue = MathUtil::getNormalizedValue(e->pos(), crec, boundarySpace, orientation);
            colorJustClicked = ColorUtil::getColor(normalizedValue, colorRamp);
            emit colorClicked(normalizedValue, colorJustClicked);
        }
    }
}

void ColorRampWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    painter.setPen(Qt::black);

    QLinearGradient grad;
    QRect crec = contentsRect();
    if (orientation==Qt::Horizontal)
    {
        //crec.adjust(rampeditor_->slidewid_->getBoundSpace(),0,-rampeditor_->slidewid_->getBoundSpace(),-1);
        crec.adjust(boundarySpace-1, 0, -boundarySpace+1, 0);
        grad = QLinearGradient( 0, 0, crec.width()-1, 0);
    }
    else
    {
        //crec.adjust(0,rampeditor_->slidewid_->getBoundSpace(),-1,-rampeditor_->slidewid_->getBoundSpace());
        crec.adjust(0, boundarySpace-1, 0, -boundarySpace+1);
        grad = QLinearGradient( 0, 0, 0, crec.height()-1);
    }

    for (int i=0; i<colorRamp.size(); i++)
    {
        //qreal nval = rampeditor_->slidewid_->getNormalizedValue(colorRamp[i].first);
        grad.setColorAt(colorRamp[i].first, colorRamp[i].second);
    }

    /*for (int i=0; i<rampeditor_->sliders.size(); i++)
    {
        qreal nval = (rampeditor_->sliders[i]->value - rampeditor_->mi_)/(rampeditor_->ma_-rampeditor_->mi_);
        grad.setColorAt(nval, rampeditor_->sliders[i]->getColor());
    }*/

    painter.fillRect( crec, grad);
    painter.drawRect(crec);

    QWidget::paintEvent(e);
}


