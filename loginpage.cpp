#include "loginpage.h"
#include "ui_loginpage.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent) : QWidget(parent), ui(new Ui::LoginPage) {
    ui->setupUi(this);

    // Set up the database connection
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }
    connect(ui->signupbutton, &QPushButton::clicked, this, &LoginPage::onSignupbuttonClicked);
    // Connect the login button to the slot
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
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}

bool LoginPage::validateCredentials(const QString &username, const QString &password) {
    // Prepare the SQL query to check the credentials
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }

    // Check if a matching record was found
    return query.next();
}
bool LoginPage::checkIfUserExist(const QString &username){
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return false;
    }

    // Check if a matching record was found
    return query.next();

}
void LoginPage::adduser(const QString &username, const QString &password) {
    QSqlQuery query;

    // Start a transaction
    QSqlDatabase::database().transaction();

    // Prepare the SQL query
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    // Execute the query
    if (query.exec()) {
        // Commit the transaction if the query succeeds
        QSqlDatabase::database().commit();
        qDebug() << "User added successfully!";
    } else {
        // Rollback the transaction if the query fails
        QSqlDatabase::database().rollback();
        qDebug() << "Failed to add user:" << query.lastError().text();
    }
}
void LoginPage::onLoginButtonClicked() {
    // Get the username and password from the input fields
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if (username == "" || password == "")
    {
        QMessageBox::information(this, "Error", "Fields is empty");
    }
    else{
        // Validate the credentials
        if (validateCredentials(username, password)) {
            QMessageBox::information(this, "Success", "Login successful!");
        } else {
            QMessageBox::warning(this, "Error", "Invalid username or password.");
        }

    }
}
void LoginPage::onSignupbuttonClicked(){
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if (username == "" || password == "")
    {
        QMessageBox::information(this, "Error", "Fields is empty");
    }
    else {
        if (checkIfUserExist(username)){
            QMessageBox::information(this, "signup failed", "the account already exist");
        }
        else{
            adduser(username, password);
            QMessageBox::information(this, "signup done", "the account is made scsuessfully");
        }
    }
}
