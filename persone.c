#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "persone.h"

int crea_lista_persone(persone *lista)
{

    if (lista == NULL)
    {
        return 1;
    }

    *lista = NULL;
    return 0;
}

int inserisci_nuova_persona(persone *lista, char nome[], char cognome[])
{

    persona p_da_inserire;

    if (crea_persona(&p_da_inserire, nome, cognome) != 0)
    {
        return 1;
    }
    persone nuovo_nodo = (persone)malloc(sizeof(struct persone));

    if (nuovo_nodo == NULL)
    {
        distruggi_persona(&p_da_inserire);
        return 1;
    }

    nuovo_nodo->p = p_da_inserire;

    persone corrente = *lista;
    persone precedente = NULL;

    char cognome_corrente[50];
    while (corrente != NULL)
    {
        get_cognome_persona(corrente->p, cognome_corrente);
        if (strcmp(cognome, cognome_corrente) <= 0)
            break;
        precedente = corrente;
        corrente = corrente->successivo;
    }

    if (precedente == NULL)
    { // Inserimento in testa
        nuovo_nodo->successivo = *lista;
        *lista = nuovo_nodo;
    }
    else
    { // Inserimento in mezzo o in coda
        precedente->successivo = nuovo_nodo;
        nuovo_nodo->successivo = corrente;
    }

    return 0;
}

persona cerca_persona_nella_lista(persone lista, char cognome_chiave[])
{

    if (lista == NULL || cognome_chiave == NULL)
    {
        return NULL;
    }

    persone corrente = lista;

    while (corrente != NULL)
    {
        char cognome[50];
        get_cognome_persona(corrente->p, cognome);

        if (strcmp(cognome, cognome_chiave) == 0)
        {
            return corrente->p;
        }

        corrente = corrente->successivo;
    }

    return NULL;
}

int modifica_persona_nella_lista(persone *lista, char cognome_attuale[], char nuovo_nome[], char nuovo_cognome[])
{

    if (lista == NULL || cognome_attuale == NULL || nuovo_nome == NULL || nuovo_cognome == NULL)
    {
        return 1;
    }

    persona p = cerca_persona_nella_lista(*lista, cognome_attuale);
    if (p == NULL)
    {
        return 1;
    }

    char nome_finale[50], cognome_finale[50];
    get_nome_persona(p, nome_finale);
    get_cognome_persona(p, cognome_finale);

    prestiti *lista_prestiti_persona = get_prestiti_della_persona(p);
    prestiti lista_prestiti_copia = NULL;
    if (lista_prestiti_persona != NULL)
    {
        lista_prestiti_copia = *lista_prestiti_persona;
        set_prestiti_della_persona(p, NULL); // Scollego la lista dei prestiti dalla persona prima di modificarla
    }

    if (nuovo_nome[0] != '\0')
        strcpy(nome_finale, nuovo_nome);
    if (nuovo_cognome[0] != '\0')
        strcpy(cognome_finale, nuovo_cognome);

    cancella_persona_dalla_lista(lista, cognome_attuale);

    if (inserisci_nuova_persona(lista, nome_finale, cognome_finale) != 0)
    {
        if (lista_prestiti_copia != NULL)
        {
            distruggi_lista_prestiti(&lista_prestiti_copia);
        }
        return 1;
    }

    persona p_modificata = cerca_persona_nella_lista(*lista, cognome_finale);
    if (p_modificata == NULL)
    {
        if (lista_prestiti_copia != NULL)
        {
            distruggi_lista_prestiti(&lista_prestiti_copia);
        }
        return 1;
    }

    set_prestiti_della_persona(p_modificata, lista_prestiti_copia);

    return 0;
}

int cancella_persona_dalla_lista(persone *lista, char cognome_chiave[])
{

    if (lista == NULL || *lista == NULL || cognome_chiave == NULL)
    {
        return 1;
    }

    persone corrente = *lista;
    persone precedente = NULL;

    while (corrente != NULL)
    {
        char cognome[50];
        get_cognome_persona(corrente->p, cognome);

        if (strcmp(cognome, cognome_chiave) == 0)
        {
            if (precedente == NULL)
            {
                *lista = corrente->successivo; // Rimuovo la testa della lista
            }
            else
            {
                precedente->successivo = corrente->successivo; // Rimuovo l'elemento corrente
            }
            distruggi_persona(&corrente->p); // Libero la memoria della persona
            free(corrente);                  // Libero la memoria del nodo
            return 0;
        }

        precedente = corrente;
        corrente = corrente->successivo;
    }

    return 1;
}

int distruggi_lista_persone(persone *lista)
{
    if (lista == NULL || *lista == NULL)
    {
        return 1;
    }

    persone corrente = *lista;

    while (corrente != NULL)
    {
        persone da_cancellare = corrente;
        corrente = corrente->successivo;
        distruggi_persona(&da_cancellare->p); // Libero la memoria della persona
        free(da_cancellare);                  // Libero la memoria del nodo
    }

    *lista = NULL;
    return 0;
}

int stampa_lista_persone(persone lista)
{
    if (lista == NULL)
    {
        printf("La lista è vuota.\n");
        return 1;
    }

    persone corrente = lista;
    while (corrente != NULL)
    {
        stampa_persona(corrente->p);
        corrente = corrente->successivo;
    }

    return 0;
}

int is_libro_in_prestito_nella_lista(persone lista, libro l)
{
    if (lista == NULL || l == NULL)
    {
        return 0;
    }

    persone corrente = lista;

    while (corrente != NULL)
    {
        prestiti *p_lista_prestiti = get_prestiti_della_persona(corrente->p);
        if (cerca_prestito_nella_lista_per_libro(*p_lista_prestiti, l) != NULL)
        {
            return 1;
        }
        corrente = corrente->successivo;
    }

    return 0;
}

persona trova_persona_per_libro(persone lista, libro l)
{
    if (lista == NULL || l == NULL)
    {
        return NULL;
    }

    persone corrente = lista;

    while (corrente != NULL)
    {
        prestiti *p_lista_prestiti = get_prestiti_della_persona(corrente->p);
        if (cerca_prestito_nella_lista_per_libro(*p_lista_prestiti, l) != NULL)
        {
            return corrente->p;
        }
        corrente = corrente->successivo;
    }

    return NULL;
}

int salva_persone_su_file(persone lista, const char *nome_file) {
    FILE *fp = fopen(nome_file, "w");
    if (fp == NULL) return 1;

    int N = 0;
    persone corrente = lista;
    persone pila = NULL;

    while (corrente != NULL) {
        N++;
        persone nuovo = (persone)malloc(sizeof(struct persone));
        if (nuovo != NULL) {
            nuovo->p = corrente->p;
            nuovo->successivo = pila;
            pila = nuovo;
        }
        corrente = corrente->successivo;
    }

    fprintf(fp, "%d\n", N);

    while (pila != NULL) {
        persone estratto = pila;
        char nome[50], cognome[50];
        prestiti *p_lista;
        
        get_nome_persona(estratto->p, nome);
        get_cognome_persona(estratto->p, cognome);
        
        fprintf(fp, "%s\n%s\n", nome, cognome);
        
        p_lista = get_prestiti_della_persona(estratto->p);
        salva_prestiti_su_file(*p_lista, fp);
        
        pila = pila->successivo;
        free(estratto);
    }
    
    fclose(fp);
    return 0;
}

int carica_persone_da_file(persone *lista, const char *nome_file, libri lista_libri) {
    FILE *fp = fopen(nome_file, "r");
    if (fp == NULL) return 1;

    distruggi_lista_persone(lista);
    crea_lista_persone(lista);

    int N;
    if (fscanf(fp, "%d\n", &N) != 1) {
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        char nome[50], cognome[50];
        int j;
        prestiti *p_lista;
        
        if (fgets(nome, sizeof(nome), fp) != NULL) {
            j = 0;
            while (nome[j] != '\n' && nome[j] != '\0') { j++; }
            nome[j] = '\0';
        }
        
        if (fgets(cognome, sizeof(cognome), fp) != NULL) {
            j = 0;
            while (cognome[j] != '\n' && cognome[j] != '\0') { j++; }
            cognome[j] = '\0';
        }

        persone nuovo = (persone)malloc(sizeof(struct persone));
        if (nuovo != NULL) {
            if (crea_persona(&(nuovo->p), nome, cognome) == 0) {
                
                p_lista = get_prestiti_della_persona(nuovo->p);
                crea_lista_prestiti(p_lista);
                carica_prestiti_da_file(p_lista, fp, lista_libri);
                
                nuovo->successivo = *lista;
                *lista = nuovo;
            } else {
                free(nuovo);
            }
        }
    }

    fclose(fp);
    return 0;
}