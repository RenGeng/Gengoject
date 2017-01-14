// Fichier source pour la création, l'affichage et le mise à jour du tableau 2D

#include "labData_2D.h"

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
