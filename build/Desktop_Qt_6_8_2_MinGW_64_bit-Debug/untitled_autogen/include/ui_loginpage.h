/********************************************************************************
** Form generated from reading UI file 'loginpage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGE_H
#define UI_LOGINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginPage
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *signupbutton;
    QPushButton *loginButton;

    void setupUi(QWidget *LoginPage)
    {
        if (LoginPage->objectName().isEmpty())
            LoginPage->setObjectName("LoginPage");
        LoginPage->resize(350, 187);
        verticalLayout = new QVBoxLayout(LoginPage);
        verticalLayout->setObjectName("verticalLayout");
        usernameLineEdit = new QLineEdit(LoginPage);
        usernameLineEdit->setObjectName("usernameLineEdit");

        verticalLayout->addWidget(usernameLineEdit);

        passwordLineEdit = new QLineEdit(LoginPage);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(passwordLineEdit);

        signupbutton = new QPushButton(LoginPage);
        signupbutton->setObjectName("signupbutton");

        verticalLayout->addWidget(signupbutton);

        loginButton = new QPushButton(LoginPage);
        loginButton->setObjectName("loginButton");

        verticalLayout->addWidget(loginButton);


        retranslateUi(LoginPage);

        QMetaObject::connectSlotsByName(LoginPage);
    } // setupUi

    void retranslateUi(QWidget *LoginPage)
    {
        usernameLineEdit->setPlaceholderText(QCoreApplication::translate("LoginPage", "Username", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("LoginPage", "Password", nullptr));
        signupbutton->setText(QCoreApplication::translate("LoginPage", "Sign up", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginPage", "Login", nullptr));
        (void)LoginPage;
    } // retranslateUi

};

namespace Ui {
    class LoginPage: public Ui_LoginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGE_H
