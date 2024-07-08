#include "passsafe.h"
#include "ui_PassSafe.h"
#include <QMainWindow>

PassSafe::PassSafe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PassSafe)
{
    ui->setupUi(this);
}

PassSafe::~PassSafe() {};
