#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QSqlDatabase>
#include "signup.h"
#include <QString>
#include "teacher.h"
#include "student.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginPage; }
QT_END_NAMESPACE

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private slots:
    void onLoginButtonClicked();
    void onSignupbuttonClicked();
    QString checkButtonState();


private:
    Ui::LoginPage *ui;
    QSqlDatabase db;

    bool connectToDatabase();
    bool validateCredentials(const QString &username, const QString &password);
};

#endif // LOGINPAGE_H
