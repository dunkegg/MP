#include "MPw.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    admittance::Magv agv;
    MPw w;
    w.show();
    return a.exec();
}
