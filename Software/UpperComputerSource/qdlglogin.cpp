#include "qdlglogin.h"
#include "ui_qdlglogin.h"
#include <QSettings>
#include <QByteArray>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QMouseEvent>
#include "qdlglogon.h"
#include <QFile>

QDlgLogin::QDlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDlgLogin)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::SplashScreen);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

QDlgLogin::~QDlgLogin()
{
    delete ui;
}

QString QDlgLogin::encrypt(const QString &str)
{
    QByteArray btArray;
    QByteArray temp = str.toLatin1();
    btArray.append(temp);
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(btArray);
    QByteArray resultArray = hash.result();
    QString md5 = resultArray.toHex();
    return md5;
}

void QDlgLogin::on_btnLogin_clicked()
{
    m_user = ui->editUser->text().trimmed();
    m_pswd = ui->editPswd->text().trimmed();
    QString encrptPSWD = encrypt(m_pswd);
    QFile readFile("./userpin");
    readFile.open(QFile::ReadOnly);
    QTextStream readStream(&readFile);
    bool hasUser = false;
    while(!readStream.atEnd())
    {
        QString str = readStream.readLine();
        if(str.startsWith(m_user))
        {
            hasUser = true;
            if((m_user + "_" + encrptPSWD) == str)
            {
                this->accept();
                break;
            }
            else if((++m_tryCount) < 3)
            {
                QMessageBox::warning(this, "错误提示", "密码错误,请重试！");
                break;
            }
        }
    }
    readFile.close();
    if((!hasUser) && (++m_tryCount < 3))
        QMessageBox::warning(this, "错误提示", "用户不存在,请重试！");
    if(m_tryCount >= 3)
    {
        QMessageBox::critical(this, "错误", "输入错误次数太多，强行退出！");
        this->reject();
    }
}
void QDlgLogin::on_btnLogon_clicked()
{
    qdlglogon *dlgLogon = new qdlglogon;
    dlgLogon->exec();
}
