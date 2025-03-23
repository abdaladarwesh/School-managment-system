#include "signup.h"
#include "ui_signup.h"  //  Include this to resolve the error
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

signup::signup(QWidget *parent)
    : QDialog(parent), ui(new Ui::signup) {  //  Correct initialization
    ui->setupUi(this);  //  Setup UI properly
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }
    connect(ui->teacher, &QRadioButton::clicked, this, &signup::checkButtonState);
    connect(ui->student, &QRadioButton::clicked, this, &signup::checkButtonState);
    connect(ui->signupbutton, &QPushButton::clicked, this, &signup::onSignupbuttonClicked);
}

signup::~signup() {
    delete ui;
}

bool signup::connectToDatabase() {
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

bool signup::validateCredentials(const QString &username, const QString &password) {
    QString accountType = checkButtonState();
    if (accountType == "none") {
        QMessageBox::information(this, "Error", "Please select an account type");
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM " + accountType + "s WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }
    return query.next();
}

bool signup::checkIfUserExist(const QString &username) {
    QString accountType = checkButtonState();
    if (accountType == "none") {
        QMessageBox::information(this, "Error", "Please select an account type");
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM " + accountType + "s WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }
    return query.next();
}

void signup::adduser(const QString &username, const QString &password) {
    QString accountType = checkButtonState();
    if (accountType == "none") {
        QMessageBox::information(this, "Error", "Please select an account type");
        return;
    }

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.prepare("INSERT INTO " + accountType + "s (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        QSqlDatabase::database().commit();
        qDebug() << "User added successfully!";
    } else {
        QSqlDatabase::database().rollback();
        qDebug() << "Failed to add user:" << query.lastError().text();
    }
}

QString signup::checkButtonState() {
    if (ui->teacher->isChecked()) {
        return "teacher";
    } else if (ui->student->isChecked()) {
        return "student";
    } else {
        return "none";
    }
}

void signup::onSignupbuttonClicked() {
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || checkButtonState() == "none") {
        QMessageBox::information(this, "Error", "Fields cannot be empty");
        return;
    }

    if (checkIfUserExist(username)) {
        QMessageBox::information(this, "Signup Failed", "The account already exists");
    } else {
        adduser(username, password);
        QMessageBox::information(this, "Signup Successful", "The account was created successfully");
    }
}





