#ifndef PRESTITI_H
#define PRESTITI_H
#include "prestito.h"

struct prestiti {
    prestito p;
    struct prestiti* successivo;
};

typedef struct prestiti* prestiti;


int crea_lista_prestiti(prestiti* lista);

int inserisci_nuovo_prestito(prestiti* lista, char data[], libro l_associato);

prestito cerca_prestito_nella_lista_per_data(prestiti lista, char data_chiave[]);

prestito cerca_prestito_nella_lista_per_libro(prestiti lista, libro l_associato);

int modifica_prestito_nella_lista(prestiti lista, char data_attuale[], char nuova_data[], libro nuovo_l_associato);

int cancella_prestito_dalla_lista(prestiti* lista, libro l_associato); // modifica da data a libro

int distruggi_lista_prestiti(prestiti* lista);

void stampa_lista_prestiti(prestiti lista);

#endif