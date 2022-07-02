#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    std::string path =  __FILE__;
    path = path.substr(0,path.size()-8) + "people.db";

    QApplication a(argc, argv);
    DbManager d(path.c_str());
    MainWindow w;
    w.show();
    return a.exec();
}
