/********************************************************************************
** Form generated from reading UI file 'signup.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUP_H
#define UI_SIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_signup
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QHBoxLayout *horizontalLayout;
    QRadioButton *teacher;
    QRadioButton *student;
    QPushButton *signupbutton;

    void setupUi(QWidget *signup)
    {
        if (signup->objectName().isEmpty())
            signup->setObjectName("signup");
        signup->resize(353, 148);
        verticalLayout = new QVBoxLayout(signup);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetNoConstraint);
        usernameLineEdit = new QLineEdit(signup);
        usernameLineEdit->setObjectName("usernameLineEdit");

        verticalLayout->addWidget(usernameLineEdit);

        passwordLineEdit = new QLineEdit(signup);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(passwordLineEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        teacher = new QRadioButton(signup);
        teacher->setObjectName("teacher");

        horizontalLayout->addWidget(teacher);

        student = new QRadioButton(signup);
        student->setObjectName("student");

        horizontalLayout->addWidget(student);


        verticalLayout->addLayout(horizontalLayout);

        signupbutton = new QPushButton(signup);
        signupbutton->setObjectName("signupbutton");

        verticalLayout->addWidget(signupbutton);


        retranslateUi(signup);

        QMetaObject::connectSlotsByName(signup);
    } // setupUi

    void retranslateUi(QWidget *signup)
    {
        usernameLineEdit->setPlaceholderText(QCoreApplication::translate("signup", "Username", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("signup", "Password", nullptr));
        teacher->setText(QCoreApplication::translate("signup", "Teacher", nullptr));
        student->setText(QCoreApplication::translate("signup", "Student", nullptr));
        signupbutton->setText(QCoreApplication::translate("signup", "Sign up", nullptr));
        (void)signup;
    } // retranslateUi

};

namespace Ui {
    class signup: public Ui_signup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUP_H
