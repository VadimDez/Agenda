#ifndef AGENDA_H
#define AGENDA_H

#include <QWidget>
#include <QDate>
#include <iostream>
#include <fstream>
#include <QColor>
using namespace std;

namespace Ui {
class Agenda;
}

class Agenda : public QWidget
{
    Q_OBJECT
    
public:
    explicit Agenda(QWidget *parent = 0);
    ~Agenda();
    int d,m,y; // giorno, mese, anno;
    int hours,min; // ore, minuti
    ofstream file; // file dove per salvare;
    void cambioColore(bool standart);
private slots:
    void on_buttonExit_clicked();
    void on_calendar_clicked(const QDate &date);
    void on_buttonEdit_clicked();
    void on_cancButton_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::Agenda *ui;
};

class calendar
{
public:
    int giorno;
    int mese;
    int anno;
    char descrizione[20];
    void inserimentoCalendar(int d, int m, int y, int hours, int min, QString descrizione);
    QString cerca(int d, int m, int y, bool cancellamento);
};

#endif // AGENDA_H
