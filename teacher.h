#ifndef TEACHER_H
#define TEACHER_H

#include <QDialog>

namespace Ui {
class teacher;
}

class teacher : public QDialog
{
    Q_OBJECT

public:
    explicit teacher(QWidget *parent = nullptr);
    ~teacher();

private:
    Ui::teacher *ui;
};

#endif // TEACHER_H
