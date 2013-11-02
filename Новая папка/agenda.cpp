#include "agenda.h"
#include "ui_agenda.h"
#include <QMessageBox>
Agenda::Agenda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Agenda)
{
    ui->setupUi(this);
    QMessageBox msgbox;

    try
    {
        file.open("calendar.txt",ios::app);
    }
    catch(...)
    {
        msgbox.setText("Errore del creazione del file");
        msgbox.exec();
    }

    hours = min = 0;
}

Agenda::~Agenda()
{
    delete ui;
    file.close();
}

void Agenda::on_buttonExit_clicked()
{
    this->close();
}

void Agenda::on_calendar_clicked(const QDate &date)
{
    /* Quando utente pigia sulla data, si pulisce textEdit e listWidget per non avere
     * le scritte di altri giorni, poi la data viene salvata nelle variabili, d,m,y
     * che servono per il controllo ecc; Poi avviene il controllo che puo ritornare
     * se ce qualche evento la sua descrizione su listWidget e text edit(per poter
     * cambiare qualcosa) */
    ui->textEdit->clear();
    ui->listWidget->clear();
    calendar calendar;
    d = date.day();
    m = date.month();
    y = date.year();
    ui->listWidget->addItem(calendar.cerca(d,m,y,false));
    ui->textEdit->setText(calendar.cerca(d,m,y,false));
}

void calendar::inserimentoCalendar(int d, int m, int y, int hours, int min, QString descrizione)
{
    /* si prendono i dati passati e si inseriscono in fondo del file */
    // per convertire in char:
    const char *str;
    QByteArray ba;
    ba = descrizione.toLatin1();
    str = ba.data();
    // fine conversione;
    ofstream file;
    file.open("calendar.txt",ios::app);
    file.seekp(0,ios_base::end); // si fa puntare il puntatore al fine del file;
    file << "Data:" << endl;
    file << d << endl;
    file << m << endl;
    file << y << endl;
    file << "Descrizione:" << endl;
    file << hours << ":" << min <<":"<< str << endl;
}

QString calendar::cerca(int d, int m, int y, bool cancellamento)
{
    // funzione che ritorna la stringa con la descrizione e data del evento se esiste
    // si inizia dalla prima riga del file a controllare la data
    // se data e' uguale copia la descrizione nella stringa e laritorna;
    // cancellamento funziona uguale, trova la posizione della data, cioe'
    // una variabile conta prima gli eventi e poi quando si va a cancellare
    // si riscrive tutto il file tra ne quell evento che ha il numero del contatore;
    QString desc; // per la descrizione;
    ifstream file1("calendar.txt");
    file1.seekg(0,ios_base::beg); // si fa puntare il puntatore al iniz. del file;
    bool exit=true; // per uscire dal ciclo
    int count(0);   // contatore degli eventi per cancellamento, si usa in modo che si aumenta contatore quando
    //si incontra "Data:" finche non si trova data della ricerca e data del evento equivalenti
    char temp[10];   // \ ;
    char temp2[10];  //  - variabili temporali per copiare la data in un vettore;
    char temp3[10];  // / ;
    char vet[20];
    // si copia la data in un solo vettore
    sprintf(temp2, "%i", d);
    sprintf(temp3, "%i", m);
    strcat(temp2,temp3);
    sprintf(temp3, "%i", y);
    strcat(temp2,temp3);

    while(exit)
    {
        if(!file1.eof())
        {
            file1 >> vet;
            strcpy(temp,"");
            if(strcmp(vet,"Data:") == 0)
            {
                count++; // serve per cancellamento
                // si conta eventi, per arrivare ad un evento da cancellare;
                for(int i=0;i<3;i++) // si copia ogni data in un vettore per controllarla
                {
                    file1 >> vet;
                    strcat(temp,vet);
                }
                if(strcmp(temp,temp2)==0) // se 2 vettori sono uguali salva la descrizione del girno;
                {
                    file1 >> vet;
                    do
                    {
                        desc = vet;
                        file1 >> vet;
                    }while(strcmp(vet,"Data:")!=0 && !file1.eof());
                    exit=false;
                }
            }
        }
        else
        {
            //Data non e' stata ancora segnata;
            exit = false;
        }
    }
    if(!cancellamento)
    {
        return desc;
    }
    else
    {
        int i(0); // contatore per cercare quale evento deve cancellare;
        ifstream file1("calendar.txt");
        ofstream tempFile("temp.txt");
        file1.seekg(0,ios_base::beg);
        while(!file1.eof())
        {
            file1 >> vet;
            if(strcmp(vet,"Data:") == 0)
            {
                i++;
                if(i != count) // per aggiungere 'Data:';
                {
                    tempFile << vet << endl; //contiene 'Data:';
                }
                if(i == count) // 2 contatori per controllo di un determinato evento
                {              // scelto da cancellare da utente
                    do
                    {
                        file1 >> vet;
                        if(strcmp(vet,"Data:") == 0)
                        {
                            tempFile << vet << endl; // Per aggiungere "Data:";
                        }
                    }while(strcmp(vet,"Data:") != 0 && !file1.eof());
                }
            }
            else
            {
                if(!file1.eof()) // per non aggiungere 2 volte ultima riga;
                    tempFile << vet << endl;
            }
        }
        file1.close();
        tempFile.close();
        remove( "calendar.txt" ); // si cancella 'calendar.txt' per riscriverlo
        ifstream temp1("temp.txt");
        ofstream newFile("calendar.txt");
        exit = true;
        while(exit)
        {
            temp1 >> vet;
            if(!temp1.eof())
            {
                newFile << vet << endl;
            }
            else
            {
                exit = false;
            }
        }
        temp1.close();
        remove( "temp.txt" );
        newFile.close();
    }
}

void Agenda::on_buttonEdit_clicked()
{
    // per inserire nuovo evento
    calendar c;
    QString descrizione;
    descrizione = ui->textEdit->toPlainText(); // si copia il testo nella stringa dal textEdit;
    c.inserimentoCalendar(d,m,y,hours,min, descrizione); // e si sinserisce nel file;
    cambioColore(false); // per cambiare il colore della giornata;
}

void Agenda::cambioColore(bool standart)
{
    /*** Per cambiare il colore dello sfondo della data se ha un evento per quell giorno ***/
    // si prende la data e si salva in una variabile che devo poi passare alla funzione
    QDate giorno = QDate(y,m,d); // il giorno
    QTextCharFormat colore; // il colore
    if(!standart)
    {   // lo sfondo diventa griggio
        colore.setBackground(QColor("#e0e0e0"));
    }
    else
    {   // lo sfondo diventa bianco come prima
        colore.setBackground(QColor("#FFF"));
    }
    ui->calendar->setDateTextFormat(giorno,colore);
    /*********** FINE ******************/
}

void Agenda::on_cancButton_clicked()
{
    // per cancellare il evento;
    calendar calendar;
    file.close();
    calendar.cerca(d,m,y,true);
    file.open("calendar.txt",ios::app);
    // per cambiare il colore dello sfondo come prima;
    cambioColore(true);
}

void Agenda::on_spinBox_2_valueChanged(int arg1)
{
    hours = arg1;
}

void Agenda::on_spinBox_valueChanged(int arg1)
{
    min = arg1;
}
