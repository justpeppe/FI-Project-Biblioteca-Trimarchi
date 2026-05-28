#ifndef LIBRI_H
#define LIBRI_H
#include "libro.h"

struct libri {
    libro l;
    struct libri* successivo;
};

typedef struct ibri* libri;


int crea_lista_libri(libri* lista);

int inserisci_nuovo_libro(libri* lista, char titolo[], char nome_autore[], char cognome_autore[]);

libro cerca_libro_nella_lista(libri lista, char titolo_chiave[]);

int modifica_libro_nella_lista(libri* lista, char titolo_attuale[], char nuovo_titolo[], char nuovo_nome_autore[], char nuovo_cognome_autore[]);

int cancella_libro_dalla_lista(libri* lista, char titolo_chiave[]);

int distruggi_lista_libri(libri* lista);

void stampa_lista_libri(libri lista);

#endif