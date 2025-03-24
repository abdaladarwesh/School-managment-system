/********************************************************************************
** Form generated from reading UI file 'student.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STUDENT_H
#define UI_STUDENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_student
{
public:
    QLabel *label;

    void setupUi(QDialog *student)
    {
        if (student->objectName().isEmpty())
            student->setObjectName("student");
        student->resize(400, 300);
        label = new QLabel(student);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 90, 371, 121));
        QFont font;
        font.setPointSize(72);
        label->setFont(font);

        retranslateUi(student);

        QMetaObject::connectSlotsByName(student);
    } // setupUi

    void retranslateUi(QDialog *student)
    {
        student->setWindowTitle(QCoreApplication::translate("student", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("student", "student", nullptr));
    } // retranslateUi

};

namespace Ui {
    class student: public Ui_student {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STUDENT_H
