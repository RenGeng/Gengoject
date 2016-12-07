//
// TEMPLATE 
//
//
// Permet de jouer un seul tour (en ne faisant rien s'il commence ou en 
// réceptionnant le coup de l'adversaire s'il ne commence pas) 
// et termine le jeu.
// Ce programme vous sert de base pour construire votre propre programme



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "labyrinthAPI.h"
#include <unistd.h>
#include <string.h>

typedef struct{
   int x;
   int y;
   int energie;
} joueur;

extern int debug;	/* hack to enable debug messages */

void affichage_manuel(char **tabData,joueur p1,joueur p2,int sizeX,int sizeY)
{
   /* int i; */
   /* labData[pos_J0]='>'; */
   /* labData[pos_J1]='@'; */
   /* for(i=0;i<(sizeX*sizeY);i++)  */
   /* { */
   /*    if(i%sizeX==0) printf("\n"); */
   /*    if (i==pos_J0 || i==pos_J1 || i==pos_tresor)  */
   /*    { */
   /* 	 putchar(labData[i]); */
   /* 	 printf("|"); */
   /*    } */
   /*    else printf("%d|",labData[i]); */
   /* } */
   /* labData[pos_J0]=0; */
   /* labData[pos_J1]=0;       */
   /* printf("\n"); */
   
   /* Problème d'affichage mais le tableau est bien initialisé */
   tabData[p1.x][p1.y] = '>';
   tabData[p2.x][p2.y] = '@';
   int i,j;
   for(i=0;i<sizeY;i++)
   {
      for(j=0;j<sizeX;j++) 
	 {
	    if (j%sizeX==0) printf("\n");
	    if ((i==p1.y && j==p1.x) || (i==p2.y && j==p2.x)) putchar(tabData[i][j]);
	    else printf("%d|", tabData[i][j]);
	 }
   }
   printf("\n");
   tabData[p1.x][p1.y] = 0;
   tabData[p2.x][p2.y] = 0;
}

char** init_tab(char *labData,int sizeX, int sizeY)
{
   /*initialisation du tableau 2D */
   char** tabData;
   int i,j;
   tabData = (char**) malloc(sizeY*sizeof(char*));
   for (i=0;i<sizeY;i++) tabData[i] = (char*) malloc(sizeX*sizeof(char));
   for (i=0;i<sizeY;i++)
   {
      for (j=0;j<sizeX;j++) tabData[i][j] = labData[i*sizeX+j];
   }
   return tabData;

}

char *maj_lab(char *labData,t_move move,int sizeX,int sizeY)
{
   int val=move.value,pos,i;
   char temp;
   int mv=move.type;
   switch(mv)
   {
      case 0: //Bouger une ligne vers la gauche
	 pos=sizeX*(val-1);
	 temp=labData[pos];
	 for(i=pos;i<pos+sizeX-1;i++) labData[i]=labData[i+1];
	 labData[i]=temp;
	 break;

      case 1: //Bouger une ligne vers la droite
	 pos=sizeX*val-1;
	 temp=labData[pos];
	 for(i=pos;i>pos-sizeX+1;i--) labData[i]=labData[i-1];
	 labData[i]=temp;
	 break;

      case 2: //Bouger une colonne vers le haut
	 pos=val-1;
	 temp=labData[pos];
	 for(i=pos;i<pos-sizeX+sizeX*sizeY;i=i+sizeX) labData[i]=labData[i+sizeX];
	 labData[i]=temp;
	 break;
      case 3: //Bouger une colonne vers le bas
	 pos=sizeX*sizeY-sizeX+val-1;
	 temp=labData[pos];
	 for(i=pos;i>val-1;i=i-sizeX) labData[i]=labData[i-sizeX];
	 labData[i]=temp;
	 break;
   }
   return labData;
   
}
int main()
{
   srand(time(NULL));
   char labName[50];					/* name of the labyrinth */
   char* labData;						/* data of the labyrinth */
   char** tabData;						/* Déclaration du tableau 2D */
   t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
   t_move move;						/* a move */
   int player;
   int sizeX,sizeY;
   joueur p1, p2; 		/* Initialisation de structure des 2 joueurs */

   /* connection to the server */
   connectToServer( "pc4023.polytech.upmc.fr", 1234, "Gengo_Lance");
	
	
   /* wait for a game, and retrieve informations about it */
   waitForLabyrinth( "DO_NOTHING timeout=6000", labName, &sizeX, &sizeY);
   labData = (char*) malloc( sizeX * sizeY );
   player = getLabyrinth(labData);
   tabData = init_tab(labData,sizeX,sizeY);

   printf("sizeX=%d et sizeY=%d\n",sizeX,sizeY);
   printf("\n");
   //printf("L%d,R%d,U%d,D%d\n",ROTATE_LINE_LEFT,ROTATE_LINE_RIGHT,ROTATE_COLUMN_UP,ROTATE_COLUMN_DOWN);
   int mv; //mouvement choisi
   int pos_J0,pos_J1,pos_tresor; //position des joueurs en 1D
   pos_tresor=sizeX*(sizeY/2)+sizeX/2;
   if(player==0)  		/* Si l'adversaire commence */
   {
      p2.x = 0;			/* L'adversaire est à gauche */
      p2.y = sizeY/2;
      p2.energie = 0; 		/* Il commence avec 0 energie */
      
      p1.x = sizeX-1;		/* On commence à droite */
      p1.y = sizeY/2;
      p1.energie = 1;		/* On commence avec 1 energie */
   }
   else
   {
      p1.x = 0;			/* On est à gauche */
      p1.y = sizeY/2;
      p1.energie = 0; 		/* On commence avec 0 energie */
      
      p2.x = sizeX-1;		/* L'adversaire commence à droite */
      p2.y = sizeY/2;
      p2.energie = 1;		/* L'adversaire commence avec 1 energie */
   }

   //labData[pos_tresor]='T';
   // affichage_manuel(tabData,p1,p2,sizeX,sizeY);
   //printLabyrinth();
	
   do
   {
      affichage_manuel(tabData,p1,p2,sizeX,sizeY);
      if(player==0)
      {
	 system("clear");
	 printLabyrinth();
      }
      /* display the labyrinth */	      
	
      if (player==1)	/* The opponent plays */
      {
	 ret = getMove( &move);
	 if(move.type<=3) labData=maj_lab(labData,move,sizeX,sizeY);
	 player=0;
	 
	 //playMove( &lab, move);
      }
      else
      {	      
	 printf("Mouvement souhaité Rien=%d\nGauche=%d\nDroite=%d\nHaut=%d\nBas=%d\nChoix: ",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	 //scanf("%d",&mv);
      refaire:
	 mv=rand()%8;
	 printf("mv=%d\n",mv);
	 player=1;
	 switch(mv)
	 {
	    case 0:
	       goto refaire;
	       break;
	    case 1:
	       goto refaire;
	       break;
	    case 2:
	       goto refaire;
	       break;
	    case 3:
	       goto refaire;
	       break;
	    case 4:
	       if(pos_J0<sizeX)
	       {
		  if((int)labData[sizeX*sizeY-sizeX+pos_J0]!=1 && sizeX*sizeY-sizeX+pos_J0!=pos_J1) pos_J0=sizeX*sizeY-sizeX+pos_J0;
		  else goto refaire;
	       }
	       else
	       {
		  if((int)labData[pos_J0-sizeX]!=1 && pos_J0-sizeX!=pos_J1) pos_J0=pos_J0-sizeX;
		  else goto refaire;
	       }	       break;
	    case 5: 
	       if(pos_J0>=(sizeX*sizeY-sizeX)) 
	       {
		  if((int)labData[pos_J0%sizeX]!=1 && pos_J0%sizeX!=pos_J1) pos_J0=pos_J0%sizeX;
		  else goto refaire;
	       } 		       
	       else 
	       {
		  if((int)labData[pos_J0+sizeX]!=1 && pos_J0+sizeX!=pos_J1) pos_J0=pos_J0+sizeX;
		  else goto refaire;
	       }
	       break;
	    case 6:
	       if(pos_J0%sizeX==0)
	       {
		  if((int)labData[pos_J0+sizeX-1]!=1 && pos_J0+sizeX-1!=pos_J1) pos_J0=pos_J0+sizeX-1;
		  else goto refaire;
	       }
	       else 
	       {
		  if((int)labData[pos_J0-1]!=1 && pos_J0-1!=pos_J1) pos_J0=pos_J0-1;
		  else goto refaire;
	       }			  
	       break;
	    case 7:
	       if((pos_J0+1)%sizeX==0) 
	       {
		  if((int)labData[pos_J0-sizeX+1]!=1 && pos_J0-sizeX+1!=pos_J1) pos_J0=pos_J0-sizeX+1;
		  else goto refaire;
	       }
	       else
	       {
		  if((int)labData[pos_J0+1]!=1 && pos_J0+1!=pos_J1) pos_J0=pos_J0+1;
		  else goto refaire;
	       }			  
	       break;		       
	 }
	 move.type=mv;
	 move.value=0;
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
	
