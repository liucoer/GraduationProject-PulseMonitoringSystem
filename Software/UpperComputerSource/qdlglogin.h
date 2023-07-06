#ifndef QDLGLOGIN_H
#define QDLGLOGIN_H

#include <QDialog>

namespace Ui
{
    class QDlgLogin;
}

class QDlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit QDlgLogin(QWidget *parent = nullptr);
    ~QDlgLogin();

private:
    Ui::QDlgLogin *ui;
    QPoint m_lastPos;
    QString m_user;
    QString m_pswd;
    int m_tryCount = 0;
    QString encrypt(const QString &str);
private slots:
    void on_btnLogin_clicked();
    void on_btnLogon_clicked();
};

#endif // QDLGLOGIN_H
