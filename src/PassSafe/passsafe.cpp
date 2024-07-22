#include "passsafe.h"
#include "ui_PassSafe.h"
#include <QMainWindow>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QThread>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QClipboard>

PassSafe::PassSafe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PassSafe)
{
    ui->setupUi(this);

    QThread *createPassThread = new QThread;
    QObject::connect(createPassThread, &QThread::started, this, [this]() {
        QString new_password = generateRandomString();

        passwordDialog = new QDialog;
        passwordDialog->setFixedSize(600, 100);
        passwordDialog->setWindowFlag(Qt::FramelessWindowHint);
        QGridLayout *passwordLayout = new QGridLayout(passwordDialog);

        QLabel *text_label = new QLabel("Memorize your new password. It is one-time and will be changed after the next start of the program");
        text_label->setAlignment(Qt::AlignCenter);

        QLabel *password_text = new QLabel(new_password);
        password_text->setAlignment(Qt::AlignCenter);

        QPushButton *copyPasswordButton = new QPushButton("Copy && Close");
        copyPasswordButton->setFixedSize(150, 25);

        passwordLayout->addWidget(text_label, 0, 0, 1, 3);
        passwordLayout->addWidget(password_text, 1, 0, 1, 3);
        passwordLayout->addWidget(copyPasswordButton, 2, 0, 1, 3, Qt::AlignCenter);

        QObject::connect(copyPasswordButton, &QPushButton::clicked, [&]() {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(new_password);
            passwordDialog->close();
        });

        passwordDialog->exec();
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
