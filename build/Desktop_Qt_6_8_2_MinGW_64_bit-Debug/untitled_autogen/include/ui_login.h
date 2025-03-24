/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QCheckBox *rememberMeCheckBox;
    QPushButton *loginButton;
    QLabel *label;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName("LoginForm");
        LoginForm->resize(400, 450);
        iconLabel = new QLabel(LoginForm);
        iconLabel->setObjectName("iconLabel");
        iconLabel->setGeometry(QRect(170, 30, 60, 60));
        iconLabel->setPixmap(QPixmap(QString::fromUtf8("icon.png")));
        titleLabel = new QLabel(LoginForm);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(60, 100, 271, 30));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        usernameInput = new QLineEdit(LoginForm);
        usernameInput->setObjectName("usernameInput");
        usernameInput->setGeometry(QRect(75, 150, 250, 40));
        passwordInput = new QLineEdit(LoginForm);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setGeometry(QRect(75, 200, 250, 40));
        passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
        rememberMeCheckBox = new QCheckBox(LoginForm);
        rememberMeCheckBox->setObjectName("rememberMeCheckBox");
        rememberMeCheckBox->setGeometry(QRect(75, 250, 120, 20));
        loginButton = new QPushButton(LoginForm);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(75, 290, 250, 45));
        label = new QLabel(LoginForm);
        label->setObjectName("label");
        label->setGeometry(QRect(180, 50, 49, 16));
        label->setPixmap(QPixmap(QString::fromUtf8("icon.png")));

        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        LoginForm->setWindowTitle(QCoreApplication::translate("login", "School Management System", nullptr));
        LoginForm->setStyleSheet(QCoreApplication::translate("login", "background-color: #f7f6f2; color: black;", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("login", "font-size: 18px; font-weight: bold; color: black;", nullptr));
        titleLabel->setText(QCoreApplication::translate("login", "School Management System", nullptr));
        usernameInput->setStyleSheet(QCoreApplication::translate("login", "border: 1px solid #ccc; border-radius: 5px; padding-left: 10px; color: black;", nullptr));
        usernameInput->setPlaceholderText(QCoreApplication::translate("login", "Username", nullptr));
        passwordInput->setStyleSheet(QCoreApplication::translate("login", "border: 1px solid #ccc; border-radius: 5px; padding-left: 10px; color: black;", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("login", "Password", nullptr));
        rememberMeCheckBox->setStyleSheet(QCoreApplication::translate("login", "color: black;", nullptr));
        rememberMeCheckBox->setText(QCoreApplication::translate("login", "Remember me", nullptr));
        loginButton->setStyleSheet(QCoreApplication::translate("login", "background-color: black; color: white; font-size: 16px; border-radius: 5px;", nullptr));
        loginButton->setText(QCoreApplication::translate("login", "Sign In", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
