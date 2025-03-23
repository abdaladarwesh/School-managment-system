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
    connect(ui->teacher, &QRadioButton::clicked, this, &LoginPage::checkButtonState);
    connect(ui->student, &QRadioButton::clicked, this, &LoginPage::checkButtonState);
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
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}

bool LoginPage::validateCredentials(const QString &username, const QString &password) {
    QString accountType = checkButtonState();
    QSqlQuery query;
    query.prepare("select * from " + accountType + "s where username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }

    // Check if a matching record was found
    return query.next();
}
void LoginPage::onLoginButtonClicked() {
    // Get the username and password from the input fields
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if (username.isEmpty() || password.isEmpty() || checkButtonState() == "none")
    {
        QMessageBox::information(this, "Error", "Fields is empty");
    }
    else{
        if (checkButtonState() == "teacher"){
            if (validateCredentials(username, password)) {
                QMessageBox::information(this, "Success", "Login successful!");
                teacher *teacherWindow = new teacher();
                teacherWindow->show();
            } else {
                QMessageBox::warning(this, "Error", "Invalid username or password.");
            }
        }
        else if (checkButtonState() == "student"){
                if (validateCredentials(username, password)) {
                    QMessageBox::information(this, "Success", "Login successful!");
                    student *studentWindow = new student();
                    studentWindow->show();
                } else {
                    QMessageBox::warning(this, "Error", "Invalid username or password.");
                }
        }
        // Validate the credentials

    }
}
void LoginPage::onSignupbuttonClicked(){
    // Create an instance of the Signup window
    signup *signupWindow = new signup();
    // Show the Signup window
    signupWindow->show();
}

QString LoginPage::checkButtonState() {
    if (ui->teacher->isChecked()) {
        qDebug() << "t";
        return "teacher";
    } else if (ui->student->isChecked()) {
        qDebug() << "s";
        return "student";
    } else {
        return "none";
    }
}



