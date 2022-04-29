#include "XSerialPortWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XSerialPortWidget w;
    w.show();
    return a.exec();
}
