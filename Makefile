.SUFFIXES:.c

CC=gcc
CFLAGS= -Wall
LIBS=include
SRCS= $(LIBS)/arbin.c $(LIBS)/stateset.c $(LIBS)/queue.c $(LIBS)/thompson-utils.c $(LIBS)/determinisation.c $(LIBS)/minimisation.c

regexdesc :	regexdesc.c
	@echo debut compil c avec edition de liens
	$(CC) $(CFLAGS) -I$(LIBS) -o regexdesc regexdesc.c $(SRCS)
	$(CC) $(CFLAGS) -I$(LIBS) -o regexdesc.exe regexdesc.c $(SRCS)
	@echo fin compil : vous pouvez executer regexdesc ou regexdesc.exe

clean :
	rm regexdesc regexdesc.exe