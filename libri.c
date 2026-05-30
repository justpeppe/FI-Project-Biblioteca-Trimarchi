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

    *lista = NULL; // Imposta la lista a NULL (vuota)
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