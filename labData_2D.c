#include "labData_2D.h"

/*-----------------------------------------------------------------------------*/
/* Fichier source pour la création, l'affichage et le mise à jour du tableau 2D*/
/*-----------------------------------------------------------------------------*/

/*Fonction qui va initialiser notre tableau en 2D à partir du tableau en 1D renvoyé par getlabyrinth()*/
char** init_lab(char *labData,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX, int sizeY)
{
   //Variables utiles pour les boucles for
   int i,j;

   //Initialisation du tableau en 2D
   char** tabData; 
   tabData = (char**) malloc(sizeY*sizeof(char*));
   for (i=0;i<sizeY;i++) tabData[i] = (char*) malloc(sizeX*sizeof(char));
	
   //On parcourt notre tableau en 2D
   for (i=0;i<sizeY;i++)
   {
      for (j=0;j<sizeX;j++) 
      {
	 //Si nous arrivons sur une coordonné d'un joueur ou du tresor, on insert le caractère correspondant à sa position
	 if((i==p1.y && j==p1.x) || (i==p2.y && j==p2.x) || (i==sizeY/2 && j==sizeX/2))
	 {
	    if((i==p1.y && j==p1.x)) tabData[i][j]=JOUEUR;
	    else if((i==p2.y && j==p2.x)) tabData[i][j]=ADVER;
	    else if((i==tresor.y && j==tresor.x)) tabData[i][j]=TRESOR;
	 }
	 
	 //Sinon on ajoute la case correspondante au tableau en 1D
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


/*Fonction qui va nous permettre d'afficher le labyrinth avec des 0 et des 1 pour que l'on puisse comprendre nos différents bug*/

void affichage_2D(char** labData_2D,t_joueur p1,t_joueur p2,t_joueur tresor,int sizeX,int sizeY)
{
   //Variables utiles pour les boucles for
   int i,j;   
	
   //On parcourt le tableau en 2D
   for(i=0;i<sizeY;i++)
   {
      for(j=0;j<sizeX;j++)
      {
	 //Si nous sommes sur une position d'un joueur ou du tresor on affiche le caractère correspondant
	 if(i==p1.y && j==p1.x) printf(">|");
	 else if(i==p2.y && j==p2.x) printf("@|");
	 else if(i==tresor.y && j==tresor.x) printf("T|");
	      
	 //Sinon on affiche juste la case (0 ou 1)
	 else printf("%d|",labData_2D[i][j]);   
	
      }
      printf("\n");
   }
}


/*Fonction qui va mettre le tableau en 2D à jour après des déplacements ou rotations*/

void maj_lab(char **labData_2D,int mv,int val,t_joueur* p1,t_joueur* p2,t_joueur* tresor,int sizeX,int sizeY, int player)
{
   //Variable utile pour les boucles for
   int i;
	
   //Variable de sauvegarde du premier caractère lorsque l'on bouge une ligne ou une colone
   char temp;
   
   /*Selon le mouvement nous effectuons la mise à jour*/
   switch(mv)
   {

      /*Bouger une ligne vers la gauche*/
      
      case 0: 
		  
	 if (p2->y == val) //Si l'adversaire est sur la ligne
	 { 
	    labData_2D[p2->y][p2->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur
	    if (p2 -> x == 0) p2->x = sizeX-1;
	    else p2->x--;
	 }
		   
	 if (tresor->y == val) //Si le trésor est sur la ligne
	 { 
	    labData_2D[tresor->y][tresor->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du tresor
	    if (tresor->x == 0) tresor->x = sizeX-1;
	    else tresor->x--;
	 }
		   
	 if (p1->y == val) //Si on est sur la ligne
	 { 
	    labData_2D[p1->y][p1->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur
	    if (p1->x == 0) p1->x = sizeX-1;
	    else p1->x--;
	 }
		   
	 temp = labData_2D[val][0]; //On sauvegarde le premier caractère de la ligne
		   
	 //On parcourt la ligne pour remettre les caractères correspondant au bonne position dans notre tableau
	 for(i=0;i<sizeX-1;i++) labData_2D[val][i]=labData_2D[val][i+1];
	 labData_2D[val][sizeX-1]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;   
	 
      /*Bouger une ligne vers la droite*/
	 
      case 1: 
		   
	 if (p2->y == val) //Si l'adversaire est sur la ligne
	 { 
	    labData_2D[p2->y][p2->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur	 
	    if (p2->x == sizeX-1) p2->x = 0;
	    else p2->x++;
	 }
		   
	 if (tresor->y == val) //Si le tresor est sur la ligne
	 { 
	    labData_2D[tresor->y][tresor->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du tresor
	    if (tresor->x == sizeX-1) tresor->x = 0;
	    else tresor->x++;
	 }
		   
	 if (p1->y == val) // Si on est sur la ligne 
	 { 
   	    labData_2D[p1->y][p1->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur	 
   	    if (p1->x == sizeX-1) p1->x = 0;
   	    else p1->x++;
	 }
		   
	 //On sauvegarde le premier caractère de la ligne
	 temp = labData_2D[val][sizeX-1];
		   
         //On parcourt la ligne pour remettre les caractères correspondant au bonne position dans notre tableau
	 for(i=sizeX-1;i>0;i--) labData_2D[val][i]=labData_2D[val][i-1];
	 labData_2D[val][0]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x] = TRESOR;
	 break;
		   
      /*Bouger une colonne vers le haut*/
	 
      case 2:  
		   
   	 if (p2->x == val) // Si l'adversaire est sur la colone
	 { 
	    labData_2D[p2->y][p2->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur	 
	    if (p2 -> y == 0) p2->y = sizeY-1;
	    else p2->y--;
	 }
		   
	 if (tresor->x == val) // Si le trésor est sur la colone
	 { 
	    labData_2D[tresor->y][tresor->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du tresor	 
	    if (tresor->y == 0) tresor->y = sizeY-1;
	    else tresor->y--;
	 }
		   
	 if (p1->x == val) // Si on est sur la colone
	 { 
   	    labData_2D[p1->y][p1->x] = 0; //On remplace le caractère par 0
		 
	    //On modifie la coordonnée du joueur	 
   	    if (p1->y == 0) p1->y = sizeY-1;
   	    else p1->y--;
	 }
		   
         //On sauvegarde le premier caractère de la colone
	 temp = labData_2D[0][val];
		   
	 //On parcourt la ligne pour remettre les caractères correspondant au bonne position dans notre tableau	   
	 for(i=0;i<sizeY-1;i++) labData_2D[i][val]=labData_2D[i+1][val];
	 labData_2D[sizeY-1][val]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;
	 
      /*Bouger une colonne vers le bas*/
	 
      case 3:   
		   
	 if (p2->x == val) // Si l'adversaire est sur la colone
	 { 
	    labData_2D[p2->y][p2->x] = 0; //On remplace le caractère par 0
		
	    //On modifie la coordonnée du joueur
	    if (p2 -> y == sizeY-1) p2->y = 0;
	    else p2->y++;
	 }
		   
	 if (tresor->x == val) // Si le trésor est sur la colone
	 {
	    labData_2D[tresor->y][tresor->x] = 0; //On remplace le caractère par 0
		
	    //On modifie la coordonnée du tresor	 
	    if (tresor->y == sizeY-1) tresor->y = 0;
	    else tresor->y++;
	 }
		   
	 if (p1->x == val) // Si on est sur la colone
	 {
   	    labData_2D[p1->y][p1->x] = 0; //On remplace le caractère par 0
		
	    //On modifie la coordonnée du joueur	 
   	    if (p1->y == sizeY-1) p1->y = 0;
   	    else p1->y++;
	 }
		
	 //On sauvegarde le premier caractère de la colone
	 temp = labData_2D[sizeY-1][val];
		   
	 //On parcourt la ligne pour remettre les caractères correspondant au bonne position dans notre tableau		   
	 for(i=sizeY-1;i>0;i--) labData_2D[i][val]=labData_2D[i-1][val];
	 labData_2D[0][val]=temp;
	 labData_2D[p1->y][p1->x]=JOUEUR;
	 labData_2D[p2->y][p2->x]=ADVER;
	 labData_2D[tresor->y][tresor->x]=TRESOR;
	 break;

      /*Déplacement vers le haut*/
      case 4: 
		   
   	 if (player == 1) //Si c'est l'adversaire qui joue
   	 {
   	    labData_2D[p2->y][p2->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p2->y == 0) p2->y = sizeY-1; //Si le joueur est sur un bord
   	    else p2->y --;
   	    labData_2D[p2->y][p2->x] = ADVER; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 else  //Sinon c'est nous qui jouons
   	 {
   	    labData_2D[p1->y][p1->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p1->y == 0) p1->y = sizeY-1; //Si le joueur est sur un bord
   	    else p1->y --;
   	    labData_2D[p1->y][p1->x] = JOUEUR; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 break;
		
      /*Déplacement vers la bas*/
	 
      case 5: 
		   
   	 if (player == 1) //Si c'est l'adversaire qui joue
   	 {
   	    labData_2D[p2->y][p2->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p2->y == sizeY-1) p2->y = 0; //Si le joueur est sur un bord
   	    else p2->y ++;
   	    labData_2D[p2->y][p2->x] = ADVER; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 else  //Sinon c'est nous qui jouons
   	 {
   	    labData_2D[p1->y][p1->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p1->y == sizeY-1) p1->y = 0; //Si le joueur est sur un bord
   	    else p1->y ++;
   	    labData_2D[p1->y][p1->x] = JOUEUR; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 break;
	
      /*Déplacement vers la gauche*/
	 
      case 6: 
		   
   	 if (player == 1) //Si c'est l'adversaire qui joue
   	 {
   	    labData_2D[p2->y][p2->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p2->x == 0) p2->x = sizeX-1; //Si le joueur est sur un bord
   	    else p2->x --;
   	    labData_2D[p2->y][p2->x] = ADVER; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 else  //Sinon c'est nous qui jouons
   	 {
   	    labData_2D[p1->y][p1->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p1->x == 0) p1->x = sizeX-1; //Si le joueur est sur un bord
   	    else p1->x --;
   	    labData_2D[p1->y][p1->x] = JOUEUR; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 break;
		   
      /*Déplacement vers la droite*/
	 
      case 7: 
		   
   	 if (player == 1) //Si c'est l'adversaire qui joue
   	 {
   	    labData_2D[p2->y][p2->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p2->x == sizeX-1) p2->x = 0; //Si le joueur est sur un bord
   	    else p2->x ++;
   	    labData_2D[p2->y][p2->x] = ADVER; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 else  //Sinon c'est nous qui jouons
   	 {
   	    labData_2D[p1->y][p1->x] = 0; //On remplace la position par un 0 car si un joueur y était c'est que ce n'était pas un mur (1)
   	    if (p1->x == sizeX-1) p1->x = 0; //Si le joueur est sur un bord
   	    else p1->x ++;
   	    labData_2D[p1->y][p1->x] = JOUEUR; //On met le caractère du joueur à la nouvelle position dans le tableau
   	 }
		   
   	 break;
   } 

