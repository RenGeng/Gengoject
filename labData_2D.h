#include "structure.h"
#define JOUEUR 8
#define ADVER 7
#define TRESOR 6

char** init_lab(char *labData,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX, int sizeY);

void affichage_2D(char** labData_2D,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX,int sizeY);

void maj_lab(char **labData_2D,int mv,int val,t_joueur* p1,t_joueur* p2,t_joueur* tresor,int sizeX,int sizeY, int player);
