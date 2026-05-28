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
    prestiti lista_prestiti_copia = *lista_prestiti_persona; // Copia temporanea della lista prestiti

    if (nuovo_nome[0] != '\0')
        strcpy(nome_finale, nuovo_nome);
    if (nuovo_cognome[0] != '\0')
        strcpy(cognome_finale, nuovo_cognome);

    cancella_persona_dalla_lista(lista, cognome_attuale);

    if (inserisci_nuova_persona(lista, nome_finale, cognome_finale) == 1)
        return 1;
    persona p_modificata = cerca_persona_nella_lista(*lista, cognome_finale);
    if (p_modificata == NULL)
        return 1;

    prestiti *lista_prestiti_modificata = get_prestiti_della_persona(p_modificata);
    *lista_prestiti_modificata = lista_prestiti_copia;

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
                *lista = corrente->successivo; // Rimuove la testa della lista
            }
            else
            {
                precedente->successivo = corrente->successivo; // Rimuove l'elemento corrente
            }
            distruggi_persona(&corrente->p); // Libera la memoria della persona
            free(corrente);                  // Libera la memoria del nodo
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
        distruggi_persona(&da_cancellare->p); // Libera la memoria della persona
        free(da_cancellare);                  // Libera la memoria del nodo
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