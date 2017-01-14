#include "A_star.h"

/*--------------------------------------------------------*/
/*Ensemble des fonctions qui nous permette de réaliser A* */
/*--------------------------------------------------------*/


/*Fonction qui vérifie si un élément est contenu dans une liste*/

int dedans(t_case *Liste,t_case v,int sizeX,int sizeY) 
{
   int i; //Variable pour la boucle for
   

   //On parcourt la liste et si on voit l'élément on renvoit 1
   for(i=0;i<sizeX*sizeY;i++) {if(Liste[i].x==v.x && Liste[i].y==v.y) return 1;}

   //Sinon on renvoit 0
   return 0;
}


/*Fonction qui calcule les données d'une case voisine de la case considérée*/

t_case voisin(t_case c,int choix,int sizeX,int sizeY,char **labData_2D,t_joueur tresor,int player)
{
   t_case v; //Initialisation de la nouvelle case (le voisin de la case considérée)

   /*Calcul de la case au-dessus*/
   
   if(choix==0)
   {
      //Si la case considérée est sur la premiere ligne
      if(c.y==0) v.y=sizeY-1;

      //Si elle est autre part
      else v.y=c.y-1;

      //Coordonnée pas modifiée les deux cases sont sur la même colone
      v.x=c.x;  
   }

   /*Calcul de la case de droite*/
   
   else if(choix==1)
   {
      //Si la case considérée est sur la dernière colone
      if(c.x==sizeX-1) v.x=0;

      //Si elle est autre part
      else v.x=c.x+1;

      //Coordonnée pas modifiée les deux cases sont sur la même ligne
      v.y=c.y;  
   }

   /*Calcul de la case en-dessous*/
   
   else if(choix==2)
   {
      //Si la case considérée est sur la dernière ligne
      if(c.y==sizeY-1) v.y=0;

      //Si elle est autre part
      else v.y=c.y+1;

      //Coordonnée pas modifiée les deux cases sont sur la même colone
      v.x=c.x;  
   }

   /*Calcul de la case à gauche*/
   
   else if(choix==3) 
   {
      //Si la case considérée est sur la première colone
      if(c.x==0) v.x=sizeX-1;

      //Si elle est autre part
      else v.x=c.x-1;

      //Coordonnée pas modifiée les deux cases sont sur la même ligne
      v.y=c.y;
   }

   //Le coût du voisin est celui de la case considéré plus un car on se déplace
   v.cost=c.cost+1;

   //L'heuristique est le coût plus le nombre de cases entre le voisin et le tresor
   v.heuristique=v.cost+abs(tresor.x-v.x)+abs(tresor.y-v.y);
   
   //Les coordonnées de la case précédente du voisin c'est celle de la case considérée initialement
   v.xp=c.x;
   v.yp=c.y;

   //On calcul la case voisine mais on ne va pas encore dessus donc exploration est de zero
   v.explor=0;
   
   //Si le voisin est un mur on met son coût à 1000 comme ça on ira jamais dedans
   if((int)labData_2D[v.y][v.x]==1)
   {
      v.cost=1000;
   }

   //On retourne pour finir la case voisine
   return v;  
  	 
}


/*Fonction qui sert à retrouver la case précédente d'une case considéré dans une liste, cette focntion nous permet de reconstruire le chemin final*/

t_case recherche(t_case *Liste,int sizeX,int sizeY,t_case c)
{
   //Variable pour la boucle for
   int i;

   //Initialisation de la case précédente
   t_case precedent;

   //On parcourt la liste
   for (i=0;i<sizeX*sizeY;i++)
   {
      //Si les coordonnées d'un élément de la liste sont les mêmes que les coordonnées de la case précédente de notre case considéré alors on a trouvé la case précédente
      if(Liste[i].x==c.xp && Liste[i].y==c.yp) precedent=Liste[i];
   }
   
   return precedent;
}


/*Fonction A* qui trouve le chemin le plus court pour aller jusqu'au trésor*/

int *A_star(t_joueur p1,t_joueur tresor,int sizeX,int sizeY,char **labData_2D,int player)
{
   /*Initialisation de toutes les données nécessaires*/

   /*Variable pour parcourir les listes :
     i: compter pour les boucles for
     ind: nombre de case dans la liste, donc liste[ind-1] sera le dernier élément ajouté à la liste 
     ind_c: indice de la case que l'on considère dans la liste de toute les cases calculées
     ind_plus_petit: indice de l'élément dans la liste avec le plus petit coût plus heuristique*/   
   int i,ind=1,ind_c=0,ind_plus_petit;

   //Liste de toutes les cases calculées
   t_case *Liste=(t_case *)malloc(sizeX*sizeY*sizeof(t_case));

   //Première case de la liste c'est la case ou se trouve le joueur au début
   t_case c={.x=p1.x,.y=p1.y,.cost=0,.heuristique=1000,.xp=-1,.yp=-1,.explor=0};

   //Case qui servira à calculer les voisins
   t_case v;

   //Tableau temporaire des mouvements à effectuer, temporaire car il sera à l'envers
   int *tab_temp=(int *)calloc(sizeX*sizeY,sizeof(int));

   //Tableau final avec tous les déplacements à faire, c'est le tableau tab_temp mais à l'endroit
   int *A_star;

   //On met le premier élément dans la liste
   Liste[ind_c]=c;

   //On initialise les coûts des autres cases à 1000 pour ne pas avoir de confusion quand on cherchera les coûts les plus faibles
   for(i=1;i<sizeX*sizeY;i++) Liste[i].cost = 1000;

   //Tant que la case que l'on considère n'a pas les même coordonnées que celle du trésor on ne s'arrete pas
   while(c.x!=tresor.x || c.y!=tresor.y) 
   {
      //On calcule successivement les quatres voisins de la case considéré via la fonction voisin() 
      for(i=0;i<4;i++)
      {
	 //Calcul du voisin haut,bas,droite,gauche selon la valeur de i 
	 v=voisin(c,i,sizeX,sizeY,labData_2D,tresor,player);

	 //Si le voisin que l'on vient de calculer n'est pas un mur et n'est pas dans la liste on l'ajoute à la liste
	 if(v.cost<100 && dedans(Liste,v,sizeX,sizeY)==0)
	 {
	    Liste[ind]=v;

	    //Le nombre de case dans la liste augmente alors de un
	    ind+=1;
	 }       
      }

      //On parcourt la liste jusqu'au dernier élément ajouté afin de trouver l'indice de l'élément inexploré le plus proche du tresor
      for(i=0;i<ind;i++)
      {
	 
	 // La conditon à gauche de || permet de prendre la plus petite case en coût et heuristique
	 // Celui à droite de || permet de prendre un élément de la liste quand on a déjà regardé toutes les cases de même coût et heuristique
	 if(((c.cost+c.heuristique)>=(Liste[i].cost+Liste[i].heuristique) && Liste[i].explor==0) || ((Liste[i].cost+Liste[i].heuristique)>= (c.cost+c.heuristique) && Liste[i].explor==0))  
	 {
	    //L'indice le plus petit est donc égal à i et on sort de la boucle
	    ind_plus_petit=i;	    
	    break;
	 }	
      }

      //On va se déplacer donc la case que l'on considérait est explorée et donc explor=1
      Liste[ind_c].explor=1;

      //On considère alors la case la plus proche du trésor
      c=Liste[ind_plus_petit];

      //Et donc l'indice de la case considéré devient l'indice de l'élément le plus proche du trésor
      ind_c=ind_plus_petit; 	  
   }
   /*Fin du premier while on a donc trouvé le trésor*/

   /*-------------------------------------------------------------------------------------------------------------------*/

   /*On doit maintenant faire le chemin inversant en trouvant les cases précédentes de chaque cases en partant du trésor*/
   
   //On réinitialise l'indice du nombre de case dans la liste à zero car on va faire le chemin inverse en partant du trésor comme ça ind sera exactement égal au nombre de case jusqu'au trésor
   ind=0;

   //Tant que les coordonnées de la case que l'on considère ne sont 'est pas égales à celle du joueur on ne s'arrête pas
   while(c.x!=p1.x || c.y!=p1.y)
   { 
      //Si la case précédente est en haut       
      if( (c.x-c.xp==0 && c.y-c.yp==1) || (c.x-c.xp==0 && c.y-c.yp==-sizeY+1) )
      {
	 //Lorsque l'on ira du joueur au trésor on devra descendre
	 tab_temp[ind]=MOVE_DOWN;

	 //On cherche la nouvelle case précédente
	 c=recherche(Liste,sizeX,sizeY,c);
      }

      //Si la case précédente est en bas 
      else if ( (c.x-c.xp==0 && c.y-c.yp==-1) || (c.x-c.xp==0 && c.y-c.yp==sizeY-1) )
      {
	 //Lorsque l'on ira du joueur au trésor on devra monter
	 tab_temp[ind]=MOVE_UP;

	 //On cherche la nouvelle case précédente
	 c=recherche(Liste,sizeX,sizeY,c);
      }

      //Si la case précédente est à gauche 
      else if ( (c.x-c.xp==1 && c.y-c.yp==0) || (c.x-c.xp==-sizeX+1 && c.y-c.yp==0) )
      {
	 //Lorsque l'on ira du joueur au trésor on devra aller à droite
	 tab_temp[ind]=MOVE_RIGHT;

	 //On cherche la nouvelle case précédente
	 c=recherche(Liste,sizeX,sizeY,c);
      }

      //Si la case précédente est à droite
      else if ( (c.x-c.xp==-1 && c.y-c.yp==0) || (c.x-c.xp==sizeX-1 && c.y-c.yp==0) )
      {
	 //Lorsque l'on ira du joueur au trésor on devra aller à gauche
	 tab_temp[ind]=MOVE_LEFT;

	 //On cherche la nouvelle case précédente
	 c=recherche(Liste,sizeX,sizeY,c);
      }

      //On augmente l'indice de un pour connaitre le nombre de case dans notre liste temporaire
      ind+=1;
   }  

   //On initialise A* de la longueur du nombre de case pour aller jusqu'au trésor soit ind
   A_star=(int *)calloc(ind,sizeof(int));

   //On parcourt A* pour lui affecter les valeurs de tab_temp dans le bon ordre
   for(i=0;i<ind;i++)
   {
      A_star[ind-i-1]=tab_temp[i];
      
   }

   //On retourne A* contenant tous les déplacements à faire pour accéder au trésor le plus rapidement possible
   return A_star;
}
