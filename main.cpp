#include <QApplication>
#include "sign_in.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create and show the login page
    // LoginPage loginPage;
    // loginPage.show();
    sign_in signin;
    signin.show();

    return a.exec();
}
