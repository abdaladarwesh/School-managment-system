#include "teacher.h"
#include "ui_teacher.h"

teacher::teacher(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::teacher)
{
    ui->setupUi(this);
}

teacher::~teacher()
{
    delete ui;
}
