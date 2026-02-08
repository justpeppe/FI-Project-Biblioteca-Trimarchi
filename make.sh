#!/bin/bash

if [ "$1" == "clean" ]; then
        rm -f *.o
        rm -f *.a
        rm -f biblioteca
else
        gcc -c -g libro.c
        gcc -c -g libri.c
        gcc -c -g persona.c
        gcc -c -g persone.c
        gcc -c -g prestito.c
        gcc -c -g prestiti.c
        ar -cr libbiblioteca.a libro.o libri.o persona.o persone.o prestito.o prestiti.o
        gcc -g biblioteca.c -L. -lbiblioteca -o biblioteca
fi
