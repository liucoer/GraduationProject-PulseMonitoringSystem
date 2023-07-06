#include "pwchartview.h"
#include <QValueAxis>
#include <QLineSeries>

void PWChartView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        LEFT_PRESS_F = true;
        beginPoint = event->position();
    }
    if(event->button() == Qt::RightButton)
        RIGHT_PRESS_F = true;
}

void PWChartView::mouseMoveEvent(QMouseEvent *event)
{
    if(LEFT_PRESS_F)
        emit mouseMovePoint(event->position());
}

void PWChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        LEFT_PRESS_F = false;
    if(event->button() == Qt::RightButton)
        RIGHT_PRESS_F = false;
    QLineSeries *series = (QLineSeries *)chart()->series().at(0);
    if((event->button() == Qt::MiddleButton) && (series->count() > 0))
    {
        QValueAxis *axisX = (QValueAxis *)(this->chart()->axes().at(0));
        QValueAxis *axisY = (QValueAxis *)(this->chart()->axes().at(1));
        qreal Xmax = series->at(series->count() - 1).x();
        axisY->setRange(0, 3.4);
        axisX->setRange((Xmax > 3) ? (Xmax - 3) : 0, Xmax);
    }
}

void PWChartView::keyPressEvent(QKeyEvent *event)
{
    QValueAxis *axisX = (QValueAxis *)(this->chart()->axes().at(0));
    QValueAxis *axisY = (QValueAxis *)(this->chart()->axes().at(1));
    QLineSeries *series = (QLineSeries *)chart()->series().at(0);
    switch(event->key())
    {
    case Qt::Key_Plus:
        axisX->setRange(axisX->min() * 1.1, axisX->max() * 0.9);
        axisY->setRange((axisY->min() < 0) ? (axisY->min() * 0.9) : (axisY->min() * 1.1),
                        (axisY->max() < 0) ? (axisY->max() * 1.1) : (axisY->max() * 0.9));
        break;
    case Qt::Key_Minus:
        axisX->setRange(axisX->min() * 0.9, axisX->max() * 1.1);
        axisY->setRange((axisY->min() < 0) ? (axisY->min() * 1.1) : (axisY->min() * 0.9),
                        (axisY->max() < 0) ? (axisY->max() * 0.9) : (axisY->max() * 1.1));
        break;
    case Qt::Key_Left:
        axisX->setRange(axisX->min() + 1, axisX->max() + 1);
        break;
    case Qt::Key::Key_Right:
        if((axisX->min() - 1) > 0)
            axisX->setRange(axisX->min() - 1, axisX->max() - 1);
        else
            axisX->setRange(0, axisX->max() - axisX->min());
        break;
    case Qt::Key_Up:
        axisY->setRange(axisY->min() - 1, axisY->max() - 1);
        break;
    case Qt::Key_Down:
        axisY->setRange(axisY->min() + 1, axisY->max() + 1);
        break;
    case Qt::Key_Home:
        if(series->count() > 0)
        {
            qreal Xmax = series->at(series->count() - 1).x();
            axisY->setRange(0, 3.4);
            axisX->setRange((Xmax > 3) ? (Xmax - 3) : 0, Xmax);
        }
    }
}
void PWChartView::wheelEvent(QWheelEvent *event)
{
    QValueAxis *axisX = (QValueAxis *)(this->chart()->axes().at(0));
    QValueAxis *axisY = (QValueAxis *)(this->chart()->axes().at(1));
    bool isUp = (event->angleDelta().y() > 0);
    if(LEFT_PRESS_F)
    {
        if(isUp)
            axisX->setRange(axisX->min() * 1.01, axisX->max() * 0.99);
        else
            axisX->setRange(axisX->min() * 0.99, axisX->max() * 1.01);
    }
    else if(RIGHT_PRESS_F)
    {
        if(isUp)
            axisY->setRange((axisY->min() < 0) ? (axisY->min() * 0.99) : (axisY->min() * 1.01),
                            (axisY->max() < 0) ? (axisY->max() * 1.01) : (axisY->max() * 0.99));
        else
            axisY->setRange((axisY->min() < 0) ? (axisY->min() * 1.01) : (axisY->min() * 0.99),
                            (axisY->max() < 0) ? (axisY->max() * 0.99) : (axisY->max() * 1.01));
    }
    else
    {
        if(isUp)
        {
            axisX->setRange(axisX->min() * 1.01, axisX->max() * 0.99);
            axisY->setRange((axisY->min() < 0) ? (axisY->min() * 0.99) : (axisY->min() * 1.01),
                            (axisY->max() < 0) ? (axisY->max() * 1.01) : (axisY->max() * 0.99));
        }
        else
        {
            axisX->setRange(axisX->min() * 0.99, axisX->max() * 1.01);
            axisY->setRange((axisY->min() < 0) ? (axisY->min() * 1.01) : (axisY->min() * 0.99),
                            (axisY->max() < 0) ? (axisY->max() * 0.99) : (axisY->max() * 1.01));
        }
    }
}
PWChartView::PWChartView(QWidget *parent): QChartView(parent)
{
    connect(this, &PWChartView::mouseMovePoint, this, &PWChartView::onMouseMovePoint);
}
PWChartView::~PWChartView()
{
}
void PWChartView::onMouseMovePoint(QPointF point)
{
    QPointF movePoint = point - beginPoint ;
    beginPoint = point;
    QValueAxis *axisX = (QValueAxis *)(this->chart()->axes().at(0));
    QValueAxis *axisY = (QValueAxis *)(this->chart()->axes().at(1));
    qreal Xmin = axisX->min() - movePoint.x() * (axisX->max() - axisX->min()) / 700.0f;
    qreal Xmax = axisX->max() - movePoint.x() * (axisX->max() - axisX->min()) / 700.0f;
    if(Xmin > 0)
        axisX->setRange(Xmin, Xmax);
    else
        axisX->setRange(0, Xmax - Xmin);
    axisY->setRange(axisY->min() + movePoint.y() * (axisY->max() - axisY->min()) / 700.0f,
                    axisY->max() + movePoint.y() * (axisY->max() - axisY->min()) / 700.0f);
}
