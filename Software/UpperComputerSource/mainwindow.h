#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSerialPort>
#include <QMainWindow>
#include <QLabel>
#include <QLineSeries>
#include <QValueAxis>

const char BEGIN_F = 0x0f;
const char END_F = 0xf0;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void keyPressEvent(QKeyEvent *ev);
private slots:

    void on_btnFind_clicked();
    void on_btnOpen_clicked();
    void on_btnClose_clicked();
    void DataReceived();
    float DataProcess(float val);
    bool findPeak(float val);
    void pulse_fre(QPointF point);
    void onNewPointArrived(QPointF point);
    void onErrorOccurred(QSerialPort::SerialPortError error);
    void on_btnClear_clicked();
    void on_chkRe_clicked(bool checked);

private:
    QLabel *labState;
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    void createChart0();
    void createChart1();
    QString curuser;
    bool ReadAble = 0;
    QByteArray DataBuf;
    qreal t = 0;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *axisX1;
    QValueAxis *axisY1;
    QLineSeries *series;
    QLineSeries *series1;
    qreal beginX = -1;
    float curtime;
    float pretime = 0;
    uint8_t curfre = 00;
signals:
    void newPoint(QPointF);
};
#endif // MAINWINDOW_H
