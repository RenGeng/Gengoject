#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "labyrinthAPI.h"
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct{  // structure joueur
   int x; //Coordonné x
   int y; //Coordonnée y
   int energie;
   char symbole; //Symbole du joueur
} t_joueur;

typedef struct{ // structure de la case
   int x,y;  // Coordonnée x y de la case
   int cost; // le coût pour aller à cette case
   int heuristique; // le coût totale pour arriver au trésor
   int xp,yp; // Coordonnée x y de la case précédente
   int explor; // 1 = exploré et 0 = inexploré
} t_case;

extern int debug;	/* hack to enable debug messages */

#endif
