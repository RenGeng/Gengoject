# Modèle de fichier Makefile  pour le fichier template.c
LIBDIR = /home/sasl/encad/brajard/projet/CGS_lib

# options de compilation
CC = gcc
CCFLAGS = -Wall -I $(LIBDIR)/include -g -lm
LIBS = -L $(LIBDIR)/lib
LDFLAGS = -lm -lcgs

# fichiers du projet
SRC = template.c labData_2D.c A_star.c
OBJ = $(SRC:.c=.o)
EXEC = template


# règle initiale
all: $(EXEC) $(EXEC2)

# dépendance des .h
template.o: structure.h labData_2D.h A_star.h
labData_2D.o: structure.h labData_2D.h
A_star.o: structure.h A_star.h

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

# règles supplémentaires
clean:
	rm -f *.o
rmproper:
	rm -f $(EXEC) *.o
