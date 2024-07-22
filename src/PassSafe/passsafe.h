#ifndef PASSSAFE_H
#define PASSSAFE_H

#include "ui_PassSafe.h"
#include <QMainWindow>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class PassSafe;
}
QT_END_NAMESPACE

class PassSafe : public QMainWindow
{
    Q_OBJECT
public:
    PassSafe(QWidget *parent = nullptr);
    ~PassSafe();

private:
    QDialog *passwordDialog;
    Ui::PassSafe *ui;

private slots:
    QString generateRandomString();

signals:
};

#endif // PASSSAFE_H
