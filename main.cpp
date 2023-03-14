#include "statisticswizard.h"

#include <QApplication>
#include <iostream>

#ifdef _WIN32
#include "windows.h"
#else
#endif
int main(int argc, char *argv[])
{
#ifdef _WIN32
#ifndef _DEBUG  // COK HOS
if (argc != 1 && argc != 2)
    {
        fprintf(stderr, "No more arguments than one!\n");
        fflush(stderr);
        return -1;
    }
    if (argc == 1)
    {
        FreeConsole();
    }
    else if(strcmp(argv[1], "--enable-logging"))
    {
        fprintf(stderr, "The given argument is wrong! Only the argument --enable-logging is accepted.\n");
        fflush(stderr);
        return -2;
    }
#endif
#endif
    QApplication a(argc, argv);
    StatisticsWizard w;
    w.show();
    return a.exec();
}
