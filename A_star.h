#include "structure.h"

int dedans(t_case *Liste,t_case v,int sizeX,int sizeY);

t_case voisin(t_case c,int choix,int sizeX,int sizeY,char **labData_2D,t_joueur tresor,int player);

t_case recherche(t_case *Liste,int sizeX,int sizeY,t_case c);

int *A_star(t_joueur p1,t_joueur tresor,int sizeX,int sizeY,char **labData_2D,int player);

