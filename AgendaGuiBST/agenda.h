#ifndef AGENDA_H
#define AGENDA_H

#include <QWidget>
#include <QDate>
#include <iostream>
#include <fstream>
#include <QColor>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTableWidgetItem>
using namespace std;

struct record
{
    int day;
    int month;
    int year;
    int hour;
    int minutes;
    char descrizone[100];
    bool cancellato;
    record()
    {
        memset(descrizone,' ',100);
        cancellato = false;
    }
};

struct albero
{
    int sx,dx,giorno,mese,anno,ore,min,pos;
    bool cancellato;
    albero()
    {
        sx = dx = -1;
        cancellato = false;
    }
};

struct vet
{
    int posizione;
    struct vet *next;
    vet()
    {
        posizione = -1;
    }
};

////////////////////////////////

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
    int posizioneItem; // per la posizione del evento selezionato;
    unsigned int quantita; // quantita degli eventi
    fstream file; // file dove per salvare;
    record rec;
    void cambioColore();
    void cerca(int posizione);
    void cancellamento();
    void svuota();
    int confronto(albero data);
private slots:
    void on_calendar_clicked(const QDate &date);
    void on_buttonEdit_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_hours_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
    void on_pushButton_clicked();
private:
    Ui::Agenda *ui;
};

///////////////////////////////

class calendar
{
public:
    record record1;
    albero albero1;
    ofstream file;
    //ofstream bst;
    void inserimentoBST();
    int confronto(albero data);
    void inserimentoCalendar(int d, int m, int y, int hours, int min, QString Descrizione);
    calendar();
    ~calendar();
};

#endif // AGENDA_H
