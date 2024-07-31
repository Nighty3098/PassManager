#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QPushButton>
#include <QMainWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(400, 600);

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
    connect(ui->showPasswordButton, &QPushButton::clicked, this, [this]() { showPasswordInput(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::checkUserPassword()
{
    QString password;
    password = ui->user_password->text();

    qDebug() << "User key: " << password;

    return password;
}

void MainWindow::openPassSafe()
{
    QString password = checkUserPassword();

    if (password == "qwerty") {
        PassSafeWindow = new PassSafe();
        PassSafeWindow->show();
        this->close();
    } else {
        QFont systemFont = QApplication::font();
        int fontSize = systemFont.pointSize();
        systemFont.setPointSize(fontSize);

        QDialog* errorDialog = new QDialog;
        errorDialog->setWindowFlag(Qt::FramelessWindowHint);
        errorDialog->setFixedSize(200, 200);
        QGridLayout* passwordLayout = new QGridLayout(errorDialog);

        QLabel* label = new QLabel("PassSafe Error: Wrong key");
        label->setAlignment(Qt::AlignCenter);
        label->setFont(systemFont);

        QPushButton* closeDialogButton = new QPushButton("close");
        closeDialogButton->setFixedSize(150, 25);
        closeDialogButton->setFont(systemFont);
        connect(closeDialogButton, &QPushButton::clicked, errorDialog, [errorDialog]() { errorDialog->close(); });

        passwordLayout->addWidget(label);
        passwordLayout->addWidget(closeDialogButton);

        errorDialog->exec();
    }
}

void MainWindow::showPasswordInput()
{
    if (ui->user_password->echoMode() == QLineEdit::Password) {
        ui->user_password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->user_password->setEchoMode(QLineEdit::Password);
    }
}
