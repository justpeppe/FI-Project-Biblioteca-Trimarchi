#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libro.h"

int crea_libro(libro* nuovo_libro, char titolo[], char nome_autore[], char cognome_autore[]) {  
    
    *nuovo_libro = (libro)malloc(sizeof(struct libro));
    
    if (*nuovo_libro == NULL) {
        return 1; // Errore di allocazione
    } 

    set_titolo(*nuovo_libro, titolo);
    set_nome_autore(*nuovo_libro, nome_autore);
    set_cognome_autore(*nuovo_libro, cognome_autore);

    return 0; 
}


int get_titolo(libro l, char titolo[]) {

    if (l == NULL || titolo == NULL) {
        return 1; 
    }

    strcpy(titolo, l->titolo);
    return 0; 
}


int get_nome_autore(libro l, char nome_autore[]) {

    if (l == NULL || nome_autore == NULL) {
        return 1; 
    }

    strcpy(nome_autore, l->nome_autore);
    return 0; 
}


int get_cognome_autore(libro l, char cognome_autore[]) {
    
    if (l == NULL || cognome_autore == NULL) {
        return 1; 
    }
    
    strcpy(cognome_autore, l->cognome_autore);
    return 0; 
}


int set_titolo(libro l, char nuovo_titolo[]) {
    
    if (l == NULL || nuovo_titolo == NULL) {
        return 1; 
    }
    
    strcpy(l->titolo, nuovo_titolo);
    return 0; 
}


int set_nome_autore(libro l, char nuovo_nome_autore[]) {
    
    if (l == NULL || nuovo_nome_autore == NULL) {
        return 1; // errore
    }
    
    strcpy(l->nome_autore, nuovo_nome_autore);
    return 0; 
}


int set_cognome_autore(libro l, char nuovo_cognome_autore[]) {
    
    if (l == NULL || nuovo_cognome_autore == NULL) {
        return 1; 
    }
    
    strcpy(l->cognome_autore, nuovo_cognome_autore);
    return 0; 
}


int distruggi_libro(libro* l) {
    
    if (*l == NULL) {
        return 1; 
    }
    
    free(*l);
    *l = NULL;
    return 0; 
}


int stampa_libro(libro l) {
    
    if (l == NULL) {
        return 1; 
    }
    
    printf("Titolo: %s\n", l->titolo);
    printf("Autore: %s %s\n", l->nome_autore, l->cognome_autore);
    return 0; 
}

