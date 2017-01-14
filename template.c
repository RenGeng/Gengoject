//
// TEMPLATE 
//
//
// Permet de jouer un seul tour (en ne faisant rien s'il commence ou en 
// réceptionnant le coup de l'adversaire s'il ne commence pas) 
// et termine le jeu.
// Ce programme vous sert de base pour construire votre propre programme
//< pos[p1.y][p1.x]

#include "structure.h"
#include "A_star.h"
#include "labData_2D.h"

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
   waitForLabyrinth( "ASTAR timeout=1000", labName, &sizeX, &sizeY);
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
      sleep(0.5); // Pour ralentir l'affichage
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
