#include "student.h"
#include "ui_student.h"

student::student(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::student)
{
    ui->setupUi(this);
}

student::~student()
{
    delete ui;
}
