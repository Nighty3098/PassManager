#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(300, 350);

    ui->user_image->setPixmap(QPixmap(":/resources/user.png").scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->closeButton->setFixedSize(15, 15);
    ui->closeButton->setStyleSheet("QPushButton {"
                                   "    border-color: rgba(0, 0, 0, 0);"
                                   "    background-color: rgba(0, 0, 0, 0);"
                                   "    background-image: url(':/resources/red.png');"
                                   "    background-repeat: no-repeat;"
                                   "}"
                                   "QPushButton:hover {"
                                   "    border-color: rgba(0, 0, 0, 0);"
                                   "    background-image: url(':/resources/redHovered.png');"
                                   "    background-repeat: no-repeat;"
                                   "    background-color: rgba(0, 0, 0, 0);"
                                   "}");

    connect(ui->closeButton, &QPushButton::clicked, this, [this]() { close(); });
    connect(ui->login_button, &QPushButton::clicked, this, [this]() { openPassSafe(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::checkUserPassword() {
    QString password;
    password = ui->user_password->text();

    qDebug() << "User password: " << password;

    return password;
}

void MainWindow::openPassSafe() {
    PassSafeWindow = new PassSafe();

    QString password = checkUserPassword();

    if(password == "qwerty") {
        PassSafeWindow->show();
        this->close();
    }
    else {
        QMessageBox::information(nullptr, "~ PassSafe ~", "PassSafe Error: Wrong password.");
    }
}
