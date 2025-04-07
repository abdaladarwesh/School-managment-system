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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sign_in
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacerBottom;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacerLeft;
    QFrame *backgroundFrame;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QSpacerItem *verticalSpacer;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer_2;
    QPushButton *loginButton;
    QSpacerItem *horizontalSpacerRight;
    QSpacerItem *verticalSpacerTop;

    void setupUi(QWidget *sign_in)
    {
        if (sign_in->objectName().isEmpty())
            sign_in->setObjectName("sign_in");
        sign_in->resize(800, 600);
        sign_in->setStyleSheet(QString::fromUtf8("background-color: #f5f7f8;"));
        gridLayout_2 = new QGridLayout(sign_in);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalSpacerBottom = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacerBottom, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalSpacerLeft = new QSpacerItem(100, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacerLeft);

        backgroundFrame = new QFrame(sign_in);
        backgroundFrame->setObjectName("backgroundFrame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(backgroundFrame->sizePolicy().hasHeightForWidth());
        backgroundFrame->setSizePolicy(sizePolicy);
        backgroundFrame->setMinimumSize(QSize(400, 292));
        backgroundFrame->setStyleSheet(QString::fromUtf8("background-color: white; border: 1px solid #ccc; border-radius: 10px;"));
        backgroundFrame->setFrameShape(QFrame::Shape::StyledPanel);
        mainLayout = new QVBoxLayout(backgroundFrame);
        mainLayout->setObjectName("mainLayout");
        titleLabel = new QLabel(backgroundFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font: bold 16pt; color: white; background-color: #2196F3; padding: 10px;"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(titleLabel);

        verticalSpacer = new QSpacerItem(6, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);

        usernameEdit = new QLineEdit(backgroundFrame);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setStyleSheet(QString::fromUtf8("padding: 10px; font-size: 14px; color :black;"));

        mainLayout->addWidget(usernameEdit);

        passwordEdit = new QLineEdit(backgroundFrame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setStyleSheet(QString::fromUtf8("padding: 10px; font-size: 14px; color:black;"));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        mainLayout->addWidget(passwordEdit);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer_2);

        loginButton = new QPushButton(backgroundFrame);
        loginButton->setObjectName("loginButton");
        loginButton->setStyleSheet(QString::fromUtf8("background-color: #2196F3; color: white; padding: 10px; font-weight: bold;"));

        mainLayout->addWidget(loginButton);


        horizontalLayout->addWidget(backgroundFrame);

        horizontalSpacerRight = new QSpacerItem(100, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacerRight);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 1);

        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        verticalSpacerTop = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacerTop, 0, 0, 1, 1);


        retranslateUi(sign_in);

        QMetaObject::connectSlotsByName(sign_in);
    } // setupUi

    void retranslateUi(QWidget *sign_in)
    {
        sign_in->setWindowTitle(QCoreApplication::translate("sign_in", "School Management System - Login", nullptr));
        titleLabel->setText(QCoreApplication::translate("sign_in", "SCHOOL MANAGEMENT SYSTEM", nullptr));
        usernameEdit->setPlaceholderText(QCoreApplication::translate("sign_in", "Username", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("sign_in", "Password", nullptr));
        loginButton->setText(QCoreApplication::translate("sign_in", "LOGIN", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sign_in: public Ui_sign_in {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_IN_H
