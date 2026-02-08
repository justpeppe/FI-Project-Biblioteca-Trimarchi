#ifndef PRESTITO_H
#define PRESTITO_H
#include "libro.h"

struct prestito {
    char data_prestito[11]; // Formato AAAA-MM-GG
    libro libro_associato;
};

typedef struct prestito* prestito;


int crea_prestito(prestito* nuovo_prestito, char data[], libro l_associato);

int get_data_del_prestito(prestito p, char data[]);

libro get_libro_del_prestito(prestito p, libro* l_cerca); // modifica aggiunta di libro* l_cerca

int set_data_del_prestito(prestito p, char nuova_data[]);

int set_libro_del_prestito(prestito p, libro nuovo_l_associato);

//int aggiorna_puntatore_libro_prestito(prestito p, libro vecchio_libro, libro nuovo_libro);

int distruggi_prestito(prestito* p);

int stampa_prestito(prestito p);

#endif