//
// TEMPLATE 
//
//
// Permet de jouer un seul tour (en ne faisant rien s'il commence ou en 
// réceptionnant le coup de l'adversaire s'il ne commence pas) 
// et termine le jeu.
// Ce programme vous sert de base pour construire votre propre programme
//< pos[p1.y][p1.x]



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "labyrinthAPI.h"
#include <unistd.h>
#include <string.h>
#include <math.h>
#define JOUEUR 8
#define ADVER 7
#define TRESOR 6

typedef struct{
   int x; //Coordonné x
   int y; //Coordonnée y
   int energie;
   char symbole; //Symbole du joueur
} t_joueur;

typedef struct{
   int x,y;  // Coordonnée x y de la case
   int cost; // le coût pour aller à cette case
   int heuristique; // le coût totale pour arriver au trésor
   int xp,yp; // Coordonnée x y de la case précédente
   int explor; // 1 = exploré et 0 = inexploré
} t_case;

extern int debug;	/* hack to enable debug messages */


char** init_lab(char *labData,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX, int sizeY)
{
   /*initialisation du tableau 2D */
   char** tabData;
   int i,j;
   tabData = (char**) malloc(sizeY*sizeof(char*));
   for (i=0;i<sizeY;i++) tabData[i] = (char*) malloc(sizeX*sizeof(char));
   for (i=0;i<sizeY;i++)
   {
      for (j=0;j<sizeX;j++) 
      {
	 if((i==p1.y && j==p1.x) || (i==p2.y && j==p2.x) || (i==sizeY/2 && j==sizeX/2))
	 {
	    if((i==p1.y && j==p1.x)) tabData[i][j]=JOUEUR;
	    else if((i==p2.y && j==p2.x)) tabData[i][j]=ADVER;
	    else if((i==tresor.y && j==tresor.x)) tabData[i][j]=TRESOR;
	    // putchar(tabData[i][j]);
	    //printf("|");
	 }
	 else
	 {	    
	    tabData[i][j] = labData[i*sizeX+j];
	    //printf("%d|",tabData[i][j]);
	 }
      }
      printf("\n");
   }
   return tabData;
}

void affichage_2D(char** labData_2D,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX,int sizeY)
{
   int i,j;   
   for(i=0;i<sizeY;i++)
   {
      for(j=0;j<sizeX;j++)
      {
	 if(i==p1.y && j==p1.x) printf(">|");
	 else if(i==p2.y && j==p2.x) printf("@|");
	 else if(i==tresor.y && j==tresor.x) printf("T|");
	 else 
	    {
	       //putchar(labData_2D[i][j]);
	       //printf("|");
	       printf("%d|",labData_2D[i][j]);
      
	    }
      }
      printf("\n");
   }
}

void maj_lab(char **labData_2D,int mv,int val,t_joueur* p1,t_joueur* p2,t_joueur* tresor,int sizeX,int sizeY, int player)
{
   int i;
   char temp;
   
   switch(mv)
   {
      case 0: //Bouger une ligne vers la gauche	 
	 if (p2->y == val)
	 { /* Si l'adversaire est aussi sur la ligne */
	    labData_2D[p2->y][p2->x] = 0;
	    if (p2 -> x == 0) p2->x = sizeX-1;
	    else p2->x--;
	 }
	 if (tresor->y == val)
	 { /* Si le trésor est aussi sur la ligne */
	    labData_2D[tresor->y][tresor->x] = 0;
	    if (tresor->x == 0) tresor->x = sizeX-1;
	    else tresor->x--;
	 }
	 if (p1->y == val)
	 { /* Si on est sur la ligne */
	    labData_2D[p1->y][p1->x] = 0;
	    if (p1->x == 0) p1->x = sizeX-1;
	    else p1->x--;
	 }
	 temp = labData_2D[val][0];
	 for(i=0;i<sizeX-1;i++) labData_2D[val][i]=labData_2D[val][i+1];
	 labData_2D[val][sizeX-1]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;   
	 
      case 1: //Bouger une ligne vers la droite   	  	
	 if (p2->y == val)
	 { /* Si l'adversaire est aussi sur la ligne */
	    labData_2D[p2->y][p2->x] = 0;
	    if (p2->x == sizeX-1) p2->x = 0;
	    else p2->x++;
	 }
	 if (tresor->y == val)
	 { /* Si le trésor est aussi sur la ligne */
	    labData_2D[tresor->y][tresor->x] = 0;
	    if (tresor->x == sizeX-1) tresor->x = 0;
	    else tresor->x++;
	 }
	 if (p1->y == val)
	 { /* Si on est sur la ligne */
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->x == sizeX-1) p1->x = 0;
   	    else p1->x++;
	 }
	 temp = labData_2D[val][sizeX-1];
	 for(i=sizeX-1;i>0;i--) labData_2D[val][i]=labData_2D[val][i-1];
	 labData_2D[val][0]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x] = TRESOR;
	 break;

      case 2: //Bouger une colonne vers le haut  	  
   	 if (p2->x == val)
	 { /* Si l'adversaire est aussi sur la ligne */
	    labData_2D[p2->y][p2->x] = 0;
	    if (p2 -> y == 0) p2->y = sizeY-1;
	    else p2->y--;
	 }
	 if (tresor->x == val)
	 { /* Si le trésor est aussi sur la ligne */
	    labData_2D[tresor->y][tresor->x] = 0;
	    if (tresor->y == 0) tresor->y = sizeY-1;
	    else tresor->y--;
	 }
	 if (p1->x == val)
	 { /* Si on est sur la ligne */
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->y == 0) p1->y = sizeY-1;
   	    else p1->y--;
	 }
	 temp = labData_2D[0][val];
	 for(i=0;i<sizeY-1;i++) labData_2D[i][val]=labData_2D[i+1][val];
	 labData_2D[sizeY-1][val]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;
	 
      case 3: //Bouger une colonne vers le bas   	 
	 if (p2->x == val)
	 { /* Si l'adversaire est aussi sur la ligne */
	    labData_2D[p2->y][p2->x] = 0;
	    if (p2 -> y == sizeY-1) p2->y = 0;
	    else p2->y++;
	 }
	 if (tresor->x == val)
	 { /* Si le trésor est aussi sur la ligne */
	    labData_2D[tresor->y][tresor->x] = 0;
	    if (tresor->y == sizeY-1) tresor->y = 0;
	    else tresor->y++;
	 }
	 if (p1->x == val)
	 { /* Si on est sur la ligne */
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->y == sizeY-1) p1->y = 0;
   	    else p1->y++;
	 }
	 temp = labData_2D[sizeY-1][val];
	 for(i=sizeY-1;i>0;i--) labData_2D[i][val]=labData_2D[i-1][val];
	 labData_2D[0][val]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;

      case 4: // Déplacement vers le haut
   	 if (player == 1) // quand c'est l'adversaire de jouer
   	 {
   	    labData_2D[p2->y][p2->x] = 0;
   	    if (p2->y == 0) p2->y = sizeY-1;
   	    else p2->y --;
   	    labData_2D[p2->y][p2->x] = ADVER;
   	 }
   	 else  // quand c'est nous qui joue
   	 {
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->y == 0) p1->y = sizeY-1;
   	    else p1->y --;
   	    labData_2D[p1->y][p1->x] = JOUEUR;
   	 }
   	 break;
      case 5: // Déplacement vers la bas
   	 if (player == 1)
   	 {
   	    labData_2D[p2->y][p2->x] = 0;
   	    if (p2->y == sizeY-1) p2->y = 0;
   	    else p2->y ++;
   	    labData_2D[p2->y][p2->x] = ADVER;
   	 }
   	 else  // quand c'est nous qui joue
   	 {
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->y == sizeY-1) p1->y = 0;
   	    else p1->y ++;
   	    labData_2D[p1->y][p1->x] = JOUEUR;
   	 }
   	 break;
      case 6: // Déplacement vers la gauche
   	 if (player == 1)
   	 {
   	    labData_2D[p2->y][p2->x] = 0;
   	    if (p2->x == 0) p2->x = sizeX-1;
   	    else p2->x --;
   	    labData_2D[p2->y][p2->x] = ADVER;
   	 }
   	 else  // quand c'est nous qui joue
   	 {
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->x == 0) p1->x = sizeX-1;
   	    else p1->x --;
   	    labData_2D[p1->y][p1->x] = JOUEUR;
   	 }
   	 break;
      case 7: // Déplacement vers la droite
   	 if (player == 1)
   	 {
   	    labData_2D[p2->y][p2->x] = 0;
   	    if (p2->x == sizeX-1) p2->x = 0;
   	    else p2->x ++;
   	    labData_2D[p2->y][p2->x] = ADVER;
   	 }
   	 else  // quand c'est nous qui joue
   	 {
   	    labData_2D[p1->y][p1->x] = 0;
   	    if (p1->x == sizeX-1) p1->x = 0;
   	    else p1->x ++;
   	    labData_2D[p1->y][p1->x] = JOUEUR;
   	 }
   	 break;
   } 
}

/* int list_non_vide(t_case* openList, int sizeX, int sizeY){ */
/*    int i; */
/*    for (i = 0; i<sizeX*sizeY; i++) { */
/*       if (openList[i].x > -1) return 1; //pas vide */
/*    } */
/*    return 0; //vide */
/* } */

/* t_case* plus_petit(t_case* openList, int sizeX, int sizeY){ */
/*    int i; */
/*    t_case* c = openList; */
/*    for (i=0; i<sizeX*sizeY;i++){ */
/*       //printf("\n petit"); */
/*       if ( (c->cost + c->heuristique) > (openList[i].cost + openList[i].heuristique)) c = openList+i; */
/*    } */
/*    return c; */
/* } */

/* t_case calcule_voisin(t_case c, int x, int y, t_joueur tresor){ */
/*    t_case v; 			/\* Le voisin de c *\/ */
/*    v.x = x; */
/*    v.y = y; */
/*    v.cost = c.cost+1; */
/*    v.heuristique = v.cost + abs( (tresor.x - v.x) - (tresor.y - v.y)); */
/*    v.xp = c.x; */
/*    v.yp = c.y; */
/*    return v; */
/* } */
/* t_case recherche(t_case c, t_case* closedList, int sizeX,int sizeY){ */
/*    int i; */
/*    t_case precedent; */
/*    for (i=0; i<sizeX*sizeY;i++){ */
/*       if(closedList[i].x == c.xp && closedList[i].y == c.yp) precedent = closedList[i]; */
/*    } */
/*    return precedent; */
/* } */
/* int* calcule_chemin(t_case c, t_case* closedList, int sizeX,int sizeY, t_joueur p1){ */
/*    int* tab = (int*)calloc(sizeX*sizeY, sizeof(int)); */
/*    int comp_tab = sizeX*sizeY-1; */
/*    while(c.x != p1.x && c.y != p1.y){ */
/*       if(c.x - c.xp == 0 && c.y-c.yp == 1){ */
/* 	 tab[comp_tab--] = MOVE_DOWN; */
/* 	 c = recherche(c,closedList,sizeX,sizeY); */
/*       } */
/*       else if (c.x -c.xp == 0 && c.y - c.yp == -1 ) { */
/* 	 tab[comp_tab--] = MOVE_UP; */
/* 	 c = recherche(c,closedList,sizeX,sizeY); */
/*       } */
/*       else if (c.x - c.xp == 1 && c.y - c.yp == 0) { */
/* 	 tab[comp_tab--] = MOVE_RIGHT; */
/* 	 c = recherche(c,closedList,sizeX,sizeY); */
/*       } */
/*       else if (c.x - c.xp == -1 && c.y - c.yp == 0) { */
/* 	 tab[comp_tab--] = MOVE_LEFT; */
/* 	 c = recherche(c,closedList,sizeX,sizeY); */
/*       } */
/*    } */
/*    return tab; */
/* } */


/* int* A_star(t_joueur p1, t_joueur tresor,char ** labData_2D,int sizeX, int sizeY){ */
/*    t_case * openList = (t_case *)calloc(sizeX * sizeY,sizeof(t_case)); */
/*    t_case * closedList = (t_case *)calloc(sizeX * sizeY,sizeof(t_case)); */
/*    t_case * petit; */
/*    t_case c = {.x = p1.x, .y = p1.y, .cost = 0, .heuristique = 0, .xp = -1, .yp = -1}; /\* Initialisation du joueur *\/ */
/*    int* tab = (int*)calloc(sizeX*sizeY, sizeof(int)); */
/*    int comp_closed = 0; */
/*    int comp_open = 1; */
/*    int x,y,i,k=0; 			/\* Pour le calcule des voisins *\/ */
/*    openList[0] = c; */
   
/*    for(i=1;i<sizeX*sizeY;i++) openList[i].cost=5000; */
/*    while(list_non_vide(openList, sizeX,sizeY)){ */
/*       printf("\nsizeX*sizeY=%d et k=%d",sizeX*sizeY,k); */
/*       k+=1; */
/*       printf("\nc.y=%d c.x=%d",c.y,c.x); */
/*       petit = plus_petit(openList, sizeX, sizeY); */
/*       c = *petit; */
/*       printf("\nc.y=%d c.x=%d",c.y,c.x); */
/*       petit->cost = 1000;	  /\* Equivalent à effacer (je pense) *\/ */
/*       petit->heuristique = 1000; /\* Equivalent à effacer (je pense)*\/ */
/*       closedList[comp_closed++] = c; */
/*       if (c.x == tresor.x && c.y == tresor.y){ */
/* 	 tab = calcule_chemin(c,closedList,sizeX,sizeY,p1); */
/* 	 return tab; */
/*       } */
     
/*       else { */
/* 	 //printf("\n balbalbal :%d %d",c.y,c.x); */
/* 	 printf("\nlabData_2D[c.y][c.x]=%d", labData_2D[c.y][c.x]); */
/* 	 if ((int)labData_2D[c.y-1][c.x] != 1){ /\* Si le voisin d'en haut n'est pas un mur *\/ */
/* 	    //printf("\npas un mur 1"); */
/* 	    y = c.y-1; */
/* 	    x = c.x; */
/* 	    openList[comp_open++] = calcule_voisin(c,x,y,tresor); */
/* 	    //printf("\n haut: %d", openList[comp_open-1].cost); */
/* 	 } */
/* 	 if ((int)labData_2D[c.y+1][c.x] != 1){ /\* Si le voisin d'en bas n'est pas un mur *\/ */
/* 	    //printf("\npas un mur 2"); */
/* 	    y = c.y+1; */
/* 	    x = c.x; */
/* 	    openList[comp_open++] = calcule_voisin(c,x,y,tresor); */
/* 	    //printf("\n bas: %d", openList[comp_open-1].cost); */
/* 	 } */
/* 	 if ((int)labData_2D[c.y][c.x-1] != 1){ /\* Si le voisin de gauche n'est pas un mur *\/ */
/* 	    //printf("\npas un mur 3"); */
/* 	    y = c.y; */
/* 	    x = c.x-1; */
/* 	    openList[comp_open++] = calcule_voisin(c,x,y,tresor); */
/* 	    //printf("\n gauche: %d", openList[comp_open-1].cost); */
/* 	 } */
/* 	 if ((int)labData_2D[c.y][c.x+1] != 1){ /\* Si le voisin de droite n'est pas un mur *\/ */
/* 	    //printf("\npas un mur 4"); */
/* 	    y = c.y; */
/* 	    x = c.x+1; */
/* 	    openList[comp_open++] = calcule_voisin(c,x,y,tresor); */
/* 	    //printf("\n droite: %d", openList[comp_open-1].cost); */
/* 	 } */
/*       } */
/*    } */
/*    return NULL; */
/* } */

int dedans(t_case *Liste,t_case v,int sizeX,int sizeY) //vérifie que v est dans la liste ou pas
{
   int i;
   for(i=0;i<sizeX*sizeY;i++) {if(Liste[i].x==v.x && Liste[i].y==v.y) return 1;} //dedans
   return 0; //pas dedans
}

t_case voisin(t_case c,int choix,int sizeX,int sizeY,char **labData_2D,t_joueur tresor,int player)
{
   t_case v;
   if (player == 0){ // Quand on est à gauche
      if(choix==0) //voisin haut
      {
	 if(c.y==0) v.y=sizeY-1; //Sur la premiere ligne
	 else v.y=c.y-1; //N'importe où 
	 v.x=c.x;  
      }
      else if(choix==1) //voisin droite
      {
	 if(c.x==sizeX-1) v.x=0; //Sur la derniere colone
	 else v.x=c.x+1; //N'importe où 
	 v.y=c.y;  
      }
      else if(choix==2) //voisin bas
      {
	 if(c.y==sizeY-1) v.y=0; //Sur la derniere ligne
	 else v.y=c.y+1; //N'importe où 
	 v.x=c.x;  
      }
   }
   else { // Quand on est à droite
      if(choix==0) //voisin haut
      {
	 if(c.y==0) v.y=sizeY-1; //Sur la premiere ligne
	 else v.y=c.y-1; //N'importe où 
	 v.x=c.x;  
      }
      else if(choix==1) //voisin bas
      {
	 if(c.y==sizeY-1) v.y=0; //Sur la derniere ligne
	 else v.y=c.y+1; //N'importe où 
	 v.x=c.x;  
      }
      else if(choix==2) //voisin gauche
      {
	 if(c.x==0) v.x=sizeX-1; //Sur la premiere colone
	 else v.x=c.x-1; //N'importe où 
	 v.y=c.y;  
      }
   }
   v.cost=c.cost+1;
   v.heuristique=v.cost+abs(tresor.x-v.x)+abs(tresor.y-v.y);
   v.xp=c.x;
   v.yp=c.y;
   v.explor=0;
   if((int)labData_2D[v.y][v.x]==1)
      {
	 v.cost=1000;
      }
   return v;  
  	 
}

t_case recherche(t_case *Liste,int sizeX,int sizeY,t_case c)
{
   int i; 
   t_case precedent;
   for (i=0;i<sizeX*sizeY;i++)
   {
      if(Liste[i].x==c.xp&&Liste[i].y==c.yp) precedent=Liste[i];
   }
   return precedent;
}

int *A_star(t_joueur p1,t_joueur tresor,int sizeX,int sizeY,char **labData_2D,int player)
{
   t_case *Liste=(t_case *)malloc(sizeX*sizeY*sizeof(t_case)); //lise des cases
   t_case c={.x=p1.x,.y=p1.y,.cost=0,.heuristique=1000,.xp=-1,.yp=-1,.explor=0}; //premier élément (joueur)
   int i,ind=1,ind_c=0,ind_plus_petit; //ind_c est l'indice de c dans la liste
   t_case v; //voisin   
   int *tab_temp=(int *)calloc(sizeX*sizeY,sizeof(int));
   Liste[ind_c]=c;
   for(i=1;i<sizeX*sizeY;i++) Liste[i].cost = 1000;
   while(c.x!=tresor.x || c.y!=tresor.y) //tant qu'on est pas arrivé au tresor on continue
      {
	 // printf("\n%d",ind_c);
        printf("Avant les for:c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	 for(i=0;i<3;i++)
	 {
	   
	    v=voisin(c,i,sizeX,sizeY,labData_2D,tresor,player);
	    if(v.cost<100 && dedans(Liste,v,sizeX,sizeY)==0) //Si ce n'est pas un mur et pas dans la liste
	    {
	       Liste[ind]=v; //On ajoute v à la liste
	       printf("entre1\n");
	       ind+=1;
	    }       
	 }
	 printf("Avant la 2eme for:c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	 for(i=0;i<ind;i++) //On prend l'élément le plus proche du tresor inexploré
	 {
	    if(((c.cost+c.heuristique)>=(Liste[i].cost+Liste[i].heuristique) && Liste[i].explor==0) || ((Liste[i].cost+Liste[i].heuristique)> (c.cost+c.heuristique) && Liste[i].explor==0)) // On ajoute les || car les autres cases ont un cost et un heuristique plus grand que c
	    {
	       ind_plus_petit=i;
	       printf("entre2\n");
	       break;
	    }	
	 }
	 printf("ind = %d et indice_petit = %d\n", ind,ind_plus_petit);
	  Liste[ind_c].explor=1;
	  printf("Avant l'affectation: c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	  c=Liste[ind_plus_petit];
	  printf("Après l'affectation: c.x = %d, c.y = %d,c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	  //sleep(2);
	  ind_c=ind_plus_petit; 	  
      }
   printf("A la sortie du 1er while: c.x = %d, c.y = %d,c.cost=%d, c.heuristique=%d, c.xp=%d, c.yp=%d \n", c.x,c.y,c.cost,c.heuristique,c.xp,c.yp);
   printf("\n sizeX=%d et sizeY=%d",sizeX,sizeY);
   ind = 0;
   while(c.x!=p1.x || c.y!=p1.y)
   { 
      printf("\nentrée dans le 2eme while\n");
      
      if(c.x-c.xp==0 && c.y-c.yp==1)
      {
	 printf("1er if!!\n");
	 tab_temp[ind]=MOVE_DOWN;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if (c.x-c.xp==0 && c.y-c.yp==-1) 
      {
	 printf("2er if!!\n");
	 tab_temp[ind]=MOVE_UP;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if (c.x-c.xp==1 && c.y-c.yp==0) 
      {
	 printf("3er if!!\n");
	 tab_temp[ind]=MOVE_RIGHT;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if (c.x-c.xp==-1 && c.y-c.yp==0) 
      {
	 printf("4er if!!\n");
	 tab_temp[ind]=MOVE_LEFT;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      //int taper;
      //attendre:
      //if(scanf("%d",&taper)){
      //printLabyrinth();
      printf("le joueur est à:%d %d et le tresor à : %d %d\n",p1.x,p1.y,tresor.x,tresor.y);
      printf("Avant l'affectation: c.x = %d, c.y = %d, c.xp=%d, c.yp=%d \n", c.x,c.y,c.xp,c.yp);
      printf("\n sizeX=%d et sizeY=%d",sizeX,sizeY);
      //}
      //else goto attendre;
      ind+=1;
   }  
   
   int *A_star=(int *)calloc(ind,sizeof(int));
   for(i=0;i<ind;i++)
      {
	 A_star[ind-i-1]=tab_temp[i];
	 printf("\n%d|",tab_temp[i]);
      }
   
   return A_star;
}

int main()
{
   srand(time(NULL));
   char labName[50];					/* name of the labyrinth*/
   char* labData;						/* data of the labyrinth */
   char** labData_2D; //labData en 2D
   t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
   t_move move;						/* a move */
   int player;
   int sizeX,sizeY;
   t_joueur p1,p2,tresor;
   int* mouvement;
   int ind=0;

   /* connection to the server */
   connectToServer( "pc4023.polytech.upmc.fr", 1234, "Gengo");
	
	
   /* wait for a game, and retrieve informations about it */
   waitForLabyrinth( "DO_NOTHING timeout=1000", labName, &sizeX, &sizeY);
   labData = (char*) malloc( sizeX * sizeY );
   player = getLabyrinth(labData);
   printf("sizeX=%d et sizeY=%d\n",sizeX,sizeY); 
	
	
	
   printf("\n");
   //printf("L%d,R%d,U%d,D%d\n",ROTATE_LINE_LEFT,ROTATE_LINE_RIGHT,ROTATE_COLUMN_UP,ROTATE_COLUMN_DOWN);
   int mv,val; //mouvement choisi
   
   if(player==0) 
   {
      p1.x = 0;			/* On est à gauche */
      p1.y = sizeY/2;
      p1.energie = 0; 		/* On commence avec 0 energie */
      
      p2.x = sizeX-1;		/* L'adversaire commence à droite */
      p2.y = sizeY/2;
      p2.energie = 1;		/* L'adversaire commence avec 1 energie */
   }
   else
   {
      p2.x = 0;			/* L'adversaire est à gauche */
      p2.y = sizeY/2;
      p2.energie = 0; 		/* Il commence avec 0 energie */
      
      p1.x = sizeX-1;		/* On commence à droite */
      p1.y = sizeY/2;
      p1.energie = 1;		/* On commence avec 1 energie */
   }

   tresor.x=sizeX/2;
   tresor.y=sizeY/2;
   tresor.energie=0;

   p1.symbole='8';
   p2.symbole='7';
   tresor.symbole='T';

   //labData[pos_tresor]='T';
   labData_2D=init_lab(labData,p1,p2,tresor,sizeX,sizeY);
   mouvement=A_star(p1,tresor,sizeX,sizeY,labData_2D,player);
   /*for(i=0;i<100;i++)
      {	 
	 printf("\nMouvement:\n%d|",mouvement[i]);
	 }*/
   printf("\n");
     //affichage_manuel(labData,pos_J0,pos_J1,pos_tresor,sizeX,sizeY);


   do
   {
      sleep(1); // Pour ralentir l'affichage
      /* /\* display the labyrinth *\/ */
      /* printLabyrinth(); */
      /* affichage_2D(labData_2D,p1,p2,sizeX,sizeY); */
      /* if(player==0) /\* The opponent plays *\/ */
      /* { */
      /* 	 system("clear"); */
      /* 	 printLabyrinth(); */
      /* 	 // affichage_manuel(labData,pos_J0,pos_J1,pos_tresor,sizeX,sizeY); */
      /* 	 printf("\n"); */
      /* 	 affichage_2D(labData_2D,p1,p2,sizeX,sizeY); */
      /* 	 printf("sizeX=%d sizeY=%d\np1x:%d p1y:%d\np2x:%d p2y:%d\n",sizeX,sizeY,p1.x,p1.y,p2.x,p2.y); */
      /* } */
      //system("clear");
      printLabyrinth();
      affichage_2D(labData_2D,p1,p2,tresor,sizeX,sizeY);
      if (player==1) 
	  {
	 ret = getMove( &move);
	 mv = move.type;
	 val = move.value;
	 maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	 player=0;
	 
	 //playMove( &lab, move);
	  }
      else
      {	 
	 printf("Mouvement souhaité Rien=%d\nGauche=%d\nDroite=%d\nHaut=%d\nBas=%d\nChoix: ",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	 //scanf("%1d %1d",&mv,&val);
      refaire:
      	 mv = mouvement[ind++];
	 val = 0;
	 printf("mv=%d m_val=%d\n",mv,val);

	 //printf("p1.energie=%d\n",p1.energie);
 	 //if(p1.energie>=5) mv=rand()%8; //Si on a assez d'energie on change une colonne ou une ligne
 	 //else mv=rand()%4+4; //Sinon non on bouge seulement
 	 //val = 0;
 	 //printf("mv=%d m_val=%d\n",mv,val);
	 switch(mv)
	 {
	    case 0: //Bouger ligne à gauche
	       //goto refaire;
	       //val=rand()%sizeY; //Choisi une ligne aléatoire
 	       p1.energie=p1.energie-5;
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	    case 1: //Bouger ligne à droite
	       //goto refaire;
	       //val=rand()%sizeY; //Choisi une ligne aléatoire
 	       p1.energie=p1.energie-5;
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	    case 2: //Bouger colone en haut
	       //goto refaire;
	       //val=rand()%sizeX; //Choisi une colonne aléatoire
 	       p1.energie=p1.energie-5;
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	    case 3: //Bouger colone en bas
	       //goto refaire;
	       //val=rand()%sizeX; //Choisi une colonne aléatoire
 	       p1.energie=p1.energie-5;
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;

	    /* Déplacement en haut */
	    case 4:	       
	       if(p1.y==0) //Si le joueur est tout en haut et veut aller en haut
	       {
		  if((int)labData_2D[sizeY-1][p1.x]!=1 && (int)labData_2D[sizeY-1][p1.x]!=64) /* 64 en ASCII est @ */
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }		     
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout en haut et veut aller en haut	       
	       {
		  
		  if((int)labData_2D[p1.y-1][p1.x]!=1 && (int)labData_2D[p1.y-1][p1.x]!=64) 
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }	       
	       break;

	    /* Déplacement en bas */
	    case 5: 		
	       
	       if(p1.y == sizeY-1) //Si le joueur est tout en bas et veut aller en bas
	       {		  
		  if((int)labData_2D[0][p1.x]!=1 && (int)labData_2D[0][p1.x]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       } 		       
	       else //Si le joueur est autre-part que tout en bas et veut aller en bas
	       {		  
		  if((int)labData_2D[p1.y+1][p1.x]!=1 && (int)labData_2D[p1.y+1][p1.x]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }
	       break;

	    /* Déplacement à gauche */
	    case 6: 	
	       if(p1.x==0) //Si le joueur est tout à gauche et veut aller à gauche
	       {		  
		  if((int)labData_2D[p1.y][sizeX-1]!=1 && (int)labData_2D[p1.y][sizeX-1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout à gauche et veut aller à gauche
	       {		  
		  if((int)labData_2D[p1.y][p1.x-1]!=1 && (int)labData_2D[p1.y][p1.x-1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }			  
	       break;

	    /* Déplacement à droite */
	    case 7: 		
	       if(p1.x == sizeX-1) //Si le joueur est tout à droite et veut aller à droite
	       {		  
		  if((int)labData_2D[p1.y][0]!=1 && (int)labData_2D[p1.y][0]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout à droite et veut aller à droite
	       {		  
		  if((int)labData_2D[p1.y][p1.x+1]!=1 && (int)labData_2D[p1.y][p1.x+1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }	
		  else goto refaire;
	       }			  
	       break;		       
	 }
	 if(mv>3) p1.energie+=1; //Si gagne de l'énergie que si l'on a bougé
	 player = 1;
	 move.type=mv;
	 move.value=val;
	 val = 0;
	 ret = sendMove(move); //printf("pos_J0=%d\n",pos_J0);
      }
      //printLabyrinth();	

   
   }
   while((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE));
	           
   if((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE)) printf("Perdu !");
   else printf("Gagné !\n");
	
   /* we do not forget to free theallocated array */
   free(labData);

   
	
   /* end the connection, because we are polite */
   closeConnection();
	
   return EXIT_SUCCESS;
}
