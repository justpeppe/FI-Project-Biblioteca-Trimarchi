#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prestiti.h"

int crea_lista_prestiti(prestiti *lista)
{
    *lista = NULL;
    return 0;
}

int inserisci_nuovo_prestito(prestiti *lista, char data[], libro l_associato)
{

    prestito p_da_inserire;
    if (crea_prestito(&p_da_inserire, data, l_associato) != 0)
    {
        return 1;
    }

    prestiti nuovo_nodo = (prestiti)malloc(sizeof(struct prestiti));

    if (nuovo_nodo == NULL)
    {
        distruggi_prestito(&p_da_inserire); // Pulisco se fallisce
        return 1;
    }

    nuovo_nodo->p = p_da_inserire;

    prestiti corrente = *lista;
    prestiti precedente = NULL;

    char data_corrente[11];
    while (corrente != NULL)
    {
        get_data_del_prestito(corrente->p, data_corrente);
        if (strcmp(data, data_corrente) <= 0)
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

prestito cerca_prestito_nella_lista_per_data(prestiti lista, char data_chiave[])
{

    while (lista != NULL)
    {

        char data[11];
        get_data_del_prestito(lista->p, data);

        if (strcmp(data, data_chiave) == 0)
        {
            return lista->p; // Prestito trovato
        }

        lista = lista->successivo;
    }
    return NULL; // Prestito non trovato
}

prestito cerca_prestito_nella_lista_per_libro(prestiti lista, libro l_associato)
{

    while (lista != NULL)
    {

        libro libro_corrente;
        get_libro_del_prestito(lista->p, &libro_corrente);

        if (libro_corrente == NULL)
        {
            return NULL;
        }

        if (libro_corrente == l_associato)
        {
            return lista->p; // Prestito trovato
        }

        lista = lista->successivo;
    }
    return NULL;
}

int modifica_prestito_nella_lista(prestiti *lista, char data_attuale[], char nuova_data[], libro nuovo_l_associato)
{

    if (lista == NULL || data_attuale == NULL)
        return 1;

    prestito p = cerca_prestito_nella_lista_per_data(*lista, data_attuale);

    if (p == NULL)
    {
        return 1; // Prestito non trovato
    }

    // Salvo i valori correnti
    char data_finale[11];
    libro libro_attuale;
    get_data_del_prestito(p, data_finale);
    get_libro_del_prestito(p, &libro_attuale);

    libro libro_finale = libro_attuale;

    if (nuova_data != NULL && nuova_data[0] != '\0')
        strcpy(data_finale, nuova_data);
    if (nuovo_l_associato != NULL)
        libro_finale = nuovo_l_associato;

    // Cancello il vecchio prestito
    cancella_prestito_dalla_lista(lista, libro_attuale);

    // Reinserisco con i valori aggiornati
    if (inserisci_nuovo_prestito(lista, data_finale, libro_finale) != 0)
        return 1;

    return 0;
}

int cancella_prestito_dalla_lista(prestiti *lista, libro l_associato)
{

    prestiti corrente = *lista;
    prestiti precedente = NULL;

    while (corrente != NULL)
    {
        libro libro_corrente;
        get_libro_del_prestito(corrente->p, &libro_corrente);

        if (libro_corrente == l_associato)
        {
            if (precedente == NULL)
            {
                *lista = corrente->successivo;
            }
            else
            {
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

int distruggi_lista_prestiti(prestiti *lista)
{
    if (lista == NULL)
        return 1;

    prestiti corrente = *lista;

    while (corrente != NULL)
    {
        prestiti da_cancellare = corrente;
        corrente = corrente->successivo;

        distruggi_prestito(&da_cancellare->p); // Libero la memoria del prestito
        free(da_cancellare); // Libero la memoria della struttura Prestiti
    }

    *lista = NULL; // Imposto la lista a NULL
    return 0;
}

int stampa_lista_prestiti(prestiti lista)
{

    if (lista == NULL)
    {
        printf("Nessun prestito per questo utente.\n");
        return 1;
    }

    while (lista != NULL)
    {
        stampa_prestito(lista->p);
        lista = lista->successivo;
    }

    printf("\nFine della lista dei prestiti.\n");
    return 0;
}

int salva_prestiti_su_file(prestiti lista, FILE *fp)
{
    int N = 0;
    prestiti corrente = lista;
    prestiti pila_appoggio = NULL;
    prestiti nodo_temp;

    // Scorro la lista originale, conto i prestiti e creo la pila d'appoggio
    while (corrente != NULL)
    {
        N = N + 1;

        nodo_temp = (prestiti)malloc(sizeof(struct prestiti));
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

    char data[11];
    char titolo_libro[100];
    libro l_associato;
    prestiti da_cancellare;

    // Svuoto la pila e scrivo su file
    while (pila_appoggio != NULL)
    {

        get_data_del_prestito(pila_appoggio->p, data);
        get_libro_del_prestito(pila_appoggio->p, &l_associato);
        get_titolo(l_associato, titolo_libro); 

        // Scrivo ogni valore su una riga separata
        fprintf(fp, "%s\n", data);
        fprintf(fp, "%s\n", titolo_libro);

        // Salvo il nodo attuale cosi poi lo libero
        da_cancellare = pila_appoggio;

        // Continuo lo scorrimento
        pila_appoggio = pila_appoggio->successivo;

        free(da_cancellare);
    }

    return 0;
}

int carica_prestiti_da_file(prestiti *lista, FILE *fp, libri lista_libri)
{
    int N;

    // Leggo la prima riga per sapere il numero di elementi da caricare
    if (fscanf(fp, "%d\n", &N) != 1)
    {
        return 1;
    }

    char data[11];
    char titolo_libro[100];
    libro l_associato;
    prestiti nuovo_prestito;

    for (int i = 0; i < N; i = i + 1)
    {

        fgets(data, 11, fp);

        // Tolgo l'invio ('\n') alla fine della stringa usando un ciclo for
        for (int j = 0; data[j] != '\0'; j = j + 1)
        {
            if (data[j] == '\n')
            {
                data[j] = '\0';
                break;
            }
        }

        fgets(titolo_libro, 100, fp);
        for (int j = 0; titolo_libro[j] != '\0'; j = j + 1)
        {
            if (titolo_libro[j] == '\n')
            {
                titolo_libro[j] = '\0';
                break;
            }
        }

        l_associato = cerca_libro_nella_lista(lista_libri, titolo_libro);

        if (l_associato != NULL)
        {
            nuovo_prestito = (prestiti)malloc(sizeof(struct prestiti));

            if (nuovo_prestito != NULL)
            {

                if (crea_prestito(&(nuovo_prestito->p), data, l_associato) == 0)
                {
                    // Inserimento in testa, più veloce di usare inserisci_prestito perchè
                    // con la pila l'elemento dopo sarà sempre più ""piccolo""" di quello attuale
                    nuovo_prestito->successivo = *lista;
                    *lista = nuovo_prestito;
                }
                else
                {
                    // Se la creazione fallisce devo pulire la memoria allocata
                    free(nuovo_prestito);
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
    }

    return 0;
}
