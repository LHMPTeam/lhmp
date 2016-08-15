// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "launcher.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Launcher w;
    w.show();

    return a.exec();
}
