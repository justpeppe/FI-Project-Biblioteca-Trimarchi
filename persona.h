#ifndef PERSONA_H
#define PERSONA_H
#include "prestiti.h"

struct persona {
    char nome[50];
    char cognome[50];
    prestiti lista_propri_prestiti;
};

typedef struct persona* persona;


int crea_persona(persona* nuova_persona, char nome[], char cognome[]);

int get_nome_persona(persona pers, char nome[]);

int get_cognome_persona(persona pers, char cognome[]);

prestiti* get_prestiti_della_persona(persona pers); // modifica 

int set_nome_persona(persona pers, char nuovo_nome[]);

int set_cognome_persona(persona pers, char nuovo_cognome[]);

int distruggi_persona(persona* pers);

int stampa_persona(persona pers);

#endif