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
   int x; //Coordonné x
   int y; //Coordonnée y
   int energie;
} t_joueur;
extern int debug;	/* hack to enable debug messages */


char** init_lab(char *labData,t_joueur p1,t_joueur p2,int sizeX, int sizeY)
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
	    if((i==p1.y && j==p1.x)) tabData[i][j]='>';
	    else if((i==p2.y && j==p2.x)) tabData[i][j]='@';
	    else if((i==sizeY/2 && j==sizeX/2)) tabData[i][j]='T';
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

void affichage_2D(char** labData_2D,t_joueur p1,t_joueur p2,int sizeX,int sizeY)
{
   int i,j;   
   for(i=0;i<sizeY;i++)
   {
      for(j=0;j<sizeX;j++)
      {
	 if((i==p1.y && j==p1.x) || (i==p2.y && j==p2.x) || (i==sizeY/2 && j==sizeX/2)) 
	 {
	    putchar(labData_2D[i][j]);
	    printf("|");
	 }
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
   char** labData_2D; //labData en 2D
   t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
   t_move move;						/* a move */
   int player;
   int sizeX,sizeY;
   t_joueur p1,p2;

   /* connection to the server */
   connectToServer( "pc4001.polytech.upmc.fr", 1234, "Gengo_Lance");
	
	
   /* wait for a game, and retrieve informations about it */
   waitForLabyrinth( "DO_NOTHING timeout=6000", labName, &sizeX, &sizeY);
   labData = (char*) malloc( sizeX * sizeY );
   player = getLabyrinth(labData);
   printf("sizeX=%d et sizeY=%d\n",sizeX,sizeY); 
	
	
	
   printf("\n");
   //printf("L%d,R%d,U%d,D%d\n",ROTATE_LINE_LEFT,ROTATE_LINE_RIGHT,ROTATE_COLUMN_UP,ROTATE_COLUMN_DOWN);
   int mv; //mouvement choisi
   
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

   //labData[pos_tresor]='T';
   labData_2D=init_lab(labData,p1,p2,sizeX,sizeY);
   printf("\n");
     //affichage_manuel(labData,pos_J0,pos_J1,pos_tresor,sizeX,sizeY);


   do
   {
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
      system("clear");
      printLabyrinth();
      affichage_2D(labData_2D,p1,p2,sizeX,sizeY);
      if (player==1) 
	  {
	 ret = getMove( &move);
	 player=0;
	 
	 //playMove( &lab, move);
	  }
      else
      {	 
	 player=1;
	 printf("Mouvement souhaité Rien=%d\nGauche=%d\nDroite=%d\nHaut=%d\nBas=%d\nChoix: ",DO_NOTHING,MOVE_LEFT,MOVE_RIGHT,MOVE_UP,MOVE_DOWN);
	 //scanf("%d",&mv);
      refaire:
	 mv=rand()%8;
	 printf("mv=%d\n",mv);
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

	    /* Déplacement en haut */
	    case 4:	       
	       if(p1.y==0) //Si le joueur est tout en haut et veut aller en haut
	       {
		  if((int)labData_2D[sizeY-1][p1.x]!=1 && (int)labData_2D[sizeY-1][p1.x]!=64) 
		  {
		     labData_2D[p1.y][p1.x]=0;
		     p1.y=sizeY-1;
		     labData_2D[p1.y][p1.x]='>';
		  }		     
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout en haut et veut aller en haut	       
	       {
		  
		  if((int)labData_2D[p1.y-1][p1.x]!=1 && (int)labData_2D[p1.y-1][p1.x]!=64) 
		  {
		     labData_2D[p1.y][p1.x]=0;
		     p1.y=p1.y-1;
		     labData_2D[p1.y][p1.x]='>';
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
		     labData_2D[p1.y][p1.x]=0;
		     p1.y=0;
		     labData_2D[p1.y][p1.x]='>';
		  }	
		  else goto refaire;
	       } 		       
	       else //Si le joueur est autre-part que tout en bas et veut aller en bas
	       {		  
		  if((int)labData_2D[p1.y+1][p1.x]!=1 && (int)labData_2D[p1.y+1][p1.x]!=64)
		  {
		     labData_2D[p1.y][p1.x]=0;
		     p1.y=p1.y+1;
		     labData_2D[p1.y][p1.x]='>';
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
		     labData_2D[p1.y][p1.x]=0;
		     p1.x=sizeX-1;
		     labData_2D[p1.y][p1.x]='>';
		  }	
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout à gauche et veut aller à gauche
	       {		  
		  if((int)labData_2D[p1.y][p1.x-1]!=1 && (int)labData_2D[p1.y][p1.x-1]!=64)
		  {
		     labData_2D[p1.y][p1.x]=0;
		     p1.x=p1.x-1;
		     labData_2D[p1.y][p1.x]='>';
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
		     labData_2D[p1.y][p1.x]=0;
		     p1.x=0;
		     labData_2D[p1.y][p1.x]='>';
		  }	
		  else goto refaire;
	       }
	       else //Si le joueur est autre-part que tout à droite et veut aller à droite
	       {		  
		  if((int)labData_2D[p1.y][p1.x+1]!=1 && (int)labData_2D[p1.y][p1.x+1]!=64)
		  {
		     labData_2D[p1.y][p1.x]=0;
		     p1.x=p1.x+1;
		     labData_2D[p1.y][p1.x]='>';
		  }	
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
