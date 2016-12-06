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

extern int debug;	/* hack to enable debug messages */

void affichage_manuel(char *labData,int pos_J0,int pos_J1,int pos_tresor,int sizeX,int sizeY)
{
   int i;
   labData[pos_J0]='>';
   labData[pos_J1]='@';
   for(i=0;i<(sizeX*sizeY);i++) 
   {
      if(i%sizeX==0) printf("\n");
      if (i==pos_J0 || i==pos_J1 || i==pos_tresor) 
      {
	 putchar(labData[i]);
	 printf("|");
      }
      else printf("%d|",labData[i]);
   }
   labData[pos_J0]=0;
   labData[pos_J1]=0;      
   printf("\n");

}
int main()
{
   srand(time(NULL));
   char labName[50];					/* name of the labyrinth */
   char* labData;						/* data of the labyrinth */
   t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
   t_move move;						/* a move */
   int player;
   int sizeX,sizeY;

   /* connection to the server */
   connectToServer( "pc4023.polytech.upmc.fr", 1234, "Gengo_Lance");
	
	
   /* wait for a game, and retrieve informations about it */
   waitForLabyrinth( "PLAY_RANDOM timeout=1000", labName, &sizeX, &sizeY);
   labData = (char*) malloc( sizeX * sizeY );
   player = getLabyrinth(labData);
   printf("sizeX=%d et sizeY=%d\n",sizeX,sizeY); 
	
	
	
   printf("\n");
   //printf("%d,G%d,D%d,H%d,B%d\n",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
   int mv; //mouvement choisi
   int pos_J0,pos_J1,pos_tresor; //position des joueurs en 1D
   pos_tresor=sizeX*(sizeY/2)+sizeX/2;
   if(player==0) 
   {
      pos_J0=sizeX*(sizeY/2); 
      pos_J1=sizeX+pos_J0-1;
   }
   else
   {
      pos_J1=sizeX*(sizeY/2); 
      pos_J0=sizeX+pos_J1-1;
   }

   labData[pos_tresor]='T';
   //affichage_manuel(labData,pos_J0,pos_J1,pos_tresor,sizeX,sizeY);
   //printLabyrinth();
	
   do
   {
      if(player==0)
      {
	 system("clear");
	 printLabyrinth();
	 affichage_manuel(labData,pos_J0,pos_J1,pos_tresor,sizeX,sizeY);
      }
      /* display the labyrinth */	      
	
      if (player==1)	/* The opponent plays */
      {
	 ret = getMove( &move);
	 player=0;
	 //playMove( &lab, move);
      }
      else
      {	      
	 //printf("Mouvement souhaité Rien=%d\nGauche=%d\nDroite=%d\nHaut=%d\nBas=%d\nChoix: ",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	 //scanf("%d",&mv);
      refaire:
	 mv=rand()%8;
	 //printf("mv=%d\n",mv);
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
	       }
	       break;
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
	 ret = sendMove(move);
	 //printf("pos_J0=%d\n",pos_J0);
      }
      //printLabyrinth();	
	   
   
   }
   while((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE));
	           
   if((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE)) printf("Perdu !");
   else printf("Gagné !\n");
	
   /* we do not forget to free the allocated array */
   free(labData);
	
	
   /* end the connection, because we are polite */
   closeConnection();
	
   return EXIT_SUCCESS;
}

