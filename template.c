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
	char labName[50];					/* name of the labyrinth */
	char* labData;						/* data of the labyrinth */
	t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
	t_move move;						/* a move */
	int player;
	int sizeX,sizeY;

	/* connection to the server */
	connectToServer( "pc4023.polytech.upmc.fr", 1234, "Gengo_Lance");
	
	
	/* wait for a game, and retrieve informations about it */
	waitForLabyrinth( "DO_NOTHING timeout=10000", labName, &sizeX, &sizeY);
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
		 
	      printf("Mouvement souhaité Rien=%d\nGauche=%d\nDroite=%d\nHaut=%d\nBas=%d\nChoix: ",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	      scanf("%d",&mv);
	      move.type = mv; //DO_NOTHING;
	      move.value = 0;
	      ret = sendMove(move);
	      player=1;
	      switch(mv)
		 {
		    case 0:
		       break;
		    case 1:
		       break;
		    case 2:
		       break;
		    case 3:
		       break;
		    case 4:
		       pos_J0=pos_J0-sizeX;
		       break;
		    case 5:
		       pos_J0=pos_J0+sizeX;
		       break;
		    case 6:
		       if(pos_J0%sizeX==0) pos_J0=pos_J0+sizeX-1;
		       else pos_J0=pos_J0-1;
		       break;
		    case 7:
		       if((pos_J0+1)%sizeX==0) pos_J0=pos_J0-sizeX+1;
		       else pos_J0=pos_J0+1;
		       break;
		       
		 }
	   }
	   //printLabyrinth();	
	   
   
	}
	while((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE));
	           
	if((player ==1 && ret != MOVE_WIN) || (player==0 && ret != MOVE_LOSE)) printf("Perdu !");
	else printf("Gagné !");
	
	/* we do not forget to free the allocated array */
	free(labData);
	
	
	/* end the connection, because we are polite */
	closeConnection();
	
	return EXIT_SUCCESS;
}

