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

int salva_persone_su_file(persone lista, const char *nome_file)
{
    FILE *fp;
    int N = 0;
    persone corrente = lista;
    persone pila_appoggio = NULL;
    persone nodo_temp;

    // Apro il file in scrittura
    fp = fopen(nome_file, "w");
    if (fp == NULL)
    {
        printf("Errore: impossibile creare il file di salvataggio.\n");
        return 1;
    }

    // Scorro la lista originale, conto le persone e creo la pila d'appoggio
    while (corrente != NULL)
    {
        N = N + 1;

        nodo_temp = (persone)malloc(sizeof(struct persone));
        if (nodo_temp != NULL)
        {
            nodo_temp->p = corrente->p;

            // Mettendo pila_appoggio = nodo_temp al prossimo giro il successivo diventando
            // la pila_appoggio ho creato il meccanismo di pila LIFO
            nodo_temp->successivo = pila_appoggio;
            pila_appoggio = nodo_temp;
        }

        corrente = corrente->successivo;
    }

    // Scrivo in cima il numero totale di elementi sul file
    fprintf(fp, "%d\n", N);

    char nome[50];
    char cognome[50];
    persone da_cancellare;
    prestiti *p_lista;

    // Svuoto la pila e scrivo su file
    while (pila_appoggio != NULL)
    {

        get_nome_persona(pila_appoggio->p, nome);
        get_cognome_persona(pila_appoggio->p, cognome);

        // Scrivo ogni valore su una riga separata
        fprintf(fp, "%s\n", nome);
        fprintf(fp, "%s\n", cognome);
        
        p_lista = get_prestiti_della_persona(pila_appoggio->p);
        salva_prestiti_su_file(*p_lista, fp); 

        // Salvo il nodo attuale cosi poi lo libero
        da_cancellare = pila_appoggio;

        // Continuo lo scorrimento
        pila_appoggio = pila_appoggio->successivo;

        free(da_cancellare);
    }

    // Chiudo il file
    fclose(fp);
    return 0;
}

int carica_persone_da_file(persone *lista, const char *nome_file, libri lista_libri)
{
    FILE *fp;
    int N;

    // Apro il file in lettura
    fp = fopen(nome_file, "r");
    if (fp == NULL)
    {
        printf("Errore: impossibile aprire il file di caricamento.\n");
        return 1;
    }

    // Leggo la prima riga per sapere il numero di elementi da caricare
    if (fscanf(fp, "%d\n", &N) != 1)
    {
        fclose(fp);
        return 1;
    }

    char nome[50];
    char cognome[50];
    persone nuova_persona;
    prestiti *p_lista;

    for (int i = 0; i < N; i = i + 1)
    {

        fgets(nome, 50, fp);
        // Tolgo l'invio ('\n') alla fine della stringa usando un ciclo for
        for (int j = 0; nome[j] != '\0'; j = j + 1)
        {
            if (nome[j] == '\n')
            {
                nome[j] = '\0';
                break;
            }
        }

        fgets(cognome, 50, fp);
        for (int j = 0; cognome[j] != '\0'; j = j + 1)
        {
            if (cognome[j] == '\n')
            {
                cognome[j] = '\0';
                break;
            }
        }

        nuova_persona = (persone)malloc(sizeof(struct persone));

        if (nuova_persona != NULL)
        {

            if (crea_persona(&(nuova_persona->p), nome, cognome) == 0)
            {
                p_lista = get_prestiti_della_persona(nuova_persona->p);
                crea_lista_prestiti(p_lista);
                carica_prestiti_da_file(p_lista, fp, lista_libri); 
                
                // Inserimento in testa, più veloce di usare inserisci_persona perchè
                // con la pila l'elemento dopo sarà sempre più ""piccolo""" di quello attuale
                nuova_persona->successivo = *lista;
                *lista = nuova_persona;
            }
            else
            {
                // Se la creazione fallisce devo pulire la memoria allocata
                free(nuova_persona);
                fclose(fp);
                return 1;
            }
        }
        else
        {
            fclose(fp);
            return 1;
        }
    }

    // Chiudo il file
    fclose(fp);
    return 0;
}