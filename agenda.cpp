#include "agenda.h"
#include "ui_agenda.h"

Agenda::Agenda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Agenda)
{
    ui->setupUi(this);
    hours = min = 0;
}

Agenda::~Agenda()
{
    delete ui;
}

void Agenda::on_calendar_clicked(const QDate &date)
{
    /* Quando utente pigia sulla data, si pulisce textEdit e listWidget per non avere
     * le scritte di altri giorni, poi la data viene salvata nelle variabili, d,m,y
     * che servono per dei controlli, ecc; Poi avviene il metodo che aggiunge nella
     * listWidget degli eventi, e si abilita il tasto per aggiungere eventi
     * che era disabilitato prima, per il motivo quando utente va a pigire sul
     * evento di listWidget per non farlo aggiungere la stessa cosa*/
    ui->textEdit->clear();
    ui->listWidget->clear();
    d = date.day();
    m = date.month();
    y = date.year();
    cout << d << "/" << m << "/" << y << endl;
    cerca(false);
    ui->buttonEdit->setDisabled(false); // si abilita il tasto per aggiungere;
}

calendar::calendar()
{
    QMessageBox msgbox;
    try
    {
        bst.open("bst.txt",ios::app); // |ios_base::binary
        file.open("calendar.txt",ios_base::app);
    }
    catch(...)
    {
        msgbox.setText("Errore del creazione del file");
        msgbox.exec();
    }
}

void calendar::inserimentoCalendar(int d, int m, int y, int hours, int min, QString Descrizione)
{
    /* si prendono i dati passati e si inseriscono in fondo del file */
    // per convertire in char:
    QByteArray ba = Descrizione.toLocal8Bit();
    const char *str = ba.data();
    // fine conversione;
    // inserimento nella struct del giorno e nella struct bst;
    record1.day = albero1.giorno = d;
    record1.month = albero1.mese = m;
    record1.year = albero1.anno = y;
    record1.hour = albero1.ore = hours;
    record1.minutes = albero1.min = min;
    strcpy(record1.descrizone,str); // si copia la descrizione;
    file.seekp(0,ios_base::end); // si fa puntare il puntatore al fine del file;
    albero1.pos = file.tellp()/sizeof(record); // avro' la posizione del evento inserito;
    file.write((const char*)&record1,sizeof(record));
    inserimentoBST();
}

void calendar::inserimentoBST()
{
    albero lettura; //struct per caricare data dal bst;
    bool trovato = false;
    bool primoEvento = true;
    int posizione; // posizione dentro il file del BST
    ifstream readbst("bst.txt");
    readbst.seekg(0,ios_base::beg);
    while(!readbst.eof() && trovato==false) // con questo ciclo trovo se c'e' un evento cancellato per recuperare lo spazio
    {
        readbst.clear();
        readbst.read((char*)&lettura,sizeof(albero));
        if(!readbst.eof()) // perche' c'e' il problema che viene letto 2 volte ultimo elem
        {
            readbst.clear();
            primoEvento = false;
            cout << "lettura:" << lettura.giorno << "/" << lettura.mese << "/" << lettura.anno << ":" << lettura.sx << "|" << lettura.dx << endl;
            if(lettura.cancellato)
            {
                trovato = true;
                posizione = (readbst.tellg()/sizeof(albero));
            }
        }
    }
    if(trovato) // se trova evento cancellato salva - la posizione in una variabile, altrimenti salva alla fine del file
    {
        bst.seekp(posizione*(sizeof(albero)),ios_base::beg);
    }
    else
    {
        bst.seekp(0,ios_base::end);
        posizione = (bst.tellp()/sizeof(albero));
    }
    bst.write((const char*)&albero1,sizeof(albero)); // salva nel file
    if(!primoEvento)
    {
        int puntatorePosizione(0);
        trovato = false;
        while(trovato == false)
        {
            readbst.clear(); // cancella gli errori;
            readbst.seekg((puntatorePosizione*sizeof(albero)),ios::beg);
            readbst.read((char*)&lettura,sizeof(albero));
            cout << "da riscrivere:" << lettura.giorno << "/" << lettura.mese << "/" << lettura.anno << ":" << lettura.sx << "|" << lettura.dx << endl;
            if(!lettura.cancellato) // serve per il primo elemento
            {
                if(confronto(lettura) > 0)
                { // a destra
                    if(lettura.dx != -1)
                    {
                        puntatorePosizione = lettura.dx;
                    }
                    else
                    {
                        lettura.dx = posizione;
                        trovato = true;
                    }
                }
                else
                { // a sinistra
                    if(lettura.sx != -1)
                    {
                        puntatorePosizione = lettura.sx;
                    }
                    else
                    {
                        lettura.sx = posizione;
                        trovato = true;
                    }
                }
            }
            else
            {
                trovato = true;
            }
        }
        cout << "da riscrivereFINALE:" << lettura.giorno << "/" << lettura.mese << "/" << lettura.anno << ":" << lettura.sx << "|" << lettura.dx << endl;
        rewrite(lettura, puntatorePosizione);
    }
}

void calendar::rewrite(albero &data, int &posizione)
{
    ofstream reWrite("bst.txt");
    reWrite.clear(); // cancella gli errori;
    reWrite.seekp(((posizione)*sizeof(albero)),ios::beg);
    reWrite.write((const char*)&data,sizeof(albero));
    reWrite.close();
}

// La funzione per decidere dove deve andare elemento A DESTRA(1) o A SINISTRA(-1);
int calendar::confronto(albero data) // albero1 e' la data selezionata;
{                                    // data e' struttura letta dal file
    if(albero1.anno > data.anno)
    {
        return 1;
    }
    else
    {
        if(albero1.anno < data.anno)
        {
            return -1;
        }
        else
        {
            if(albero1.mese > data.mese)
            {
                return 1;
            }
            else
            {
                if(albero1.mese < data.mese)
                {
                    return -1;
                }
                else
                {
                    if(albero1.giorno > data.giorno)
                    {
                        return 1;
                    }
                    else
                    {
                        if(albero1.giorno < data.giorno)
                        {
                            return -1;
                        }
                        else
                        {
                            if(albero1.ore > data.ore)
                            {
                                return 1;
                            }
                            else
                            {
                                if(albero1.ore < data.ore)
                                {
                                    return -1;
                                }
                                else
                                {
                                    if(albero1.min > data.min)
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        if(albero1.min < data.min)
                                        {
                                            return -1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Agenda::cerca(int posizione)
{
    albero bst;
    ifstream readBst("bst.txt",ios_base::app);
    ifstream readCalendar("calendar.txt",ios_base::app);
    bool trovato = false;
    if(!readBst.eof())
    {
        readBst.clear();
        readBst.seekg(0,ios_base::beg);
        readBst.seekg((posizione*(sizeof(albero))),ios_base::cur);
        readBst.read((char*)&bst,sizeof(albero));
        cout << bst.giorno << "/" << bst.mese << "/" << bst.anno << ":" << bst.sx << "|" << bst.dx << endl;
        if(!readBst.eof()) // seconda volta serve per non aggiungere due volte la descrizione;
        {
            if(!bst.cancellato)
            {
                if(bst.anno == y)
                {
                    if(bst.mese == m)
                    {
                        if(bst.giorno == d)
                        {
                            readCalendar.seekg((bst.pos*sizeof(albero)),ios_base::beg);
                            readCalendar.read((char*)&rec,sizeof(record));
                            ui->listWidget->addItem(rec.descrizone);
                            trovato = true;
                            if(bst.dx != -1)
                            {
                                cerca(bst.dx);
                            }
                            if(bst.sx != -1)
                            {
                                cerca(bst.sx);
                            }
                        }
                    }
                }
                if(!trovato)
                {   // controllo se la data che ha selezionato, e' > di data sul file;
                    if(confronto(bst) < 0) // se -1 a destra , se 1 a sinistra
                    { // se e' > va a destra
                        if(bst.dx != -1)
                        {
                            cerca(bst.dx);
                        }
                    }
                    else
                    { // se e' < vado a sinistra;
                        if(bst.sx != -1)
                        {
                            cerca(bst.sx);
                        }
                    }
                }
            }
        }
    }
}

int Agenda::confronto(albero data)
{
    int ritorno;
    if(y > data.anno)
    {
        ritorno = 1;
    }
    else if(y < data.anno)
    {
        ritorno = -1;
    }
    else if(m > data.mese)
    {
        ritorno = 1;
    }
    else if(m < data.mese)
    {
        ritorno = -1;
    }
    else if(d > data.giorno)
    {
        ritorno = 1;
    }
    else if(d < data.giorno)
    {
        ritorno = -1;
    }
    else if(hours > data.ore)
    {
        ritorno = 1;
    }
    else if(hours < data.ore)
    {
        ritorno = -1;
    }
    else if(min > data.min)
    {
        ritorno = 1;
    }
    else if(min < data.min)
    {
        ritorno = -1;
    }
    return ritorno;
}

void Agenda::cancellamento()
{   /* Scorrendo file si riscrive nel altro file, controllando le date, e la posizione
     * selezionata dal utente nella listWidget, se trova la data e la posizione uguale
     * si ritorna di sizeof(record) byte indietro con il cursore fi ofstream,
     * in questo modo sparisce evento, pero' questo modo, ha un problema, che non puo'
     * cancellare ultimo evento del file, perche' anche se ritorna d
     */
    int countRiga(0); // contatore per la riga selezionata si controlla con variabile posizioneItem
    int size = 0-sizeof(record);
    bool cancellato = false; // serve per non cancellare gli elementi;
    ifstream file1;
    file1.open("calendar.txt",ios_base::app);
    ofstream tempFile("temp.txt");
    file1.seekg(0,ios_base::beg);
    while(!file1.eof())
    {
        file1.read((char*)&rec,sizeof(record));
        if(!file1.eof()) // il controllo serve per non aggiungere 2volte ultimo evento alla fine;
        {
            tempFile.write((const char*)&rec,sizeof(record));
        }
        if(rec.day == d)
        {
            if(rec.month == m)
            {
                if(rec.year == y)
                {
                    if(countRiga == posizioneItem && !cancellato) // si controlla se e' la riga selezionata;
                    {   // bool cancellato serve per non cancellare gli elementi che stanno dopo, perche'
                        // una volta preso la posizione della riga nella 'posizioneItem' dopo che si cancella
                        // una riga dalla listWidget le posizioni si scambiano ma 'posizioneItem' rimane e
                        // il ciclo continua, quindi puo' cancellare righe sbagliate
                        tempFile.seekp(size,ios_base::cur);
                        cancellato = true;
                    }
                    else
                    {
                        countRiga++;
                    }
                }
            }
        }
    }
    // si riscrive il file;
    file1.close();
    tempFile.close();
    remove( "calendar.txt" ); // si cancella 'calendar.txt' per riscriverlo
    ifstream temp1("temp.txt");
    ofstream newFile("calendar.txt");
    while(!temp1.eof())
    {
        temp1.read((char*)&rec,sizeof(record));
        if(!temp1.eof())
        {
            newFile.write((const char*)&rec,sizeof(record));
        }
    }
    temp1.close();
    remove( "temp.txt" );
    newFile.close();
}

void Agenda::on_buttonEdit_clicked()
{
    // per inserire nuovo evento
    // si prende tutto cio che e' scritto nella textEdit e si salva nella stringa
    // che poi verra' passata al metodo d'inserimento, e si evidenzia il giorno
    calendar c;
    QString descrizione;
    descrizione = ui->textEdit->toPlainText(); // si copia il testo nella stringa dal textEdit;
    c.inserimentoCalendar(d,m,y,hours,min, descrizione); // e si sinserisce nel file;
    cambioColore(false); // per cambiare il colore della giornata;
    ui->textEdit->clear(); // se utente pigia aggiungere si pulisce textEdit
}

void Agenda::cambioColore(bool standart)
{
    // Per cambiare il colore dello sfondo della data se ha uno o piu eventi
    // per quell giorno(si evidenzia),si prende la data e si salva in una variabile che
    // devo poi passare alla funzione
    // Si usa questo metodo anche per ritornare al colore come era prima senza eventi
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
    file.close();
    cancellamento();
    file.open("calendar.txt",ios_base::app);
    /* per cambiare il colore dello sfondo come prima,
     * controlla quantita di eventi nel giorno, se sono 0
     * cambia il colore come prima,altrimenti rimane evidenziato*/
    cerca(false); // conta quantita
    if(quantita == 0)
    {
        cambioColore(true);
    }
    // leva dalla listWidget(solo per la grafica, per non aggiornare)
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
}

void Agenda::on_spinBox_2_valueChanged(int arg1)
{
    hours = arg1;
}

void Agenda::on_spinBox_valueChanged(int arg1)
{
    min = arg1;
}

void Agenda::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // Se preme due volte sulla cella nella listWidget,
    // si permette di modificare la cella scrivendo sopra
    item->setFlags( item->flags() |
                    Qt::ItemIsSelectable |
                    Qt::ItemIsEditable |
                    Qt::ItemIsEnabled );
    ui->listWidget->editItem(item);// modifica;
}

void Agenda::on_listWidget_itemChanged(QListWidgetItem *item)
{
    /* Subito dopo della modifica prende il testo inserito della modifica
     * e lo salva nella stringa, controlla il file giorno per girno se sono uguali
     * le date, se si- controlla anche la posizione del evento nella listWidget,
     * il motivo e', che utente ha selezionato un evento che ha la sua posizione
     * nella listWidget, grazie a qusto si scorre file e si si trova un giorno uguale
     * si guarda se posizione che ha selezionato utente e' uguale a contatore degli
     * eventi del giorno, se si, si esce dal ciclo riscrivendo su evento da cambiare,
     * se no - si aumenta il contatore degli eventi del giorno e si continua il ciclo
     **/
    record rec;
    QString desc = item->text(); // salva nella stringa cosa ha scritto utente
    fstream file1;
    file1.open("calendar.txt",ios_base::app);
    file1.seekg(0,ios_base::beg); // si fa puntare il puntatore al iniz. del file;
    bool exit=true; // per uscire dal ciclo
    int countRiga(0);
    while(exit)
    {
        file1.read((char*)&rec,sizeof(record));
        if(!file1.eof())
        {// il controllo serve per non aggiungere 2volte ultimo evento alla fine;
            if(rec.day == d)
            {
                if(rec.month == m)
                {
                    if(rec.year == y)
                    {
                        if(countRiga == posizioneItem)
                        {
                            exit = false;
                        }
                        else
                        {
                            countRiga++;
                        }
                    }
                }
            }
        }
    }
    QByteArray ba = desc.toLocal8Bit(); // converto QString to char;
    const char *str = ba.data(); // conver.
    strncpy(rec.descrizone,str,100);
    int size = 0-sizeof(record);
    file1.seekp(size,ios_base::cur);
    file1.write((const char*)&rec,sizeof(record));
    file1.close();
}

void Agenda::on_listWidget_itemClicked(QListWidgetItem *item)
{   // Quando utente clicka sulla cella della listWidget,
    // prende la posizione del evento selezionato
    // e stampa nella textEdit ore e la descrizione del evento selezionato;
    posizioneItem = ui->listWidget->row(ui->listWidget->currentItem());
    cerca(true); // riempe la struttura
    ui->textEdit->setText(QString::number(rec.hour) + ":" + QString::number(rec.minutes) + " " + rec.descrizone);
    ui->buttonEdit->setDisabled(true); // si disabilita il tasto per aggiungere
}

calendar::~calendar()
{
    file.close();
    bst.close();
}
