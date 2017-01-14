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
  
   int mv,val; //mouvement choisi
   
   if(player==0) 
   {
      p1.x = 0;			/* On est à gauche */
      p1.y = sizeY/2;
      p1.energie = 0; 		/* On commence avec 0 energie */
      
      p2.x = sizeX-1;		/* L'adversaire commence à droite */
      p2.y = sizeY/2;
      p2.energie = 2;		/* L'adversaire commence avec 1 energie */
   }
   else
   {
      p2.x = 0;			/* L'adversaire est à gauche */
      p2.y = sizeY/2;
      p2.energie = 0; 		/* Il commence avec 0 energie */
      
      p1.x = sizeX-1;		/* On commence à droite */
      p1.y = sizeY/2;
      p1.energie = 2;		/* On commence avec 1 energie */
   }

   //Initalisation des positions et de l'énergie du trésor
   tresor.x=sizeX/2;
   tresor.y=sizeY/2;
   tresor.energie=0;

   //Initialisation des symboles pour afficher les joueurs et trésor dans notre tableau avec des 0 et 1 pour débuguer
   p1.symbole='8';
   p2.symbole='7';
   tresor.symbole='T';

   //Initialisation du tableau en 2D via le tableau en 1D renvoyé par getlabyrinth()
   labData_2D=init_lab(labData,p1,p2,tresor,sizeX,sizeY);

   //Récupération des mouvements pour faire le chemin le plus court jusqu'au trésor
   mouvement=A_star(p1,tresor,sizeX,sizeY,labData_2D,player);  
  
   do
   {      
      printLabyrinth(); //affichage du labyrinthe

      //Récupération des mouvements de l'adversaire et mise à jour du labyrinthe en fonction de ces derniers
      if (player==1) 
      {
	 ret = getMove( &move);
	 mv = move.type;
	 val = move.value;
	 maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	 player=0; 
      }

      //C'est à nous de jouer
      else
      {	 	  
      refaire:

	 //On parcourt la liste mouvement des mouvements à faire pour aller au trésor le plus vite possible
      	 mv = mouvement[ind++];
	 //On ne bouge pas de ligne ou colonne
	 val = 0;
	 
	 switch(mv)
	 {
	    /*Bouger ligne à gauche*/
	    
	    case 0: 

	       //On perd 5 d'énergie
 	       p1.energie=p1.energie-5;
	       //On met à jour le labyrinthe
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	       

	    /*Bouger ligne à droite */
	       
	    case 1:

	       //On perd 5 d'énergie
 	       p1.energie=p1.energie-5;
	       //On met à jour le labyrinthe
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	       

	    /*Bouger colone en haut*/
	       
	    case 2: 

	       //On perd 5 d'énergie
 	       p1.energie=p1.energie-5;
	       //On met à jour le labyrinthe
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	       
	       
	    /*Bouger colone en bas*/
	       
	    case 3:
	       
	       //On perd 5 d'énergie
	       p1.energie=p1.energie-5;
	       //On met à jour le labyrinthe
	       maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
	       break;
	       

	    /*Déplacement en haut*/
	       
	    case 4:
	       
	       if(p1.y==0) //Si le joueur est tout en haut et veut aller en haut
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[sizeY-1][p1.x]!=1 && (int)labData_2D[sizeY-1][p1.x]!=64) /* 64 en ASCII est @ */
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       else //Si le joueur est autre-part que tout en haut et veut aller en haut	       
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y-1][p1.x]!=1 && (int)labData_2D[p1.y-1][p1.x]!=64) 
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       break;
	       

	    /*Déplacement en bas*/
	       
	    case 5: 		
	       
	       if(p1.y == sizeY-1) //Si le joueur est tout en bas et veut aller en bas
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[0][p1.x]!=1 && (int)labData_2D[0][p1.x]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       else //Si le joueur est autre-part que tout en bas et veut aller en bas
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y+1][p1.x]!=1 && (int)labData_2D[p1.y+1][p1.x]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       break;

	       
	    /*Déplacement à gauche*/
	       
	    case 6:
	       
	       if(p1.x==0) //Si le joueur est tout à gauche et veut aller à gauche
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y][sizeX-1]!=1 && (int)labData_2D[p1.y][sizeX-1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       else //Si le joueur est autre-part que tout à gauche et veut aller à gauche
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y][p1.x-1]!=1 && (int)labData_2D[p1.y][p1.x-1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)

		  else goto refaire;
	       }			  
	       break;
	       

	    /*Déplacement à droite*/
	       
	    case 7:
	       
	       if(p1.x == sizeX-1) //Si le joueur est tout à droite et veut aller à droite
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y][0]!=1 && (int)labData_2D[p1.y][0]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       else //Si le joueur est autre-part que tout à droite et veut aller à droite
	       {
		  //Si la case où on veut aller n'est pas un mur ou l'autre joueur
		  if((int)labData_2D[p1.y][p1.x+1]!=1 && (int)labData_2D[p1.y][p1.x+1]!=64)
		  {
		     maj_lab(labData_2D,mv,val,&p1,&p2,&tresor,sizeX,sizeY,player);
		  }

		  //Sinon on recommence (c'est pour les mouvements aléatoire que l'on vérifie cela)
		  else goto refaire;
	       }
	       
	       break;		       
	 }

	 //Si on a bougé et non pas modifier une ligne ou colonne alors on gagne un d'énergie
	 if(mv>3) p1.energie+=1;

	 //C'est à l'autre de jouer
	 player = 1;

	 //On envoie nos mouvements
	 move.type=mv;
	 move.value=val;
	 val = 0;
	 ret = sendMove(move); 
      }
      
   
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
