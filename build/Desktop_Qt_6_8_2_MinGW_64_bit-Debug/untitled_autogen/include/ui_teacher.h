/********************************************************************************
** Form generated from reading UI file 'teacher.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHER_H
#define UI_TEACHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_teacher
{
public:
    QLabel *label;

    void setupUi(QDialog *teacher)
    {
        if (teacher->objectName().isEmpty())
            teacher->setObjectName("teacher");
        teacher->resize(400, 300);
        label = new QLabel(teacher);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 90, 411, 141));
        QFont font;
        font.setPointSize(72);
        label->setFont(font);

        retranslateUi(teacher);

        QMetaObject::connectSlotsByName(teacher);
    } // setupUi

    void retranslateUi(QDialog *teacher)
    {
        teacher->setWindowTitle(QCoreApplication::translate("teacher", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("teacher", "teacher", nullptr));
    } // retranslateUi

};

namespace Ui {
    class teacher: public Ui_teacher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHER_H
