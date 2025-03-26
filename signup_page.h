#ifndef SIGNUP_PAGE_H
#define SIGNUP_PAGE_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class signup_page;
}

class signup_page : public QWidget
{
    Q_OBJECT

private slots:
    void onSignupbuttonClicked();


public:
    explicit signup_page(QWidget *parent = nullptr);  // Ensure the class name matches
    ~signup_page();

private:
    Ui::signup_page *ui;
    QSqlDatabase db;

    bool connectToDatabase();
    bool validateCredentials(const QString &username, const QString &password);
    void adduser(const QString &username, const QString &password);
    bool checkIfUserExist(const QString &username);

};

#endif // SIGNUP_PAGE_H
