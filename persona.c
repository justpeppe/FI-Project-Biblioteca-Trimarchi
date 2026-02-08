#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "persona.h"

int crea_persona(persona* nuova_persona, char nome[], char cognome[]) {
    
    if (nuova_persona == NULL || nome == NULL || cognome == NULL) {
        return 1; 
    }
    
    *nuova_persona = (persona)malloc(sizeof(struct persona));
    
    if (*nuova_persona == NULL) {
        return 1; 
    }
    
    strcpy((*nuova_persona)->nome, nome);
    strcpy((*nuova_persona)->cognome, cognome);
    (*nuova_persona)->lista_propri_prestiti = NULL; 
    
    return 0; 
}


int get_nome_persona(persona pers, char nome[]) {
    
    if (pers == NULL || nome == NULL) {
        return 1; 
    }
    
    strcpy(nome, pers->nome);
    return 0; 
}


int get_cognome_persona(persona pers, char cognome[]) {
    
    if (pers == NULL || cognome == NULL) {
        return 1; 
    }
    
    strcpy(cognome, pers->cognome);
    return 0; 
}


prestiti* get_prestiti_della_persona(persona pers) {
    
    if (pers == NULL) {
        return NULL;
    }

    // Restituisce l'indirizzo del puntatore alla lista, non una sua copia.
    return &(pers->lista_propri_prestiti);
}


int set_nome_persona(persona pers, char nuovo_nome[]) {
    
    if (pers == NULL || nuovo_nome == NULL) {
        return 1; 
    }
    
    strcpy(pers->nome, nuovo_nome);
    return 0; 
}


int set_cognome_persona(persona pers, char nuovo_cognome[]) {
    
    if (pers == NULL || nuovo_cognome == NULL) {
        return 1; 
    }
    
    strcpy(pers->cognome, nuovo_cognome);
    return 0; 
}


int distruggi_persona(persona* pers) {
    
    if (pers == NULL || *pers == NULL) {
        return 1; 
    }
    
    distruggi_lista_prestiti(&((*pers)->lista_propri_prestiti));
    free(*pers);
    *pers = NULL; 
    return 0; 
}


int stampa_persona(persona pers) {
    
    if (pers == NULL) {
        return 1;
    }
    
    printf("Utente: %s %s\n", pers->nome, pers->cognome);
    printf("Prestiti in corso:\n");
    stampa_lista_prestiti(pers->lista_propri_prestiti);
    
    return 0; 
}