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


extern int debug;	/* hack to enable debug messages */


int main()
{
	char labName[50];					/* name of the labyrinth */
	char* labData;						/* data of the labyrinth */
	t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
	t_move move;						/* a move */
	int player;
	int sizeX,sizeY;
	int i;

	/* connection to the server */
	connectToServer( "pc4023.polytech.upmc.fr", 1234, "Gengo_Lance");
	
	
	/* wait for a game, and retrieve informations about it */
	waitForLabyrinth( "DO_NOTHING timeout=10", labName, &sizeX, &sizeY);
	labData = (char*) malloc( sizeX * sizeY );
	player = getLabyrinth(labData);
	printf("sizeX=%d et sizeY=%d\n",sizeX,sizeY); 
	
	
	for(i=0;i<(sizeX*sizeY);i++) 
	   {
	      if(i%sizeX==0 && i!=0) printf("\n");
	      printf("%d|",labData[i]);	      	      
	   }
	printf("\n");
	printf("%d,G%d,D%d,H%d,B%d\n",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	int mv; //mouvement choisi
        printLabyrinth();
	do
	{
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
	   }
	   printLabyrinth();	
	      
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

