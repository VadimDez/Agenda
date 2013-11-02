/*
  Author: Vadym Yatsyuk
  Programma che utilizzando file gestisce un'agenda.
 */
#include <QtGui/QApplication>
#include "agenda.h"

int main(int argc, char *argv[])
{
    ofstream file("calendar.txt"); // crea il file
    QApplication a(argc, argv);
    Agenda w;
    w.show();

    return a.exec();
}
