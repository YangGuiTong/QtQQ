#include "CCMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCMainWindow w;
    w.show();
    return a.exec();
}
