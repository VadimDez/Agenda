#include "agenda.h"
#include "ui_agenda.h"
int sizeAlbero = sizeof(albero);
int sizeRecord = sizeof(record);
vet *vetPosizioni,root;

Agenda::Agenda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Agenda)
{
    ui->setupUi(this);
    hours = min = 0;
    ui->pushButton->setDisabled(true); // disabilito il tasto per cancellare fino a quell momento quando utente non preme una cella della ListWidget
    vetPosizioni = &root;
}

Agenda::~Agenda()
{
    delete ui;
}

void Agenda::on_calendar_clicked(const QDate &date)
{
    /* Quando utente pigia sulla data, si pulisce textEdit e listWidget per non avere
     * le scritte d'altri giorni, poi la data viene salvata nelle variabili, d,m,y
     * che servono per dei controlli, ecc; Poi avviene il metodo che aggiunge nella
     * listWidget degli eventi, e si abilita il tasto per aggiungere eventi
     * che era disabilitato prima, per il motivo quando utente va a pigire sul
     * evento di listWidget per non farlo aggiungere la stessa cosa*/
    ui->textEdit->clear();
    ui->listWidget->clear();
    d = date.day();
    m = date.month();
    y = date.year();
    svuota(); // cosi svuota tutto che c'era dentro la struttura;
    vetPosizioni = &root;
    cerca(0); // fa vedere eventi del giorno
    ui->buttonEdit->setDisabled(false); // si abilita il tasto per aggiungere;
}

calendar::calendar()
{
    QMessageBox msgbox;
    try
    {
        file.open("calendar.txt", ios::binary | ios::in | ios::out);
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
    bool trovato=false;
    record reading;
    ifstream calRead("calendar.txt");
    while(!trovato) // trovo primo evento cancellato;
    {
        calRead.read((char*)&reading,sizeRecord);
        if(!calRead.eof())
        {
            calRead.clear();
            if(reading.cancellato)
            {
                trovato = true;
            }
        }
        else
        {
            trovato = true;
        }
    }
    albero1.pos = calRead.tellg()/sizeRecord; // avro' la posizione del evento inserito;
    file.seekp(albero1.pos,ios_base::beg);
    file.write((const char*)&record1,sizeRecord);
    inserimentoBST();
}

void calendar::inserimentoBST()
{   /* Controlla se c'e' un evento cancellato nel file BST se c'e' prende quella posizione
     * se non c'e' prende quella in fondo del file.Poi controlla se e' primo o no
     * se primo evento del file lo aggiunge e basta, se non e' il primo, scorre
     * usando SX o DX trovando un posto vuoto.*/
    ofstream bst("bst.txt",ios::binary | ios::in | ios::out);
    albero lettura; //struct per caricare data dal bst;
    bool trovato = false;
    bool primoEvento = true;
    int posizione; // posizione dentro il file del BST
    ifstream readbst("bst.txt",ios_base::app | ios_base::binary);
    readbst.seekg(0,ios_base::beg);
    while(!readbst.eof() && trovato==false) // con questo ciclo trovo se c'e' un evento cancellato per recuperare lo spazio
    {
        readbst.clear();
        readbst.read((char*)&lettura,sizeAlbero);
        if(!readbst.eof()) // perche' c'e' il problema che viene letto 2 volte ultimo elem
        {
            readbst.clear();
            primoEvento = false;
            if(lettura.cancellato)
            {
                trovato = true;
                posizione = (readbst.tellg()/sizeAlbero);
            }
        }
    }
    if(trovato) // se trova evento cancellato salva - la posizione in una variabile, altrimenti salva alla fine del file
    {
        bst.seekp(posizione*(sizeAlbero),ios_base::beg);
    }
    else
    {
        bst.seekp(0,ios_base::end);
        posizione = (bst.tellp()/sizeAlbero);
    }
    bst.write((const char*)&albero1,sizeAlbero); // salva nel file
    if(!primoEvento)
    {
        int puntatorePosizione(0);
        trovato = false;
        while(trovato == false)
        {
            readbst.clear(); // cancella gli errori;
            readbst.seekg((puntatorePosizione*sizeAlbero),ios::beg);
            readbst.read((char*)&lettura,sizeAlbero);
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
        readbst.close();
        bst.seekp((puntatorePosizione)*sizeAlbero,ios_base::beg);
        bst.write((const char*)&lettura,sizeAlbero);
        bst.close();
    }
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

void Agenda::cerca(int posizione)
{
    /** Avendo la posizione scorre il file bst trovando tutti eventi di un giorno*/
    albero bst;
    ifstream readBst("bst.txt");
    ifstream readCalendar("calendar.txt");
    bool trovato = false;
    readBst.seekg((posizione*(sizeAlbero)),ios_base::beg); // va sulla riga dove punta SX o DX
    readBst.read((char*)&bst,sizeAlbero);
    if(!readBst.eof())
    {
        if(!bst.cancellato)
        {
            if(bst.anno == y)
            {
                if(bst.mese == m)
                {
                    if(bst.giorno == d)
                    {
                        readCalendar.seekg((bst.pos*sizeRecord),ios_base::beg);
                        readCalendar.read((char*)&rec,sizeRecord);
                        ui->listWidget->addItem(rec.descrizone);
                        vetPosizioni->posizione = posizione;
                        vetPosizioni->next = new struct vet;
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
                if(confronto(bst) > 0) // se 1 a destra , se -1 a sinistra
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

int Agenda::confronto(albero data)
{
    if(y > data.anno)
    {
        return 1;
    }
    else if(y < data.anno)
    {
        return -1;
    }
    else if(m > data.mese)
    {
        return 1;
    }
    else if(m < data.mese)
    {
        return -1;
    }
    else if(d > data.giorno)
    {
        return 1;
    }
    else if(d < data.giorno)
    {
        return -1;
    }
    else if(hours > data.ore)
    {
        return 1;
    }
    else if(hours < data.ore)
    {
        return -1;
    }
    else if(min > data.min)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void Agenda::cancellamento()
{   /* Prende la posizione dalla strutture delle posizioni e va a leggere
     * la riga dal file BST salvandola nella struttura mette che evento e' cancellato
     * e la riscrive nel file BST su posizione precedente(cioe' riscrive la riga)
     * Usando la struttura appena letta prendo la posizione che punta al file che
     * contiene descrizioni e anche li metto che evento e' cancellato.
     */
    albero reading;
    vetPosizioni = &root;
    for(int i=0;i<posizioneItem;i++) // cosi trovo la posizione del evento che sta nel file bst
    {
        vetPosizioni = vetPosizioni->next;
    }
    ifstream calRead("calendar.txt");
    ofstream calWrite("calendar.txt",ios::binary | ios::in | ios::out);
    ifstream bstRead("bst.txt"); // per leggere dal file
    ofstream bstWrite("bst.txt",ios::binary | ios::in | ios::out); // per scrivere su file
    bstRead.seekg((vetPosizioni->posizione*sizeAlbero),ios_base::beg);
    bstRead.read((char*)&reading,sizeAlbero); // leggo evento da cancellare
    reading.cancellato = true; // e lo metto come cancellato
    bstWrite.seekp((vetPosizioni->posizione*sizeAlbero),ios_base::beg);
    bstWrite.write((const char*)&reading,sizeAlbero); // e riscrivo dentro file
    calRead.seekg((reading.pos*sizeRecord),ios_base::beg); // vado alla riga con descrizione
    calRead.read((char*)&rec,sizeRecord); // e anche qui metto come cancellato
    rec.cancellato = true;
    calWrite.seekp((reading.pos*sizeRecord),ios_base::beg);
    calWrite.write((const char*)&rec,sizeRecord); // e riscrivo;
    calRead.close();
    calWrite.close();
    bool trovato = false;
    bstRead.seekg(0,ios_base::beg);
    while(!trovato) // trovo la struttura che punta a evento cancellato e modifico il puntatore SX o DX
    {
        bstRead.read((char*)&reading,sizeAlbero);
        if(!bstRead.eof())
        {
            if(!reading.cancellato)
            {
                if(reading.sx == vetPosizioni->posizione)
                {
                    reading.sx = -1;
                    trovato = true;
                }
                else if(reading.dx == vetPosizioni->posizione)
                {
                    reading.dx = -1;
                    trovato = true;
                }
            }
        }
        else
        {
            trovato = true;
        }
    }
    bstWrite.seekp(bstRead.tellg(),ios_base::beg);
    bstWrite.write((const char*)&reading,sizeAlbero);
    bstRead.close();
    bstWrite.close();
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
    cambioColore(); // per cambiare il colore della giornata;
    ui->textEdit->clear(); // se utente pigia aggiungere si pulisce textEdit
}

void Agenda::cambioColore()
{
    // Per cambiare il colore dello sfondo della data
    QDate giorno = QDate(y,m,d); // il giorno
    QTextCharFormat colore; // il colore
    // lo sfondo diventa griggio
    colore.setBackground(QColor("#e0e0e0"));
    ui->calendar->setDateTextFormat(giorno,colore);
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
    /** Subito dopo della modifica prende il testo inserito della modifica
     * e lo salva nella stringa,leggo la posizione selezionata dal utente
     * nella listWidget e scorro la struttura fino ad elemento cercato.
     * Prendo la posizione che contiene la struttura e vado a leggere
     * la riga del file BST che ha quella posizione. Ottengo un'altra
     * struttura con i dati, avro la posizione della riga del file che
     * ha la descrizione.Leggo la riga e scambio 'descrizione'.
     */
    record rec;
    albero albero1;
    QString desc = item->text(); // salva nella stringa cosa ha scritto utente
    vetPosizioni = &root;
    for(int i=0;i<posizioneItem;i++)
    {
        vetPosizioni = vetPosizioni->next;
    }
    fstream file1;
    file1.open("bst.txt");
    file1.seekg(vetPosizioni->posizione,ios_base::beg); // punto ad elemento da modificare;
    file1.read((char*)&albero1,sizeAlbero);
    file1.close();
    file1.open("calendar.txt");
    file1.seekg(albero1.pos,ios_base::beg);
    file1.read((char*)&rec,sizeAlbero);
    QByteArray ba = desc.toLocal8Bit(); // converto QString to char;
    const char *str = ba.data(); // conver.
    strncpy(rec.descrizone,str,100);
    file1.seekp(albero1.pos,ios::beg);
    file1.write((const char*)&rec,sizeAlbero);
    file1.close();
}

void Agenda::on_listWidget_itemClicked(QListWidgetItem *item)
{   // Quando utente clicca sulla cella della listWidget,
    // prende la posizione del evento selezionato
    // e stampa nella textEdit ore e la descrizione del evento selezionato;
    posizioneItem = ui->listWidget->row(ui->listWidget->currentItem());
    cerca(0); // riempe la struttura
    ui->textEdit->setText(QString::number(rec.hour) + ":" + QString::number(rec.minutes) + " " + rec.descrizone);
    ui->buttonEdit->setDisabled(true); // si disabilita il tasto per aggiungere
    ui->pushButton->setDisabled(false); // abilito tasto di cancellazione;
}

calendar::~calendar()
{
    file.close();
}

void Agenda::on_hours_valueChanged(int arg1) // ore
{
    hours = arg1;
}

void Agenda::on_spinBox_2_valueChanged(int arg1) // minuti
{
    min = arg1;
}

void Agenda::on_pushButton_clicked() // bottone per cancellamento;
{ // Premendo tasto per cancellazione avviene il metodo cancellamento e si elimina dalla listWidget in diretta la riga da cancellare
    cancellamento();
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
}

void Agenda::svuota()
{
    // Serve per svuotare la struttura delle posizioni
    bool exit=false;
    int counter(0);
    while(!exit) // ciclo conta quantita' delle strutture;
    {
        if(vetPosizioni->posizione != -1)
        {
            vetPosizioni = vetPosizioni->next;
            counter++;
        }
        else
        {
            exit = true;
        }
    }
    if(counter != 0) // se non e' vuota la struttura fai:
    {
        vet *tmpRoot;
        tmpRoot = &root; // inizio delle strutture;
        tmpRoot->posizione = -1;
        tmpRoot = tmpRoot->next; // inizio dal secondo elem.
        for(int i(0);i<counter-1;i++) // cancella tutto fino in fondo;
        {
            vet *tmp;
            tmp = tmpRoot;
            if(i!=counter-2) // cosi non puntera a una zona che e' fuori;
            {
                tmpRoot = tmpRoot->next;
            }
            delete tmp;
        }
    }
}
