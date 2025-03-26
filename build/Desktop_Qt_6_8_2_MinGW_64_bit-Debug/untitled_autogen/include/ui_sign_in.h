/********************************************************************************
** Form generated from reading UI file 'sign_in.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGN_IN_H
#define UI_SIGN_IN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sign_in
{
public:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *signupbutton;

    void setupUi(QWidget *sign_in)
    {
        if (sign_in->objectName().isEmpty())
            sign_in->setObjectName("sign_in");
        sign_in->resize(780, 586);
        sign_in->setStyleSheet(QString::fromUtf8("background-image: url(:/image/bg.jpeg);"));
        usernameLineEdit = new QLineEdit(sign_in);
        usernameLineEdit->setObjectName("usernameLineEdit");
        usernameLineEdit->setGeometry(QRect(260, 340, 381, 41));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        usernameLineEdit->setPalette(palette);
        passwordLineEdit = new QLineEdit(sign_in);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setGeometry(QRect(250, 400, 401, 41));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        passwordLineEdit->setPalette(palette1);
        loginButton = new QPushButton(sign_in);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(250, 500, 401, 51));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        loginButton->setPalette(palette2);
        signupbutton = new QPushButton(sign_in);
        signupbutton->setObjectName("signupbutton");
        signupbutton->setGeometry(QRect(239, 460, 191, 24));

        retranslateUi(sign_in);

        QMetaObject::connectSlotsByName(sign_in);
    } // setupUi

    void retranslateUi(QWidget *sign_in)
    {
        sign_in->setWindowTitle(QCoreApplication::translate("sign_in", "Form", nullptr));
        loginButton->setText(QCoreApplication::translate("sign_in", "signin ", nullptr));
        signupbutton->setText(QCoreApplication::translate("sign_in", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sign_in: public Ui_sign_in {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_IN_H
