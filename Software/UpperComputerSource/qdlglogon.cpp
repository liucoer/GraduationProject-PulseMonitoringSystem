#include "qdlglogon.h"
#include "ui_qdlglogon.h"
#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>
#include "qdlglogin.h"

qdlglogon::qdlglogon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qdlglogon)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

qdlglogon::~qdlglogon()
{
    delete ui;
}

void qdlglogon::on_btnRegist_clicked()
{
    QString userid = ui->editUser->text();
    QString pwsd = ui->editPswd->text();
    QString surepwsd = ui->editRePswd->text();
    if(surepwsd == pwsd)
    {
        QString path = "./userpin";
        QFile userFile(path);
        userFile.open(QFile::Append);
        QTextStream dataStrem(&userFile);
        QByteArray btArray;
        QByteArray temp = pwsd.toLatin1();
        btArray.append(temp);
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(btArray);
        QByteArray resultArray = hash.result();
        QString md5 = resultArray.toHex();
        dataStrem << userid << "_" << md5 + "\n";
        userFile.close();
        QMessageBox::information(this, "注册", "注册成功！");
        this->close();
    }
    else
        QMessageBox::information(this, "注册", "密码输入不一致！");
}

