#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QValueAxis>
#include <QLineSeries>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <iostream>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPort = new QSerialPort;
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    labState = new QLabel("设备连接状态：未连接");
    labState->setMinimumWidth(200);
    labState->setAlignment(Qt::AlignVCenter);
    ui->statusBar->addWidget(labState);
    createChart1();
    createChart0();
    connect(serialPort, &QSerialPort::errorOccurred, this, &MainWindow::onErrorOccurred);
    on_btnFind_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_F11)
    {
        if(isFullScreen())
            showNormal();
        else
            showFullScreen();
    }
    QWidget::keyPressEvent(ev);
}

void MainWindow::createChart0()
{
    QChartView *chart_view;
    chart_view = ui->chartViewO;
    QChart *chart = new QChart;
    chart->setTheme(QChart::ChartThemeBrownSand);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setContentsMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    axisX = new QValueAxis;
    axisX->setRange(0, 3);
    axisX->setMinorTickCount(9);
    axisX->setTickCount(7);
    QPen pen = axisX->gridLinePen();
    pen.setWidth(3);
    pen.setColor(QColor::fromRgb(255, 255, 255, 128));
    axisX->setGridLinePen(pen);
    chart->addAxis(axisX, Qt::AlignBottom);
    axisY = new QValueAxis;
    axisY->setRange(0, 3.4);
    axisY->setMinorTickCount(10);
    axisY->setTickCount(4);
    axisY->setGridLinePen(pen);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();
    series = new QLineSeries();
    series->setUseOpenGL(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series->setColor(QColor::fromRgb(255, 0, 0));
    chart_view->setChart(chart);
}

void MainWindow::createChart1()
{
    QChartView *chart_view;
    chart_view = ui->chartView3;
    QChart *chart = new QChart;
    chart->setTheme(QChart::ChartThemeBrownSand);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setContentsMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    axisX1 = new QValueAxis;
    axisX1->setRange(0, 3);
    axisX1->setMinorTickCount(9);
    axisX1->setTickCount(7);
    QPen pen = axisX1->gridLinePen();
    pen.setWidth(3);
    pen.setColor(QColor::fromRgb(255, 255, 255, 128));
    axisX1->setGridLinePen(pen);
    chart->addAxis(axisX1, Qt::AlignBottom);
    axisY1 = new QValueAxis;
    axisY1->setRange(0, 3.4);
    axisY1->setMinorTickCount(10);
    axisY1->setTickCount(4);
    axisY1->setGridLinePen(pen);
    chart->addAxis(axisY1, Qt::AlignLeft);
    chart->legend()->hide();
    series1 = new QLineSeries();
    series1->setUseOpenGL(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->addSeries(series1);
    series1->attachAxis(axisX1);
    series1->attachAxis(axisY1);
    series1->setColor(QColor::fromRgb(255, 0, 0));
    chart_view->setChart(chart);
}



void MainWindow::on_btnFind_clicked()
{
    ui->comboBox->clear();
    foreach(const QSerialPortInfo info, QSerialPortInfo::availablePorts())
        ui->comboBox->addItem(info.portName() + info.description());
}

void MainWindow::on_btnOpen_clicked()
{
    serialPort->setPortName(ui->comboBox->currentText().first(4));
    if(serialPort->isOpen())
        labState->setText("设备连接状态：已连接");
    else
    {
        if(serialPort->open(QIODevice::ReadWrite))
        {
            serialPort->clear();
            labState->setText("设备连接状态：已连接");
            if(ui->chkRe->checkState())
                connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::DataReceived);
            connect(this, &MainWindow::newPoint, this, &MainWindow::onNewPointArrived);
        }
    }
}


void MainWindow::on_btnClose_clicked()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
        labState->setText("设备连接状态: 未连接");
        disconnect(serialPort, &QSerialPort::readyRead, this, &MainWindow::DataReceived);
        disconnect(this, &MainWindow::newPoint, this, &MainWindow::onNewPointArrived);
    }
    else
        labState->setText("设备连接状态: 未连接");
}

static QPointF hextoPoint(QByteArray data)
{
    QPointF point;
    union
    {
        unsigned char info[4];
        float f;
    } float_data;
    for(size_t i = 0; i != 4; ++i)
        float_data.info[i] = data.at(i);
    point.setX(float_data.f);
    for(size_t i = 0; i != 4; ++i)
        float_data.info[i] = data.at(i + 4);
    point.setY(float_data.f);
    return point;
}

void MainWindow::DataReceived()
{
    if(serialPort->bytesAvailable())
    {
        QByteArray data = serialPort->readAll();
        if(!data.isEmpty())
        {
            char *temp = data.data();
            for(int i = 0; i != data.size(); i++)
            {
                if(ReadAble)
                {
                    if((temp[i] == END_F) && (DataBuf.size() == 8))
                    {
                        ReadAble = false;
                        emit newPoint(hextoPoint(DataBuf));
                    }
                    else if(DataBuf.size() < 8)
                        DataBuf.append(temp[i]);
                    else
                        ReadAble = false;
                }
                else
                {
                    if(temp[i] == BEGIN_F)
                    {
                        ReadAble = true;
                        DataBuf.clear();
                    }
                }
            }
        }
    }
}

float MainWindow::DataProcess(float val)
{
    const float alpha = 0.086;
    static float last_value = 0.5f;
    last_value = (val * alpha + (1 - alpha) * last_value);
    return last_value;
}
//寻找峰值
bool MainWindow::findPeak(float val)
{
    static float last_value = 0.0f;
    static uint16_t count = 0;
    static uint16_t count1 = 0;
    if((val - last_value <= -0.01) && (count1 == 0))
        ++count;
    else if((val - last_value > 0.01) && (count > 10))
        ++count1;
    else if((val - last_value <= -0.01) && (count1 != 0))
        count = count1 = 0;
    else if((val - last_value > 0.01) && (count <= 10))
        count = 0;
    last_value = val;
    if(count1 > 10)
    {
        count = count1 = 0;
        return true;
    }
    return false;
}
//脉搏频率计算
void MainWindow::pulse_fre(QPointF point)
{
    if(findPeak(point.y()))
    {
        curtime = point.x();
        float fre = 60.0f / (curtime - pretime);
        curfre = fre < 30 ? curfre : fre;
        pretime = curtime;
    }
}

void MainWindow::onNewPointArrived(QPointF point)
{
    beginX = (beginX > 0) ? beginX : point.x();
    point.setX(point.x() - beginX);
    ui->dataEdit->append(QString::asprintf("%f,%f", point.x(), point.y()));
    (point.x() > axisX->max()) ? (axisX->setRange(point.x() - (axisX->max() - axisX->min()), point.x())) : void();
    (point.y() > axisY->max()) ? (axisY->setMax(point.y())) : void();
    (point.y() < axisY->min()) ? (axisY->setMin(point.y())) : void();
    series->append(point);
    point.setY(DataProcess(point.y()));
    pulse_fre(point);
    (point.x() > axisX1->max()) ? (axisX1->setRange(point.x() - (axisX1->max() - axisX1->min()),
                                   point.x())) : void();
    (point.y() > axisY1->max()) ? (axisY1->setMax(point.y())) : void();
    (point.y() < axisY1->min()) ? (axisY1->setMin(point.y())) : void();
    ui->dataEdit_2->append(QString::asprintf("%f,%f", point.x(), point.y()));
    series1->append(point);
    ui->lcdNumber->display(curfre);
}

void MainWindow::onErrorOccurred(QSerialPort::SerialPortError error)
{
    switch(error)
    {
    case QSerialPort::SerialPortError::NoError:
        break;
    case QSerialPort::SerialPortError::UnknownError:
        labState->setText("设备连接状态：未知错误");
        break;
    default:
        break;
    }
}

void MainWindow::on_btnClear_clicked()
{
    series->clear();
    series1->clear();
    ui->dataEdit->clear();
    ui->dataEdit_2->clear();
    axisX->setRange(0, axisX->max() - axisX->min());
    axisX1->setRange(0, axisX1->max() - axisX1->min());
    beginX = -1;
}

void MainWindow::on_chkRe_clicked(bool checked)
{
    if(checked)
    {
        serialPort->clear();
        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::DataReceived);
    }
    else
        disconnect(serialPort, &QSerialPort::readyRead, this, &MainWindow::DataReceived);
}


