#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QString>
#include <QSqlDatabase>


namespace Ui {
class signup;
}

class signup : public QDialog
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();

private slots:
    QString checkButtonState();
    void onSignupbuttonClicked();


private:
    Ui::signup *ui;
    QSqlDatabase db;

    bool connectToDatabase();
    bool validateCredentials(const QString &username, const QString &password);
    void adduser(const QString &username, const QString &password);
    bool checkIfUserExist(const QString &username);

};

#endif // SIGNUP_H
