#ifndef QDLGLOGON_H
#define QDLGLOGON_H

#include <QDialog>

namespace Ui
{
    class qdlglogon;
}

class qdlglogon : public QDialog
{
    Q_OBJECT

public:
    explicit qdlglogon(QWidget *parent = nullptr);
    ~qdlglogon();

private slots:
    void on_btnRegist_clicked();
    friend QString encrypt(const QString &str);
private:
    Ui::qdlglogon *ui;
};

#endif // QDLGLOGON_H
