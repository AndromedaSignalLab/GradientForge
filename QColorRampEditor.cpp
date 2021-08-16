/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#include "QColorRampEditor.hpp"
#include <QPainter>
#include <QMouseEvent>
#include "QSlidersWidget.hpp"

// -----------------------------------------------------------
// QColorRampEditor ------------------------------------------
// -----------------------------------------------------------
QColorRampEditor::QColorRampEditor(QWidget* parent, int orientation) : QWidget(parent),
    ori_(orientation),
    activeSlider_(-1), slideUpdate_(false),
    bspace_(5),
    visText_(false), textColor_(Qt::white), textAcc_(1)
{
    if (ori_==Qt::Horizontal)
        setMinimumSize(50,40);
    else
        setMinimumSize(40,50);

    setContentsMargins(0,0,0,0);
    if (ori_==Qt::Horizontal)
        setLayout(new QVBoxLayout());
    else
        setLayout(new QHBoxLayout());
    //layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setContentsMargins(0,0,0,0);

    rampwid_ = new QRampWidget();
    rampwid_->rampeditor_ = this;
    rampwid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    rampwid_->setContentsMargins(0,0,0,0);

    layout()->addWidget(rampwid_);

    slidewid_ = new QSlidersWidget();
    slidewid_->rampEditor = this;
    if (ori_==Qt::Horizontal)
    {
        slidewid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        slidewid_->setFixedHeight(16);
    }
    else
    {
        slidewid_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        slidewid_->setFixedWidth(16);
    }
    slidewid_->setContentsMargins(0,0,0,0);
    layout()->addWidget(slidewid_);

    textwid_ = new QSliderTextWidget();
    textwid_->rampeditor_ = this;
    if (ori_==Qt::Horizontal)
    {
        textwid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        textwid_->setFixedHeight(12);
    }
    else
    {
        textwid_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        textwid_->setFixedWidth(12);
    }
    layout()->addWidget(textwid_);
    textwid_->setVisible(false);

    // init sliders
    QVector<QPair<qreal, QColor> > ramp;
    ramp.push_back(QPair<qreal, QColor>(0.0, Qt::black));
    ramp.push_back(QPair<qreal, QColor>(1.0, Qt::red));
    setRamp(ramp);
}
// -----------------------------------------------------------
QColorRampEditor::~QColorRampEditor()
{
    for (int i=0; i<sliders.size(); i++) delete(sliders[i]);
}
// -----------------------------------------------------------
int QColorRampEditor::getSliderNum()
{
    return sliders.size();
}
// -----------------------------------------------------------
void QColorRampEditor::setSlideUpdate(bool val)
{
    slideUpdate_ = val;
}
// -----------------------------------------------------------
bool QColorRampEditor::colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2)
{
    return a1.first < a2.first;
}

// -----------------------------------------------------------
QVector<QPair<qreal, QColor> > QColorRampEditor::getRamp()
{

    // create output
    QVector<QPair<qreal, QColor> > ret;
    for (int i=0; i<sliders.size(); i++)
    {
        QColor col = sliders[i]->getColor();
        ret.push_back(QPair<qreal, QColor>(sliders[i]->val,col));
    }
    // sort the slider list
    std::sort(ret.begin(), ret.end(), QColorRampEditor::colorRampSort);

    return ret;
}
// -----------------------------------------------------------
QVector<QRgb> QColorRampEditor::getColorTable()
{
    // get ramp and normalize
    QVector<QPair<qreal, QColor> > ramp = getRamp();
    for (int i=0; i<ramp.size(); i++) ramp[i].first = (ramp[i].first - mi_)/(ma_-mi_);

    QVector<QRgb> ctable;
    int index = 0;
    for (int i = 0; i < 256; i++)
    {
        float val = 1.0f*i/255;
        if (val>ramp[index+1].first) index++;
        // linear interpolate color
        QColor cc;
        float d = val - ramp[index].first;
        float dr = ramp[index+1].second.redF() - ramp[index].second.redF();
        float dg = ramp[index+1].second.greenF() - ramp[index].second.greenF();
        float db = ramp[index+1].second.blueF() - ramp[index].second.blueF();
        qreal dd = ramp[index+1].first - ramp[index].first;
        float red = ramp[index].second.redF() +  d*dr/dd;
        float green = ramp[index].second.greenF() +  d*dg/dd;
        float blue = ramp[index].second.blueF() +  d*db/dd;
		if (red<0.0f) red=0;
		if (red>1.0f) red=1;
		if (green<0.0f) green=0;
		if (green>1.0f) green=1;
		if (blue<0.0f) blue=0;
		if (blue>1.0f) blue=1;
        cc.setRedF(red);
        cc.setGreenF(green);
        cc.setBlueF(blue);
        ctable.push_back(cc.rgb());
    }
    return ctable;
}
// -----------------------------------------------------------
void QColorRampEditor::setRamp(QVector<QPair<qreal, QColor> > ramp)
{
    if (ramp.size()<2) return;

    // sort the slider list
    std::sort(ramp.begin(), ramp.end(), QColorRampEditor::colorRampSort);

    for (int i=0; i<sliders.size(); i++) delete(sliders[i]);
    sliders.clear();

    // find min/max
    mi_=ramp.first().first;
    ma_=ramp.last().first;

    // create sliders
    for (int i=0; i<ramp.size(); i++)
    {
        QColorRampEditorSlider* sl = new QColorRampEditorSlider(ori_,ramp[i].second, slidewid_);
        sl->val = ramp[i].first;
        sliders.push_back(sl);
        updatePos(sl);
        sl->show();
    }

    //emit rampChanged();
    slidewid_->update();
}

void QColorRampEditor::setMappingTextVisualize(bool vis)
{
    visText_ = vis;
    textwid_->setVisible(visText_);
    update();
}

void QColorRampEditor::setMappingTextColor(QColor col)
{
    textColor_ = col;
    update();
}

void QColorRampEditor::setMappingTextAccuracy(int acc)
{
    textAcc_ = acc;
    update();
}

qreal QColorRampEditor::updateValue(QColorRampEditorSlider* sl)
{
    QRect crec = slidewid_->contentsRect();
    if (ori_==Qt::Horizontal)
    {
        crec.adjust(bspace_,0,-bspace_,0);
        sl->val = mi_ + (1.0*sl->pos().x()-bspace_)/crec.width()*(ma_-mi_);
    }
    else
    {
        crec.adjust(0,bspace_,0,-bspace_);
        sl->val = mi_ + (1.0*sl->pos().y()-bspace_)/crec.height()*(ma_-mi_);
    }
    return sl->val;
}

int QColorRampEditor::updatePos(QColorRampEditorSlider* sl)
{
    QRect crec = slidewid_->contentsRect();
    qreal pos;
    if (ori_==Qt::Horizontal)
    {
        crec.adjust(bspace_,0,-bspace_,0);
        pos = (sl->val- mi_)/(ma_-mi_)*crec.width();
        pos -= sl->width()/2;
        pos += bspace_;
        sl->move(pos,0);
    }
    else
    {
        crec.adjust(0, bspace_,0,-bspace_);
        pos = (sl->val- mi_)/(ma_-mi_)*crec.height();
        pos -= sl->height()/2;
        pos += bspace_;
        sl->move(0,pos);
    }
    return pos;
}

// -----------------------------------------------------------
void QColorRampEditor::setSliderColor(int index, QColor col)
{
    if (index<0 || index>=sliders.size()) return;
    sliders[index]->setColor(col);
    emit rampChanged();
}

// -----------------------------------------------------------
void QColorRampEditor::resizeEvent (QResizeEvent* e)
{
    for (int i=0; i<sliders.size(); i++)
    {
        QColorRampEditorSlider* sl = sliders[i];
        updatePos(sl);
    }
}
// -----------------------------------------------------------
void QColorRampEditor::mousePressEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        QRect crec = contentsRect();
        if (ori_==Qt::Horizontal)
        {
            crec.adjust(bspace_,0,-bspace_,0);
            if (crec.contains(e->pos(), true )) // test mouse is in ramp
            {
                QColorRampEditorSlider* sl = new QColorRampEditorSlider(ori_, Qt::white, slidewid_);
                sliders.push_back(sl);
                sl->move(e->pos().x(),0);
                updateValue(sl);
                sl->show();
                std::sort(sliders.begin(), sliders.end(), QSlidersWidget::SliderSort);
                updateRamp();
            }
        }
        else
        {
            crec.adjust(0,bspace_,0,-bspace_);
            if (crec.contains(e->pos(), true )) // test mouse is in ramp
            {
                QColorRampEditorSlider* sl = new QColorRampEditorSlider(ori_, Qt::white, slidewid_);
                sliders.push_back(sl);
                sl->move(0,e->pos().y());
                updateValue(sl);
                sl->show();
                std::sort(sliders.begin(), sliders.end(), QSlidersWidget::SliderSort);
                updateRamp();
            }
        }
    }
}

void QColorRampEditor::updateRamp()
{
    rampwid_->update();
    if (textwid_->isVisible()) textwid_->update();
    emit rampChanged();
}


// -----------------------------------------------------------
// QRampWidget -----------------------------------------------
// -----------------------------------------------------------

QRampWidget::QRampWidget(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setContentsMargins(0,0,0,0);
    setMinimumHeight(5);
    setMinimumWidth(5);
}

void QRampWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setPen(Qt::black);

    QLinearGradient grad;
    QRect crec = contentsRect();
    if (rampeditor_->ori_==Qt::Horizontal)
    {
        crec.adjust(rampeditor_->bspace_,0,-rampeditor_->bspace_,-1);
        grad = QLinearGradient( 0, 0, crec.width()-1, 0);
    }
    else
    {
        crec.adjust(0,rampeditor_->bspace_,-1,-rampeditor_->bspace_);
        grad = QLinearGradient( 0, 0, 0, crec.height()-1);
    }

    for (int i=0; i<rampeditor_->sliders.size(); i++)
    {
        qreal nval = (rampeditor_->sliders[i]->val - rampeditor_->mi_)/(rampeditor_->ma_-rampeditor_->mi_);
        grad.setColorAt(nval, rampeditor_->sliders[i]->getColor());
    }

    painter.fillRect( crec, grad);
    painter.drawRect(crec);

    QWidget::paintEvent(e);
}

// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
QColorRampEditorSlider::QColorRampEditorSlider(int orientation, QColor col, QWidget* parent) : QWidget(parent),
    ori_(orientation), color_(col)
{
    if (ori_==Qt::Horizontal)
        setFixedSize(9, 16);
    else
        setFixedSize(16,9);
}
// -----------------------------------------------------------
void QColorRampEditorSlider::setColor(QColor col)
{
    color_ = col;
}
// -----------------------------------------------------------
QColor QColorRampEditorSlider::getColor()
{
    return color_;
}

// -----------------------------------------------------------
void QColorRampEditorSlider::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(color_);
    if (ori_==Qt::Horizontal)
    {
        QRect rec(0,7,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(0,7) << QPoint(4,0) << QPoint(8,7) << QPoint(8, 15) << QPoint(0, 15);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }
    else
    {
        QRect rec(7,0,8,8);
        painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(7,0) << QPoint(0,4) << QPoint(7,8);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }

}


// -----------------------------------------------------------
// QSliderTextWidget -----------------------------------------
// -----------------------------------------------------------
QSliderTextWidget::QSliderTextWidget(QWidget* parent) : QWidget(parent)
{
    setContentsMargins(0,0,0,0);
}

void QSliderTextWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);

    QFont f = painter.font();
    f.setPixelSize(8);
    painter.setFont(f);

    painter.setPen(rampeditor_->textColor_);
    painter.setBrush(rampeditor_->textColor_);

    QFontMetrics fm(f);

    // adjust size for vertical
    if (rampeditor_->ori_==Qt::Vertical)
    {
        {
            QString txt1 = QString::number(rampeditor_->sliders.first()->val, 'f', rampeditor_->textAcc_);
            QString txt2 = QString::number(rampeditor_->sliders.last()->val, 'f', rampeditor_->textAcc_);
            int w = fm.boundingRect(txt1).width() + 4;
            if (w>this->width()) setFixedWidth(w);
            w = fm.boundingRect(txt2).width() + 4;
            if (w>this->width()) setFixedWidth(w);
        }
        // draw the text for vertical orientation
        for (int i=0; i<rampeditor_->sliders.size(); i++)
        {
            int pos = rampeditor_->sliders[i]->pos().y();
            qreal val = rampeditor_->sliders[i]->val;
            QString txt = QString::number(val, 'f', rampeditor_->textAcc_);
            painter.drawText(2, pos + rampeditor_->sliders[i]->height(), txt);
        }
    }
    else // draw the text for horizontal orientation
    {
        for (int i=0; i<rampeditor_->sliders.size(); i++)
        {
            int pos = rampeditor_->sliders[i]->pos().x();
            qreal val = rampeditor_->sliders[i]->val;
            QString txt = QString::number(val, 'f', rampeditor_->textAcc_);
            if ((pos+fm.boundingRect(txt).width())>width()) pos += -fm.boundingRect(txt).width() + rampeditor_->sliders[i]->width();
            painter.drawText(pos, 2+fm.height(), txt);
        }
    }

    QWidget::paintEvent(e);
}

