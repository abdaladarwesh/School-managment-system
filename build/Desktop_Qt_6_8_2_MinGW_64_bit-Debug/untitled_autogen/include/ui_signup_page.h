/********************************************************************************
** Form generated from reading UI file 'signup_page.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUP_PAGE_H
#define UI_SIGNUP_PAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_signup_page
{
public:
    QLabel *label;
    QLineEdit *usernameLineEdit;
    QComboBox *comboBox;
    QPushButton *signupbutton;
    QLineEdit *passwordLineEdit;

    void setupUi(QWidget *signup_page)
    {
        if (signup_page->objectName().isEmpty())
            signup_page->setObjectName("signup_page");
        signup_page->resize(866, 585);
        signup_page->setStyleSheet(QString::fromUtf8("background-image: url(:/image/P:\\coding\\loginform\\loginform\\bg.jpeg);"));
        label = new QLabel(signup_page);
        label->setObjectName("label");
        label->setGeometry(QRect(350, 10, 151, 71));
        QPalette palette;
        label->setPalette(palette);
        QFont font;
        font.setFamilies({QString::fromUtf8("Bauhaus 93")});
        font.setPointSize(28);
        font.setBold(false);
        font.setItalic(false);
        label->setFont(font);
        usernameLineEdit = new QLineEdit(signup_page);
        usernameLineEdit->setObjectName("usernameLineEdit");
        usernameLineEdit->setGeometry(QRect(220, 300, 371, 51));
        QPalette palette1;
        QBrush brush(QColor(7, 7, 7, 255));
        brush.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        usernameLineEdit->setPalette(palette1);
        comboBox = new QComboBox(signup_page);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::UserAvailable));
        comboBox->addItem(icon, QString());
        comboBox->addItem(icon, QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(220, 410, 251, 28));
        QPalette palette2;
        comboBox->setPalette(palette2);
        signupbutton = new QPushButton(signup_page);
        signupbutton->setObjectName("signupbutton");
        signupbutton->setGeometry(QRect(220, 448, 361, 51));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        signupbutton->setPalette(palette3);
        passwordLineEdit = new QLineEdit(signup_page);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setGeometry(QRect(220, 350, 371, 51));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        passwordLineEdit->setPalette(palette4);

        retranslateUi(signup_page);

        QMetaObject::connectSlotsByName(signup_page);
    } // setupUi

    void retranslateUi(QWidget *signup_page)
    {
        signup_page->setWindowTitle(QCoreApplication::translate("signup_page", "Form", nullptr));
        label->setText(QCoreApplication::translate("signup_page", "HELLO", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("signup_page", "Teacher", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("signup_page", "Student", nullptr));

        signupbutton->setText(QCoreApplication::translate("signup_page", "signup ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class signup_page: public Ui_signup_page {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUP_PAGE_H
