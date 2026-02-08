#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prestiti.h"

int crea_lista_prestiti(prestiti* lista) {
    *lista = NULL;
    return 0; 
}


int inserisci_nuovo_prestito(prestiti* lista, char data[], libro l_associato) {
    
    prestito p_da_inserire;
    if (crea_prestito(&p_da_inserire, data, l_associato) != 0) {
        return 1;  
    }
    
    prestiti nuovo_nodo = (prestiti)malloc(sizeof(struct prestiti));
    
    if (nuovo_nodo == NULL) {
        distruggi_prestito(&p_da_inserire); // Pulisci se fallisce
        return 1;
    }

    nuovo_nodo->p = p_da_inserire;
    
    prestiti corrente = *lista;
    prestiti precedente = NULL;

    char data_corrente[11];
    while (corrente != NULL) {
        get_data_del_prestito(corrente->p, data_corrente);
        if (strcmp(data, data_corrente) <= 0) break;
        
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


prestito cerca_prestito_nella_lista_per_data(prestiti lista, char data_chiave[]) {
    
    while (lista != NULL) {

        char data[11];
        get_data_del_prestito(lista->p, data);

        if (strcmp(data, data_chiave) == 0) {
            return lista->p; // Prestito trovato
        }

        lista = lista->successivo;
    }
    return NULL; // Prestito non trovato
}

prestito cerca_prestito_nella_lista_per_libro(prestiti lista, libro l_associato) {
   
    while (lista != NULL) {
        
        libro libro_corrente;
        get_libro_del_prestito(lista->p, &libro_corrente);

        if (libro_corrente == NULL) {
            return NULL;   
        }

        if (libro_corrente == l_associato) {
            return lista->p; // Prestito trovato
        }

        lista = lista->successivo;
    }
    return NULL;
}

int modifica_prestito_nella_lista(prestiti lista, char data_attuale[], char nuova_data[], libro nuovo_l_associato) {
    
    prestito p = cerca_prestito_nella_lista_per_data(lista, data_attuale);
    
    if (p == NULL) {
        return 1; // Prestito non trovato
    }

    if (set_data_del_prestito(p, nuova_data) != 0) {
        return 1;  
    }

    if (set_libro_del_prestito(p, nuovo_l_associato) != 0) {
        return 1;  
    }

    return 0; 
}


int cancella_prestito_dalla_lista(prestiti* lista, libro l_associato) {
    
    prestiti corrente = *lista;
    prestiti precedente = NULL;

    while (corrente != NULL) {
        libro libro_corrente;
        get_libro_del_prestito(corrente->p, &libro_corrente);

        if (libro_corrente == l_associato) {
            if (precedente == NULL) {
                *lista = corrente->successivo; 
            } else {
                precedente->successivo = corrente->successivo; 
            }
            
            distruggi_prestito(&corrente->p); 
            free(corrente); 
            return 0; 
        }
        
        precedente = corrente;
        corrente = corrente->successivo;
    }
    
    return 1;
}


int distruggi_lista_prestiti(prestiti* lista) {

    prestiti corrente = *lista;
    
    while (corrente != NULL) {
        prestiti da_cancellare = corrente;
        corrente = corrente->successivo;
    
        distruggi_prestito(&da_cancellare->p); // Libera la memoria del prestito
        free(da_cancellare); // Libera la memoria della struttura Prestiti
    }
    
    *lista = NULL; // Imposta la lista a NULL
    return 0; 
}


void stampa_lista_prestiti(prestiti lista) {
    
    if (lista == NULL) {
        printf("Nessun prestito per questo utente.\n");
        return;
    }
    
    while (lista != NULL) {
        stampa_prestito(lista->p);
        lista = lista->successivo;
    }

    printf("\nFine della lista dei prestiti.\n");
}

