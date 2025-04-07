#include "sign_in.h"
#include "ui_sign_in.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "signup_page.h"
#include "student.h"
#include "teacher.h"

sign_in::sign_in(QWidget *parent): QWidget(parent), ui(new Ui::sign_in)
{
    ui->setupUi(this);
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }
    connect(ui->loginButton, &QPushButton::clicked, this, &sign_in::onLoginButtonClicked);

}




sign_in::~sign_in()
{
    delete ui;
}

bool sign_in::connectToDatabase() {
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

QString sign_in::validateCredentials(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("select user_id from users where username = :username and password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "User Query failed:\n" << query.lastError().text();
        return ""; // Return empty string on failure
    }

    // If a matching record is found, return the role
    if (query.next()) {
        int user_id = query.value(0).toInt();
        query.prepare("select * from teachers where user_id = :user_id");
        query.bindValue(":user_id", user_id);
        if (!query.exec()) {
            qDebug() << "Teacher Query failed:" << query.lastError().text();
            return ""; // Return empty string on failure
        }
        if (query.next()){
            return "teacher";
        }
        else {
            query.prepare("select * from students where user_id = :user_id");
            query.bindValue(":user_id", user_id);
            if (!query.exec()) {
                qDebug() << "Student Query failed:" << query.lastError().text();
                return ""; // Return empty string on failure
            }
            if(query.next()){
                return "student";
            }
        }
    }
    return ""; // Return empty string if no user found
}

void sign_in::onLoginButtonClicked() {
    // Get the username and password from the input fields
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::information(this, "Error", "Fields is empty");
    }
    else{
        QString role = validateCredentials(username, password);
        if (role != "") {
            if (role == "teacher"){
                QMessageBox::information(this, "Success", "Login successful!");
                teacher *teacherWindow = new teacher();
                teacherWindow->show();
                this->close();
            }
            else if (role == "student"){
                QMessageBox::information(this, "Success", "Login successful!");
                student *studentWindow = new student();
                studentWindow->show();
                this->close();
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

void sign_in::onSignupbuttonClicked(){
    // Create an instance of the Signup window
    signup_page *signupWindow = new signup_page();
    // Show the Signup window
    signupWindow->show();
    this->close();
}






