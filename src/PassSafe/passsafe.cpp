#include "passsafe.h"
#include "ui_PassSafe.h"
#include <QMainWindow>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QThread>
#include <QMenu>
#include <QLabel>
#include <QShortcut>
#include <QGridLayout>
#include <QPushButton>
#include <QClipboard>

PassSafe::PassSafe(QWidget* parent) : QMainWindow(parent), ui(new Ui::PassSafe)
{
    ui->setupUi(this);

    QThread* createPassThread = new QThread;
    QObject::connect(createPassThread, &QThread::started, this, [this]() {
        QString new_password = generateRandomString();

        passwordDialog = new QDialog;
        passwordDialog->setFixedSize(600, 100);
        passwordDialog->setWindowFlag(Qt::FramelessWindowHint);
        QGridLayout* passwordLayout = new QGridLayout(passwordDialog);

        QLabel* text_label = new QLabel("Memorize your new key. It is one-time and will be changed after the next start of the program");
        text_label->setAlignment(Qt::AlignCenter);

        QLabel* password_text = new QLabel(new_password);
        password_text->setAlignment(Qt::AlignCenter);

        QPushButton* copyPasswordButton = new QPushButton("Copy && Close");
        copyPasswordButton->setFixedSize(150, 25);

        passwordLayout->addWidget(text_label, 0, 0, 1, 3);
        passwordLayout->addWidget(password_text, 1, 0, 1, 3);
        passwordLayout->addWidget(copyPasswordButton, 2, 0, 1, 3, Qt::AlignCenter);

        QObject::connect(copyPasswordButton, &QPushButton::clicked, [&]() {
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setText(new_password);
            passwordDialog->close();
        });

        passwordDialog->exec();
    });
    createPassThread->start();

    ui->listOfData->setContextMenuPolicy(Qt::CustomContextMenu);

    QShortcut* addDataAction = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(addDataAction, &QShortcut::activated, this, [this]() { addData(); });

    QShortcut* deleteDataAction = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(deleteDataAction, &QShortcut::activated, this, [this]() { deleteItem(); });

    QObject::connect(ui->addData, &QPushButton::clicked, this, [this]() { addData(); });
    QObject::connect(ui->copySiteBtn, &QPushButton::clicked, this, [this]() { copyData("site"); });
    QObject::connect(ui->copyPassBtn, &QPushButton::clicked, this, [this]() { copyData("password"); });
}
PassSafe::~PassSafe() {};

QString PassSafe::generateRandomString()
{
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

bool PassSafe::checkForData(QString data)
{
    bool found = false;

    for (int i = 0; i < ui->listOfData->count(); ++i) {
        QListWidgetItem* item = ui->listOfData->item(i);
        if (item->text() == data) {
            found = true;
            break;
        }
    }

    if (found) {
        return true;
    } else {
        return false;
    }
}

void PassSafe::loadPasswordsDB() { }
void PassSafe::decrypt_data() { }
void PassSafe::encrypt_data() { }

void PassSafe::addData()
{
    QString site = ui->siteData->text();
    QString password = ui->passwordData->text();

    QString data = site + " | " + password;

    if (!checkForData(data)) {
        ui->listOfData->addItem(data);

        qDebug() << "site: " << site << " password: " << password;

        ui->siteData->clear();
        ui->passwordData->clear();
    } else {
        QMessageBox(QMessageBox::Information, "PassSafe", "The data is already in the database").setWindowFlag(Qt::FramelessWindowHint);
    }
}

void PassSafe::copyData(QString dataType)
{
    QClipboard* clipboard = QApplication::clipboard();

    QListWidgetItem* selectedItem = ui->listOfData->currentItem();
    QString selectedText = selectedItem->text();
    QStringList parts = selectedText.split(" | ");

    if (dataType == "password") {
        qDebug() << "copied password: " << parts[1];

        clipboard->setText(parts[1]);
    }
    if (dataType == "site") {
        qDebug() << "copied site: " << parts[0];

        clipboard->setText(parts[0]);
    } else {
    }
}

void PassSafe::deleteItem()
{
    delete ui->listOfData->takeItem(ui->listOfData->row(ui->listOfData->currentItem()));
}

void PassSafe::editData() { }

void PassSafe::on_listOfData_customContextMenuRequested(const QPoint& pos)
{
    QPoint item = ui->listOfData->mapToGlobal(pos);
    QMenu* submenu = new QMenu;

    submenu->addAction("Copy site");
    submenu->addAction("Copy password");
    submenu->addSeparator();
    submenu->addAction("Delete");
    submenu->addAction("Edit");

    QAction* rightClickItem = submenu->exec(item);
    if (rightClickItem && rightClickItem->text().contains("Copy site")) {
        copyData("site");
    }
    if (rightClickItem && rightClickItem->text().contains("Copy password")) {
        copyData("password");
    }
    if (rightClickItem && rightClickItem->text().contains("Delete")) {
        deleteItem();
    }
    if (rightClickItem && rightClickItem->text().contains("Edit")) {
        editData();
    } else {
    }
}
