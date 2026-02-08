#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "persone.h"

int crea_lista_persone(persone* lista) {
    
    if (lista == NULL) {
        return 1;
    }
    
    *lista = NULL; 
    return 0;
}


int inserisci_nuova_persona(persone* lista, char nome[], char cognome[]) {
    
    persona p_da_inserire;
    
    if (crea_persona(&p_da_inserire, nome, cognome) != 0) {
        return 1;
    }
    persone nuovo_nodo = (persone)malloc(sizeof(struct persone));
    
    if (nuovo_nodo == NULL) {
        distruggi_persona(&p_da_inserire);
        return 1;
    }
    
    nuovo_nodo->p = p_da_inserire;

    persone corrente = *lista;
    persone precedente = NULL;
    
    char cognome_corrente[50];
    while (corrente != NULL) {
        get_cognome_persona(corrente->p, cognome_corrente);
        if (strcmp(cognome, cognome_corrente) <= 0) break;
        precedente = corrente;
        corrente = corrente->successivo;
    }

    if (precedente == NULL) { // Inserimento in testa
        nuovo_nodo->successivo = *lista;
        *lista = nuovo_nodo;
    } else { // Inserimento in mezzo o in coda
        precedente->successivo = nuovo_nodo;
        nuovo_nodo->successivo = corrente;
    }

    return 0;
}


persona cerca_persona_nella_lista(persone lista, char cognome_chiave[]) {
    
    if (lista == NULL || cognome_chiave == NULL) {
        return NULL;
    }
    
    persone corrente = lista;
    
    while (corrente != NULL) {
        char cognome[50];
        get_cognome_persona(corrente->p, cognome);
        
        if (strcmp(cognome, cognome_chiave) == 0) {
            return corrente->p; 
        }
        
        corrente = corrente->successivo;
    }
    
    return NULL; 
}


int modifica_persona_nella_lista(persone lista, char cognome_attuale[], char nuovo_nome[], char nuovo_cognome[]) {
    
    if (lista == NULL || cognome_attuale == NULL || nuovo_nome == NULL || nuovo_cognome == NULL) {
        return 1;
    }
    
    persone corrente = lista;
    
    while (corrente != NULL) {
        char cognome[50];
        get_cognome_persona(corrente->p, cognome);
        
        if (strcmp(cognome, cognome_attuale) == 0) {
            set_nome_persona(corrente->p, nuovo_nome);
            set_cognome_persona(corrente->p, nuovo_cognome);
            return 0;
        }
        
        corrente = corrente->successivo;
    }
    
    return 1; 
}


int cancella_persona_dalla_lista(persone* lista, char cognome_chiave[]) {
    
    if (lista == NULL || *lista == NULL || cognome_chiave == NULL) {
        return 1;
    }
    
    persone corrente = *lista;
    persone precedente = NULL;
    
    while (corrente != NULL) {
        char cognome[50];
        get_cognome_persona(corrente->p, cognome);
        
        if (strcmp(cognome, cognome_chiave) == 0) {
            if (precedente == NULL) {
                *lista = corrente->successivo; // Rimuove la testa della lista
            } else {
                precedente->successivo = corrente->successivo; // Rimuove l'elemento corrente
            }
            distruggi_persona(&corrente->p); // Libera la memoria della persona
            free(corrente); // Libera la memoria del nodo
            return 0;
        }
        
        precedente = corrente;
        corrente = corrente->successivo;
    }
    
    return 1; 
}


int distruggi_lista_persone(persone* lista) {
    if (lista == NULL || *lista == NULL) {
        return 1;
    }
    
    persone corrente = *lista;
    
    while (corrente != NULL) {
        persone da_cancellare = corrente;
        corrente = corrente->successivo;
        distruggi_persona(&da_cancellare->p); // Libera la memoria della persona
        free(da_cancellare); // Libera la memoria del nodo
    }
    
    *lista = NULL;
    return 0;
}


void stampa_lista_persone(persone lista) {
    if (lista == NULL) {
        printf("La lista è vuota.\n");
        return;
    }
    
    persone corrente = lista;
    while (corrente != NULL) {
        stampa_persona(corrente->p);
        corrente = corrente->successivo;
    }
}