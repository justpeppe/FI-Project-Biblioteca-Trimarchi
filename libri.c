#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libri.h"

int crea_lista_libri(libri *lista)
{
    if (lista == NULL)
        return 1;
    *lista = NULL;
    return 0;
}

int inserisci_nuovo_libro(libri *lista, char titolo[], char nome_autore[], char cognome_autore[])
{

    libri nuovo_libro = (libri)malloc(sizeof(struct libri));

    if (nuovo_libro == NULL)
    {
        return 1; // Errore di allocazione
    }

    if (crea_libro(&nuovo_libro->l, titolo, nome_autore, cognome_autore) != 0)
    {
        free(nuovo_libro);
        return 1;
    }

    char titolo_nuovo[100];
    get_titolo(nuovo_libro->l, titolo_nuovo);

    libri corrente = *lista;
    libri precedente = NULL;

    char titolo_corrente[100];
    while (corrente != NULL)
    {
        get_titolo(corrente->l, titolo_corrente);
        if (strcmp(titolo_corrente, titolo_nuovo) >= 0)
            break;

        precedente = corrente;
        corrente = corrente->successivo;
    }

    if (precedente == NULL)
    { // Inserimento in testa
        nuovo_libro->successivo = *lista;
        *lista = nuovo_libro;
    }
    else
    { // Inserimento in mezzo o in coda
        precedente->successivo = nuovo_libro;
        nuovo_libro->successivo = corrente;
    }

    return 0;
}

libro cerca_libro_nella_lista(libri lista, char titolo_chiave[])
{

    while (lista != NULL)
    {

        char titolo[100];
        get_titolo(lista->l, titolo);

        if (strcmp(titolo, titolo_chiave) == 0)
        {
            return lista->l; // Libro trovato
        }

        lista = lista->successivo;
    }

    return NULL; // Libro non trovato
}

// Se passo parametri vuoti vuol dire che non voglio effettuare modifiche
int modifica_libro_nella_lista(libri *lista, char titolo_attuale[],
                               char nuovo_titolo[], char nuovo_nome_autore[], char nuovo_cognome_autore[])
{

    libro l = cerca_libro_nella_lista(*lista, titolo_attuale);
    if (l == NULL)
        return 1;

    if (nuovo_nome_autore[0] != '\0')
        set_nome_autore(l, nuovo_nome_autore);
    if (nuovo_cognome_autore[0] != '\0')
        set_cognome_autore(l, nuovo_cognome_autore);

    // Se il titolo cambia, serve ri-ordinare la lista
    if (nuovo_titolo[0] != '\0')
    {
        set_titolo(l, nuovo_titolo);

        // Scollego il nodo dalla lista (senza liberare il libro)
        libri corrente = *lista;
        libri precedente = NULL;
        libri nodo_da_spostare = NULL;

        while (corrente != NULL)
        {
            if (corrente->l == l)
            {
                nodo_da_spostare = corrente;
                if (precedente == NULL)
                    *lista = corrente->successivo;
                else
                    precedente->successivo = corrente->successivo;
                break;
            }
            precedente = corrente;
            corrente = corrente->successivo;
        }

        if (nodo_da_spostare == NULL)
            return 1;

        // Reinserisco il nodo nella posizione corretta (ordinato per titolo)
        char titolo_nuovo[100];
        get_titolo(l, titolo_nuovo);

        libri pos_corrente = *lista;
        libri pos_precedente = NULL;
        char titolo_corrente[100];

        while (pos_corrente != NULL)
        {
            get_titolo(pos_corrente->l, titolo_corrente);
            if (strcmp(titolo_corrente, titolo_nuovo) >= 0)
                break;
            pos_precedente = pos_corrente;
            pos_corrente = pos_corrente->successivo;
        }

        if (pos_precedente == NULL)
        {
            nodo_da_spostare->successivo = *lista;
            *lista = nodo_da_spostare;
        }
        else
        {
            pos_precedente->successivo = nodo_da_spostare;
            nodo_da_spostare->successivo = pos_corrente;
        }
    }

    return 0;
}

int cancella_libro_dalla_lista(libri *lista, char titolo_chiave[])
{

    libri corrente = *lista;
    libri precedente = NULL;

    while (corrente != NULL)
    {

        char titolo[100];
        get_titolo(corrente->l, titolo);

        if (strcmp(titolo, titolo_chiave) == 0)
        {

            if (precedente == NULL)
            {
                *lista = corrente->successivo;
            }
            else
            {
                precedente->successivo = corrente->successivo;
            }

            distruggi_libro(&corrente->l);
            free(corrente);

            return 0;
        }

        precedente = corrente;
        corrente = corrente->successivo;
    }
    return 1; // Libro non trovato
}

int distruggi_lista_libri(libri *lista)
{
    if (lista == NULL)
        return 1;

    libri corrente = *lista;
    while (corrente != NULL)
    {

        libri da_cancellare = corrente;
        corrente = corrente->successivo;
        distruggi_libro(&da_cancellare->l);
        free(da_cancellare);
    }

    *lista = NULL;
    return 0;
}

void stampa_lista_libri(libri lista)
{
    if (lista == NULL)
    {
        printf("Nessun libro presente.\n");
        return;
    }

    while (lista != NULL)
    {
        stampa_libro(lista->l);
        lista = lista->successivo;
    }
}

int salva_libri_su_file(libri lista, const char *nome_file)
{
    FILE *fp;
    int N = 0;
    libri corrente = lista;
    libri pila_appoggio = NULL;
    libri nodo_temp;

    // Apro il file in scrittura
    fp = fopen(nome_file, "w");
    if (fp == NULL)
    {
        printf("Errore: impossibile creare il file di salvataggio.\n");
        return 1;
    }

    // Scorro la lista originale, conto i libri e creo la pila d'appoggio
    while (corrente != NULL)
    {
        N = N + 1;

        nodo_temp = (libri)malloc(sizeof(struct libri));
        if (nodo_temp != NULL)
        {
            nodo_temp->l = corrente->l;

            // Mettendo pila_appoggio = nodo_temp al prossimo giro il successivo diventando
            // la pila_appoggio ho creato il meccanismo di pila LIFO
            nodo_temp->successivo = pila_appoggio;
            pila_appoggio = nodo_temp;
        }

        corrente = corrente->successivo;
    }

    // Scrivo in cima il numero totale di elementi sul file
    fprintf(fp, "%d\n", N);

    char titolo[100];
    char nome[50];
    char cognome[50];
    libri da_cancellare;

    // Svuoto la pila e scrivo su file
    while (pila_appoggio != NULL)
    {

        get_titolo(pila_appoggio->l, titolo);
        get_nome_autore(pila_appoggio->l, nome);
        get_cognome_autore(pila_appoggio->l, cognome);

        // Scrivo ogni valore su una riga separata
        fprintf(fp, "%s\n", titolo);
        fprintf(fp, "%s\n", nome);
        fprintf(fp, "%s\n", cognome);

        // Salvo il nodo attuale cosi poi lo libero
        da_cancellare = pila_appoggio;

        // Continuo lo scorrimento
        pila_appoggio = pila_appoggio->successivo;

        free(da_cancellare);
    }

    // Chiudo il file
    fclose(fp);
    return 0;
}

int carica_libri_da_file(libri *lista, const char *nome_file)
{
    FILE *fp;
    int N;

    // Apro il file in lettura
    fp = fopen(nome_file, "r");
    if (fp == NULL)
    {
        printf("Errore: impossibile aprire il file di caricamento.\n");
        return 1;
    }

    // Leggo la prima riga per sapere il numero di elementi da caricare
    if (fscanf(fp, "%d\n", &N) != 1)
    {
        fclose(fp);
        return 1;
    }

    char titolo[100];
    char nome[50];
    char cognome[50];
    libri nuovo_libro;

    for (int i = 0; i < N; i = i + 1)
    {

        fgets(titolo, 100, fp);

        // Tolgo l'invio ('\n') alla fine della stringa usando un ciclo for
        for (int j = 0; titolo[j] != '\0'; j = j + 1)
        {
            if (titolo[j] == '\n')
            {
                titolo[j] = '\0';
                break;
            }
        }

        fgets(nome, 50, fp);
        for (int j = 0; nome[j] != '\0'; j = j + 1)
        {
            if (nome[j] == '\n')
            {
                nome[j] = '\0';
                break;
            }
        }

        fgets(cognome, 50, fp);
        for (int j = 0; cognome[j] != '\0'; j = j + 1)
        {
            if (cognome[j] == '\n')
            {
                cognome[j] = '\0';
                break;
            }
        }

        nuovo_libro = (libri)malloc(sizeof(struct libri));

        if (nuovo_libro != NULL)
        {

            if (crea_libro(&(nuovo_libro->l), titolo, nome, cognome) == 0)
            {
                // Inserimento in testa, più veloce di usare inserisci_libro perchè
                // con la pila l'elemento dopo sarà sempre più ""piccolo""" di quello attuale
                nuovo_libro->successivo = *lista;
                *lista = nuovo_libro;
            }
            else
            {
                // Se la creazione fallisce devo pulire la memoria allocata
                free(nuovo_libro);
                fclose(fp);
                return 1;
            }
        }
        else
        {
            fclose(fp);
            return 1;
        }
    }

    // Chiudo il file
    fclose(fp);
    return 0;
}