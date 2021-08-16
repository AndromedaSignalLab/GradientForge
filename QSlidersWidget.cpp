#include "QSlidersWidget.hpp"

// -----------------------------------------------------------
// QSlidersWidget --------------------------------------------
// -----------------------------------------------------------
QSlidersWidget::QSlidersWidget(QWidget* parent) : QWidget(parent),
    activeSlider(-1)
{
    setContentsMargins(0,0,0,0);
    colorChooseDialog = new QColorDialog();
}

QSlidersWidget::~QSlidersWidget()
{
   if(colorChooseDialog)
       delete(colorChooseDialog);
}

// -----------------------------------------------------------
void QSlidersWidget::setColorChoose(QColorDialog* coldlg)
{
    colorChooseDialog = coldlg;
}

// -----------------------------------------------------------
void QSlidersWidget::mousePressEvent(QMouseEvent* e)
{

    if (e->button()== Qt::LeftButton)
    {
        if (rampEditor->sliders.size()<2) return;
        for (int i=1; i<rampEditor->sliders.size()-1; i++)
        {
            QRect srec = rampEditor->sliders[i]->geometry();
            if (srec.contains(e->pos(), true ))
            {
                activeSlider = i;
                break;
            }
        }
    }
}
// -----------------------------------------------------------
void QSlidersWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (activeSlider>=0)
    {
        QRect crec = contentsRect();

        qreal pos;
        if (rampEditor->ori_==Qt::Horizontal)
        {
            crec.adjust(rampEditor->bspace_,0,-rampEditor->bspace_,0);
            pos = 1.0*(e->pos().x()-rampEditor->bspace_)/(crec.width());
        }
        else
        {
            crec.adjust(0,rampEditor->bspace_,0,-rampEditor->bspace_);
            pos = 1.0*(e->pos().y()-rampEditor->bspace_)/(crec.height());
        }

        if (pos<0.0 || pos>1.0)
        {
            delete(rampEditor->sliders[activeSlider]);
            rampEditor->sliders.removeAt(activeSlider);
            activeSlider = -1;
            //rampeditor_->updateRamp();
        }
        else
        {
            if (rampEditor->ori_==Qt::Horizontal)
                rampEditor->sliders[activeSlider]->move(e->pos().x() - 4, 0);
            else
                rampEditor->sliders[activeSlider]->move(0,e->pos().y() - 4);

            rampEditor->updateValue(rampEditor->sliders[activeSlider]);
            //std::sort(rampEditor->sliders.begin(), rampEditor->sliders.end(), SliderSort); //Enable to make sliders can't pass each other
            //if (rampeditor_->slideUpdate_) rampeditor_->updateRamp();
        }
        update();
        rampEditor->updateRamp();
    }
}
// -----------------------------------------------------------
void QSlidersWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        activeSlider = -1;
        rampEditor->updateRamp();
    }
}
// -----------------------------------------------------------
void QSlidersWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button()== Qt::LeftButton)
    {
        int index = -1;
        for (int i=0; i<rampEditor->sliders.size(); i++)
        {
            QRect srec = rampEditor->sliders[i]->geometry();
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
                rampEditor->sliders[index]->setColor(colorChooseDialog->selectedColor());
                rampEditor->updateRamp();
            }
        }
    }
}


bool QSlidersWidget::SliderSort(const QColorRampEditorSlider* a1, const QColorRampEditorSlider* a2)
{
    return a1->val < a2->val;
}
