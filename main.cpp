#include <QApplication>
#include "loginpage.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create and show the login page
    LoginPage loginPage;
    loginPage.show();

    return a.exec();
}
