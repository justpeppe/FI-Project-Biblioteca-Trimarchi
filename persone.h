#ifndef PERSONE_H
#define PERSONE_H
#include "persona.h"

struct persone {
    persona p;
    struct persone* successivo;
};

typedef struct persone* persone;


int crea_lista_persone(persone* lista);

int inserisci_nuova_persona(persone* lista, char nome[], char cognome[]);

persona cerca_persona_nella_lista(persone lista, char cognome_chiave[]);

int modifica_persona_nella_lista(persone lista, char cognome_attuale[], char nuovo_nome[], char nuovo_cognome[]);

int cancella_persona_dalla_lista(persone* lista, char cognome_chiave[]);

int distruggi_lista_persone(persone* lista);

void stampa_lista_persone(persone lista);

#endif