#include "signup_page.h"
#include "ui_signup_page.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

signup_page::signup_page(QWidget *parent): QWidget(parent), ui(new Ui::signup_page)
{
    ui->setupUi(this);
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }
    connect(ui->signupbutton, &QPushButton::clicked, this, &signup_page::onSignupbuttonClicked);
}

signup_page::~signup_page()
{
    delete ui;
}


bool signup_page::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QODBC");
    QString connectionString =
        "Driver={Oracle in OraDB21Home1};"
        "DBQ=//localhost:1521/XE;"
        "Uid=c##app_user;"
        "Pwd=123;";
    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully!";
    return true;
}

bool signup_page::checkIfUserExist(const QString &username) {
    QString accountType = ui->comboBox->currentText();
    if (accountType == "none") {
        QMessageBox::warning(this, "Error", "Please select an account type");
        return false;
    }
    if (accountType == "Teacher"){
        QSqlQuery query;
        query.prepare("select teacher_id from teachers where user_id = (select user_id from users where username = :username)");
        query.bindValue(":username", username);

        if (!query.exec()) {
            qDebug() << "Query failed:" << query.lastError().text();
            return false;
        }
        return query.next();
    }
    else if(accountType == "Student"){
        QSqlQuery query;
        query.prepare("select student_id from students where user_id = (select user_id from users where username = :username);");
        query.bindValue(":username", username);

        if (!query.exec()) {
            qDebug() << "Query failed:" << query.lastError().text();
            return false;
        }
        return query.next();
    }
}

void signup_page::adduser(const QString &username, const QString &password) {
    QString accountType = ui->comboBox->currentText();
    if (accountType == "none") {
        QMessageBox::warning(this, "Error", "Please select an account type");
        return;
    }

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.prepare("insert into users (username, password) values (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        QSqlDatabase::database().commit();
        qDebug() << "User added successfully!";
    } else {
        QSqlDatabase::database().rollback();
        qDebug() << "Failed to add user:" << query.lastError().text();
    }
    if (accountType == "Student"){
        query.prepare("insert into students (user_id) select user_id from users where username = :username FETCH FIRST 1 ROW ONLY");
        query.bindValue(":username", username);
        if (query.exec()) {
            QSqlDatabase::database().commit();
            qDebug() << "User added successfully!";
        } else {
            QSqlDatabase::database().rollback();
            qDebug() << "Failed to add user:" << query.lastError().text();
        }
    }
    else if (accountType == "Teacher"){
        query.prepare("insert into teachers (user_id) select user_id from users where username = :username FETCH FIRST 1 ROW ONLY");
        query.bindValue(":username", username);
        if (query.exec()) {
            QSqlDatabase::database().commit();
            qDebug() << "User added successfully!";
        } else {
            QSqlDatabase::database().rollback();
            qDebug() << "Failed to add user:" << query.lastError().text();
        }
    }
}

void signup_page::onSignupbuttonClicked() {
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Fields cannot be empty");
        return;
    }

    if (checkIfUserExist(username)) {
        QMessageBox::warning(this, "Signup Failed", "The account already exists");
    } else {
        adduser(username, password);
        QMessageBox::information(this, "Signup Successful", "The account was created successfully");
    }
}


