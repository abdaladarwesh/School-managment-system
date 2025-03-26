#ifndef STUDENT_H
#define STUDENT_H

#include <QDialog>

namespace Ui {
class student;
}

class student : public QDialog
{
    Q_OBJECT

public:
    explicit student(QWidget *parent = nullptr);
    ~student();

private:
    Ui::student *ui;
};

#endif // STUDENT_H
