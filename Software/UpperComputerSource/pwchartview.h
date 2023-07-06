#ifndef PWCHARTVIEW_H
#define PWCHARTVIEW_H

#include <QChartView>
class PWChartView : public QChartView
{
    Q_OBJECT
private:
    QPointF beginPoint;
    bool LEFT_PRESS_F = false;
    bool RIGHT_PRESS_F = false;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
public:
    explicit PWChartView(QWidget *parent = nullptr);
    ~PWChartView();
signals:
    void mouseMovePoint(QPointF point);
private slots:

    void onMouseMovePoint(QPointF point);
};

#endif // PWCHARTVIEW_H
