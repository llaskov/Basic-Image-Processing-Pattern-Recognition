#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Image processor");

    MainWindow main_win;
    main_win.show();

    return app.exec();
}//main
