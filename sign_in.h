#ifndef SIGN_IN_H
#define SIGN_IN_H

#include <QWidget>
#include <QWidget>
#include <QSqlDatabase>
#include <QString>
#include "teacher.h"
#include "student.h"


QT_BEGIN_NAMESPACE
namespace Ui { class sign_in; }
QT_END_NAMESPACE

class sign_in : public QWidget
{
    Q_OBJECT

public:
    explicit sign_in(QWidget *parent = nullptr);
    ~sign_in();
private slots:
    void onLoginButtonClicked();
    void onSignupbuttonClicked();

private:
    Ui::sign_in *ui;
    QSqlDatabase db;

    bool connectToDatabase();
    QString validateCredentials(const QString &username,const QString &password);

};

#endif // SIGN_IN_H
