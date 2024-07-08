#include "passsafe.h"
#include "ui_PassSafe.h"
#include <QMainWindow>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QThread>


PassSafe::PassSafe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PassSafe)
{
    ui->setupUi(this);

    QThread *createPassThread = new QThread;
    QObject::connect(createPassThread, &QThread::started, this, [this]() {
        QString new_password = generateRandomString();

        QMessageBox::information(nullptr, "~ NEW PASSWORD ~", "Memorize your new password. It is one-time and will be changed after the next start of the program: " + new_password);
    });
    createPassThread->start();
}

PassSafe::~PassSafe() {};


QString PassSafe::generateRandomString() {
    int length = QRandomGenerator::global()->bounded(8, 14);
    QString result;

    for (int i = 0; i < length; ++i) {
        int type = QRandomGenerator::global()->bounded(2);
        if (type == 0) {
            char c = QRandomGenerator::global()->bounded('z' - 'a' + 1) + 'a';
            result += c;
        } else {
            char c = QRandomGenerator::global()->bounded(10) + '0';
            result += c;
        }
    }

    return result;
}
