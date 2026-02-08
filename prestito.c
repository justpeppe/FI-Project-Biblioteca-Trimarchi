#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prestito.h"

int crea_prestito(prestito* nuovo_prestito, char data[], libro l_associato) {

    *nuovo_prestito = (prestito)malloc(sizeof(struct prestito));
    
    if (*nuovo_prestito == NULL) {
        return 1;
    }
    
    strcpy((*nuovo_prestito)->data_prestito, data);
    (*nuovo_prestito)->libro_associato = l_associato;
    
    return 0;
}


int get_data_del_prestito(prestito p, char data[]) {
    
    if (p == NULL || data == NULL) {
        return 1; 
    }
    
    strcpy(data, p->data_prestito);
    return 0; 
}


libro get_libro_del_prestito(prestito p, libro* l_cerca) {
    if (p == NULL || l_cerca == NULL) {
        return NULL; 
    }
    
    *l_cerca = p->libro_associato;
    return *l_cerca; 
}


int set_data_del_prestito(prestito p, char nuova_data[]) {
    
    if (p == NULL || nuova_data == NULL) {
        return 1;
    }
    
    strcpy(p->data_prestito, nuova_data);
    return 0;
}


int set_libro_del_prestito(prestito p, libro nuovo_l_associato) {
    
    if (p == NULL || nuovo_l_associato == NULL) {
        return 1;
    }
    
    p->libro_associato = nuovo_l_associato;
    return 0;
}

int distruggi_prestito(prestito* p) {
    if (p == NULL || *p == NULL) {
        return 1;
    }
    
    free(*p);
    *p = NULL;
    return 0;
}

int stampa_prestito(prestito p) {
    if (p == NULL) {
        return 1;
    }

    char data[11];
    get_data_del_prestito(p, data);
    
    libro l;
    get_libro_del_prestito(p, &l);
    if (l == NULL) {
        return 1;  
    }
    
    char titolo[100];
    get_titolo(l, titolo);
    
    printf("Libro associato: %s\n", titolo);
    printf("Data: %s\n", data);
    
    return 0;
}
