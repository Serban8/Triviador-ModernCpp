#include "TriviadorClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TriviadorClient w;
    w.show();
    return a.exec();
}
