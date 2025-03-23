#include "loginpage.h"
#include "ui_loginpage.h"
#include "signup.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>
#include "teacher.h"
#include "student.h"

LoginPage::LoginPage(QWidget *parent) : QWidget(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);

    // Set up the database connection
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }
    connect(ui->signupbutton, &QPushButton::clicked, this, &LoginPage::onSignupbuttonClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::onLoginButtonClicked);
}

LoginPage::~LoginPage() {
    delete ui;
}

bool LoginPage::connectToDatabase() {
    // Set up the Oracle database connection
    db = QSqlDatabase::addDatabase("QODBC");
    QString connectionString =
        "Driver={Oracle in OraDB21Home1};"
        "DBQ=//localhost:1521/XE;"
        "Uid=c##app_user;"
        "Pwd=123;";
    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        QMessageBox::warning(this, "Data base", "an error happend during the connection to database");
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}

QString LoginPage::validateCredentials(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return ""; // Return empty string on failure
    }

    // If a matching record is found, return the role
    if (query.next()) {
        return query.value(0).toString(); // Get the role from the query result
    }

    return ""; // Return empty string if no user found
}
void LoginPage::onLoginButtonClicked() {
    // Get the username and password from the input fields
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::information(this, "Error", "Fields is empty");
    }
    else{
        QString role = validateCredentials(username);
            if (role != "") {
            if (role == "teacher"){
                QMessageBox::information(this, "Success", "Login successful!");
                teacher *teacherWindow = new teacher();
                teacherWindow->show();
            }
            else if (role == "student"){
                QMessageBox::information(this, "Success", "Login successful!");
                student *studentWindow = new student();
                studentWindow->show();
            }
            else if (role == "admin"){
                QMessageBox::information(this, "Success", "Login successful! ADMIN !!");
            }
            else {
                QMessageBox::warning(this, "Error", "Invalid username or password.");
            }
            }
    }
}
void LoginPage::onSignupbuttonClicked(){
    // Create an instance of the Signup window
    signup *signupWindow = new signup();
    // Show the Signup window
    signupWindow->show();
}


