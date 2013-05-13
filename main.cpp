#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    w.setWindowTitle("Extrem Data Base For Video Game");
    w.show();
    
    return a.exec();
}
