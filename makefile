#=============================================================
simulation: compile
	clear
	./master

compile:	master Nodo Utente

clean:
	ipcrm -a
	rm master
	rm Nodo
	rm Utente
	clear

run:
	clear
	./master

#=============================================================

CC = gcc -std=c89 -pedantic

#=============================================================
master: master.c
	${CC}	master.c -o master

Nodo: Nodo.c
	${CC}	Nodo.c -o Nodo

Utente: Utente.c
	${CC}	Utente.c -o Utente