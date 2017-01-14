#include "A_star.h"

int dedans(t_case *Liste,t_case v,int sizeX,int sizeY) //vérifie que v est dans la liste ou pas
{
   int i;
   for(i=0;i<sizeX*sizeY;i++) {if(Liste[i].x==v.x && Liste[i].y==v.y) return 1;} //dedans
   return 0; //pas dedans
}

t_case voisin(t_case c,int choix,int sizeX,int sizeY,char **labData_2D,t_joueur tresor,int player)
{
   t_case v;
   // if (player == 0){ // Quand on est à gauche
      if(choix==0) //voisin haut
      {
	 if(c.y==0) v.y=sizeY-1; //Sur la premiere ligne
	 else v.y=c.y-1; //N'importe où 
	 v.x=c.x;  
      }
      else if(choix==1) //voisin droite
      {
	 if(c.x==sizeX-1) v.x=0; //Sur la derniere colone
	 else v.x=c.x+1; //N'importe où 
	 v.y=c.y;  
      }
      else if(choix==2) //voisin bas
      {
	 if(c.y==sizeY-1) v.y=0; //Sur la derniere ligne
	 else v.y=c.y+1; //N'importe où 
	 v.x=c.x;  
      }
      else if(choix==3) //voisin gauche 
      {
   	 if(c.x==0) v.x=sizeX-1; //Sur la premiere colone
   	 else v.x=c.x-1; //N'importe où
   	 v.y=c.y;
      }
      //}
   /* else { // Quand on est à droite */
   /*    if(choix==0) //voisin haut */
   /*    { */
   /* 	 if(c.y==0) v.y=sizeY-1; //Sur la premiere ligne */
   /* 	 else v.y=c.y-1; //N'importe où  */
   /* 	 v.x=c.x;   */
   /*    } */
   /*    else if(choix==1) //voisin bas */
   /*    { */
   /* 	 if(c.y==sizeY-1) v.y=0; //Sur la derniere ligne */
   /* 	 else v.y=c.y+1; //N'importe où  */
   /* 	 v.x=c.x;   */
   /*    } */
   /*    else if(choix==2) //voisin gauche */
   /*    { */
   /* 	 if(c.x==0) v.x=sizeX-1; //Sur la premiere colone */
   /* 	 else v.x=c.x-1; //N'importe où  */
   /* 	 v.y=c.y;   */
   /*    } */
   /* } */
   v.cost=c.cost+1;
   v.heuristique=v.cost+abs(tresor.x-v.x)+abs(tresor.y-v.y);
   v.xp=c.x;
   v.yp=c.y;
   v.explor=0;
   if((int)labData_2D[v.y][v.x]==1)
      {
	 v.cost=1000;
      }
   return v;  
  	 
}

t_case recherche(t_case *Liste,int sizeX,int sizeY,t_case c)
{
   int i; 
   t_case precedent;
   for (i=0;i<sizeX*sizeY;i++)
   {
      if(Liste[i].x==c.xp&&Liste[i].y==c.yp) precedent=Liste[i];
   }
   return precedent;
}

int *A_star(t_joueur p1,t_joueur tresor,int sizeX,int sizeY,char **labData_2D,int player)
{
   t_case *Liste=(t_case *)malloc(sizeX*sizeY*sizeof(t_case)); //lise des cases
   t_case c={.x=p1.x,.y=p1.y,.cost=0,.heuristique=1000,.xp=-1,.yp=-1,.explor=0}; //premier élément (joueur)
   int i,ind=1,ind_c=0,ind_plus_petit; //ind_c est l'indice de c dans la liste
   t_case v; //voisin   
   int *tab_temp=(int *)calloc(sizeX*sizeY,sizeof(int));
   Liste[ind_c]=c;
   for(i=1;i<sizeX*sizeY;i++) Liste[i].cost = 1000;
   while(c.x!=tresor.x || c.y!=tresor.y) //tant qu'on est pas arrivé au tresor on continue
      {
	 // printf("\n%d",ind_c);
        printf("Avant les for:c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	 for(i=0;i<4;i++)
	 {
	   
	    v=voisin(c,i,sizeX,sizeY,labData_2D,tresor,player);
	    if(v.cost<100 && dedans(Liste,v,sizeX,sizeY)==0) //Si ce n'est pas un mur et pas dans la liste
	    {
	       Liste[ind]=v; //On ajoute v à la liste
	       printf("entre1\n");
	       ind+=1;
	    }       
	 }
	 printf("Avant la 2eme for:c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	 for(i=0;i<ind;i++) //On prend l'élément le plus proche du tresor inexploré
	 {
	    if(((c.cost+c.heuristique)>=(Liste[i].cost+Liste[i].heuristique) && Liste[i].explor==0) || ((Liste[i].cost+Liste[i].heuristique)>= (c.cost+c.heuristique) && Liste[i].explor==0))  /* La conditon à gauche de || permet de prendre la plus petite case en cost et heuristique */
	                    /* Celui à droite de || permet de prendre un élément de la Liste quand on a déjà regardé tous les cases de même cost et heuristique*/
	    {
	       ind_plus_petit=i;
	       printf("entre2\n");
	       break;
	    }	
	 }
	 printf("ind = %d et indice_petit = %d\n", ind,ind_plus_petit);
	  Liste[ind_c].explor=1;
	  printf("Avant l'affectation: c.x=%d, c.y=%d, c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	  c=Liste[ind_plus_petit];
	  printf("Après l'affectation: c.x = %d, c.y = %d,c.cost=%d, c.heuristique=%d \n", c.x,c.y,c.cost,c.heuristique);
	  //sleep(2);
	  ind_c=ind_plus_petit; 	  
      }
   printf("A la sortie du 1er while: c.x = %d, c.y = %d,c.cost=%d, c.heuristique=%d, c.xp=%d, c.yp=%d \n", c.x,c.y,c.cost,c.heuristique,c.xp,c.yp);
   printf("\n sizeX=%d et sizeY=%d",sizeX,sizeY);
   ind = 0;
   while(c.x!=p1.x || c.y!=p1.y)
   { 
      printf("\nentrée dans le 2eme while\n");
      
      if( (c.x-c.xp==0 && c.y-c.yp==1) || (c.x-c.xp==0 && c.y-c.yp==-sizeY+1) ) // Case précédente en haut
      {
	 printf("1er if!!\n");
	 tab_temp[ind]=MOVE_DOWN;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if ( (c.x-c.xp==0 && c.y-c.yp==-1) || (c.x-c.xp==0 && c.y-c.yp==sizeY-1) ) // Case précédente en bas
      {
	 printf("2er if!!\n");
	 tab_temp[ind]=MOVE_UP;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if ( (c.x-c.xp==1 && c.y-c.yp==0) || (c.x-c.xp==-sizeX+1 && c.y-c.yp==0) ) // case précédente à gauche
      {
	 printf("3er if!!\n");
	 tab_temp[ind]=MOVE_RIGHT;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
      else if ( (c.x-c.xp==-1 && c.y-c.yp==0) || (c.x-c.xp==sizeX-1 && c.y-c.yp==0) ) // case précédente à droite
      {
	 printf("4er if!!\n");
	 tab_temp[ind]=MOVE_LEFT;
	 c=recherche(Liste,sizeX,sizeY,c);
      }
   /*    int taper; */
   /* attendre: */
   /*    if(scanf("%d",&taper)){ */
   /*    printLabyrinth(); */
   /*    printf("le joueur est à:%d %d et le tresor à : %d %d\n",p1.x,p1.y,tresor.x,tresor.y); */
   /*    printf("Avant l'affectation: c.x = %d, c.y = %d, c.xp=%d, c.yp=%d \n", c.x,c.y,c.xp,c.yp); */
   /*    printf("\n sizeX=%d et sizeY=%d",sizeX,sizeY); */
   /*    } */
   /*    else goto attendre; */
      ind+=1;
   }  
   
   int *A_star=(int *)calloc(ind,sizeof(int));
   for(i=0;i<ind;i++)
      {
	 A_star[ind-i-1]=tab_temp[i];
	 printf("\n%d|",tab_temp[i]);
      }
   
   return A_star;
}
