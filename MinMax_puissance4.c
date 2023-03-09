#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <time.h>
#include <stdbool.h> //boolean
#include <stdlib.h>  //for system()


#define NB_COLONNE 7
#define NB_LIGNE 6
#define VIDE '.'
#define JOUEUR_PIECE 'X'
#define AI_PIECE 'O'

#define DEBUG false

typedef char Grille[6][7];
typedef int Coups[7];
typedef char Liste[10];

struct Minimax_struct{
    int score, colonne;
};


//entete
void initialiseGrille(Grille grille);
void afficheGrille(Grille grille);

bool est_valide_colonne(Grille grille, int colonne);
int choisirColonne(Grille grille, char pion);
int cherche_ligne(Grille grille, int colonne);


int longueur_liste(Liste liste);
void conversion_filtre(char filtre[], int nb_element, char pion);
void conversion_liste(Liste liste,  int longueur, char pion, char adversaire);
void copie_grille(Grille original, Grille copie);

int coups_possibles(Grille grille, Coups coups);
void jouer(Grille grille, int ligne, int colonne, char pion);
bool est_gagnant(Grille grille, char pion);

struct Minimax_struct minimax(Grille grille, int profondeur, bool maximiseJoueur, char joueur);

int score(Grille grille, char pion);
int calcul(Liste liste, int longueur, char pion);



//initialise la grille
void initialiseGrille(Grille grille){
    int colonne, ligne;
    for(ligne = NB_LIGNE - 1 ; ligne >=0 ; ligne--){
        for(colonne = 0; colonne < NB_COLONNE; colonne++){
            grille[ligne][colonne] = VIDE;
            
        }
    }
    
}

//procedure pour afficher le plateau de jeu
void afficheGrille(Grille grille){
    int colonne, ligne;
    int boucle;
    
	
    //system("clear"); /*clear pour linux, cls pour windows*/
    
    for(ligne = NB_LIGNE - 1 ; ligne >=0 ; ligne--){
        boucle = 0 ;
        while(boucle++ < NB_COLONNE) printf("+---"); 
        
        printf("+\n");
        
        for(colonne = 0; colonne < NB_COLONNE; colonne++){
            printf("| %c ", grille[ligne][colonne]);
            
        }
        printf("|\n");
    }
    
    boucle = 0 ;
    while(boucle++ < NB_COLONNE) printf("+---"); 
    printf("+\n");
}

// controle si une colonne est valide
bool est_valide_colonne(Grille grille, int colonne){
    
    if(grille[NB_LIGNE - 1][colonne] == VIDE){
        return true;
    }else{
        return false; 
    }
     
}

//choisir une colonne
int choisirColonne(Grille grille, char pion){
    int colonne = 0;
    
    printf("Joueur %c : sélectionner votre colonne entre (0 - %d)\n", pion, NB_COLONNE-1);
    scanf("%d", &colonne);
    
       
    if (colonne >= 0 || colonne < NB_COLONNE){
        if (est_valide_colonne(grille, colonne) == true){
            return colonne;
        }else{
            return -1;
        }
        
    }else{
        return -1;
    }
    
    return colonne;
}

//recherche la ligne disponible de la colonne
int cherche_ligne(Grille grille, int colonne){
    bool boolVide = false;
    int ligne = 0;
    
    while(boolVide == false && ligne < NB_LIGNE){
        if(grille[ligne][colonne] == VIDE){
            boolVide = true;
        }else{
            ligne ++;
        }
    }
    
    if(boolVide == false){
        return -1;
    }else{
        return ligne;
    }
}

//retourne la taille d'un tableau
int longueur_liste(Liste liste){
    int longueur = 0;    
    
    while (liste[longueur] != '-'){
        longueur++;
    }
    
    return longueur;
} 

//recherche les coups possibles
int coups_possibles(Grille grille, Coups coups){
    int nbr_element = 0;
    int colonne = 0;
    
    for(colonne = 0; colonne < NB_COLONNE; colonne++){
        
        if(grille[NB_LIGNE-1][colonne] == VIDE){
            coups[nbr_element] = colonne;
            
            nbr_element++;
        }
    }
    
    return nbr_element;
}

//jouer
void jouer(Grille grille, int ligne, int colonne, char pion){
    grille[ligne][colonne] = pion;
}


//conservion de mon filtre
void conversion_filtre(char filtre[], int nb_element, char pion){
    for(int i=0; i<nb_element; i++){
        if(filtre[i] == 'P'){
            filtre[i] = pion;
        }
    }
}

//conversion d'une liste pour le calcul des points
void conversion_liste(Liste liste, int longueur, char pion, char adversaire){
	// b -> bordure du jeu || pion adversaire
	// P -> pion du joueur
	
    int i;
    char temp[10] ={"----------"};
	
	
    
    temp[0] = 'b';
    for (i=0; i < longueur; i++){
        
        if(liste[i] == pion){
            temp[i + 1] = 'P';
        }else{
            if(liste[i] != 'b'){
                if(liste[i] == adversaire){
                    temp[i + 1] = 'b';
                }else{
                    temp[i + 1] = '.';
                }
            }else{
               temp[i + 1] = 'b'; 
            }
        }
    }   
    temp[longueur+1] = 'b';
    memcpy (liste, temp, 10);
    
    
}



bool est_gagnant(Grille grille, char pion){
    int i, ligne, colonne, longueur;
    bool victoire = false;
    
    Liste liste = {"----------"};
    char filtre[] = {"PPPP"};
    char * result;
    
    conversion_filtre(filtre, sizeof(filtre), pion);
    
    
    //lignes
    longueur = 0;
    for (ligne = 0; ligne < NB_LIGNE; ligne++){
        for (colonne = 0; colonne < NB_COLONNE; colonne++){
            liste[colonne] = grille[ligne][colonne];
        }
        
        result = strstr( liste, filtre );
        if ( result != NULL ) {
            victoire = true;
        }
        memset( liste, '-', 10 );
    }
    
    
    //colonnes
    if(victoire == false){
        longueur = 0;
        for (colonne =0; colonne < NB_COLONNE; colonne++){
            for(ligne =0; ligne < NB_LIGNE; ligne++){
                liste[ligne] = grille[ligne][colonne];
            }
            
            result = strstr( liste, filtre );
            if ( result != NULL ) {
                victoire = true;
            }
            memset( liste, '-', 10 );
        }
    }
    
    //diagonales_montantes
    if(victoire == false){
        longueur = 0;
        for (ligne =0; ligne < NB_LIGNE-3; ligne++){
            i=0;
            for(colonne=0; colonne<NB_COLONNE; colonne++){
                if(ligne + i < NB_LIGNE){
                    liste[i] = grille[ligne + i][colonne];
                }
                i++;
            }
            
            result = strstr( liste, filtre );
            if ( result != NULL ) {
                victoire = true;
            }
            memset( liste, '-', 10 );
        }
        
        longueur = 0;
        for (colonne =1; colonne < NB_COLONNE-3; colonne++){
           i=0;
           for(ligne =0; ligne < NB_LIGNE; ligne++){
               if(colonne + i < NB_COLONNE){
                   liste[i] = grille[ligne][colonne + i];
               }
               i++;
           }
           
           result = strstr( liste, filtre );
           if ( result != NULL ) {
               victoire = true;
           }
           memset( liste, '-', 10 );
        }
    }
    
    
    //diagonales_descendantes
    if(victoire == false){
        
        longueur = 0;
        for (ligne = NB_LIGNE-1 ; ligne >= 3; ligne--){
            i=0;
            for(colonne=0; colonne<NB_COLONNE; colonne++){
                if(ligne - i >= 0){
                    liste[i] = grille[ligne - i][colonne];
                }
                i++;
            }
            
            result = strstr( liste, filtre );
            if ( result != NULL ) {
               victoire = true;
            }
            memset( liste, '-', 10 );
        }
        
        longueur = 0;
        for (colonne =1; colonne < NB_COLONNE-3; colonne++){
           i=0;
           for(ligne = NB_LIGNE - 1; ligne > 0; ligne--){
               if(colonne + i < NB_COLONNE){
                   liste[i] = grille[ligne][colonne + i];
               }
               i++;
           }
           
           result = strstr( liste, filtre );
           if ( result != NULL ) {
               victoire = true;
           }
           memset( liste, '-', 10 );
        }
    }
    
    return victoire;
}

int score(Grille grille, char pion){
    int i, ligne, colonne, longueur;
    int score = 0;
    
    Liste liste = {"----------"};
    
    
    //lignes
    longueur = 0;
    for (ligne = 0; ligne < NB_LIGNE; ligne++){
        for (colonne = 0; colonne < NB_COLONNE; colonne++){
            liste[colonne] = grille[ligne][colonne];
            
			//attention si sous la case du dessous est vide alors elle n'est pas jouable
            if(liste[colonne] == VIDE && ligne > 0){
                
                if(grille[ligne - 1][colonne] == VIDE){
                    liste[colonne] = 'b';
                }
            }
            
        }
        
        
        
        longueur = longueur_liste(liste);
        score+= calcul(liste, longueur, pion);
        memset( liste, '-', 10 );
    }
    
    //colonnes
    longueur = 0;
    for (colonne =0; colonne < NB_COLONNE; colonne++){
        for(ligne =0; ligne < NB_LIGNE; ligne++){
            liste[ligne] = grille[ligne][colonne];
        }
        longueur = longueur_liste(liste);
        score+= calcul(liste, longueur, pion);
        memset( liste, '-', 10 );
    }
    
    //diagonales diagonales montantes
    longueur = 0;
    for (ligne =0; ligne < NB_LIGNE-3; ligne++){
        i=0;
        for(colonne=0; colonne<NB_COLONNE; colonne++){
            if(ligne + i < NB_LIGNE){
                liste[i] = grille[ligne + i][colonne];
                
                //attention si sous la case du dessous est vide alors elle n'est pas jouable
                if(liste[i] == VIDE && ligne + i > 0){
                    if(grille[ligne +i - 1][colonne] == VIDE){
                        liste[i] = 'b';
                    }
                }
            }
            i++;
        }
        longueur = longueur_liste(liste);
        score+= calcul(liste, longueur, pion);
        memset( liste, '-', 10 );
    }
    
    longueur = 0;
    for (colonne =1; colonne < NB_COLONNE-3; colonne++){
       i=0;
       for(ligne =0; ligne < NB_LIGNE; ligne++){
           if(colonne + i < NB_COLONNE){
               liste[i] = grille[ligne][colonne + i];
               
			   //attention si sous la case du dessous est vide alors elle n'est pas jouable
               if(liste[i] == VIDE && ligne > 0){
                    if(grille[ligne - 1][colonne + i] == VIDE){
                        liste[i] = 'b';
                    }
                }
           }
           i++;
       }
       longueur = longueur_liste(liste);
       score+= calcul(liste, longueur, pion);
       memset( liste, '-', 10 );
    }
    
    //diagonales diagonales descendantes
    longueur = 0;
    for (ligne = NB_LIGNE-1 ; ligne >= 3; ligne--){
        i=0;
        for(colonne=0; colonne<NB_COLONNE; colonne++){
            if(ligne - i >= 0){
                liste[i] = grille[ligne - i][colonne];
                
				//attention si sous la case du dessous est vide alors elle n'est pas jouable
                if(liste[i] == VIDE && ligne -i > 0){
                    if(grille[ligne  -i - 1][colonne ] == VIDE){
                        liste[i] = 'b';
                    }
                }
            }
            i++;
        }
        longueur = longueur_liste(liste);
        score+= calcul(liste, longueur, pion);
        memset( liste, '-', 10 );
    }
    
    longueur = 0;
    for (colonne =1; colonne < NB_COLONNE-3; colonne++){
       i=0;
       for(ligne = NB_LIGNE - 1; ligne > 0; ligne--){
           if(colonne + i < NB_COLONNE){
               liste[i] = grille[ligne][colonne + i];
               
			   //attention si sous la case du dessous est vide alors elle n'est pas jouable
               if(liste[i] == VIDE && ligne > 0){
                    if(grille[ligne - 1][colonne ] == VIDE){
                        liste[i] = 'b';
                    }
                }
           }
           i++;
       }
       longueur = longueur_liste(liste);
       score+= calcul(liste, longueur, pion);
       memset( liste, '-', 10 );
    }
    
    
    
    return score;
}



//je transforme mes colonnes, lignes et diagonales en liste
//cela permet de rechercher la présence de combinaison et d'asscier des points (positifs ou négatifs)

int calcul(Liste liste, int longueur, char pion){
    int point = 0;
    Liste liste_initiale ={"----------"};
    char adversaire;
    
    memcpy (liste_initiale, liste, 10);
    
    adversaire = JOUEUR_PIECE;
    if(pion == JOUEUR_PIECE ){
       
       adversaire = AI_PIECE; 
    }
    
    
    conversion_liste(liste, longueur, pion, adversaire);
    
    
    //recherche des points
    char filtre_1[] = {"PPPP"};
    char * result_1 = strstr( liste, filtre_1 );
    if ( result_1 != NULL ) {
        point +=100;
        
        
    }
    
    char filtre_2[] = {"P.PP"};
    char * result_2 = strstr( liste, filtre_2 );
    if ( result_2 != NULL ) {
        point +=10;
        
    }
    
    char filtre_3[] = {"PP.P"};
    char * result_3 = strstr( liste, filtre_3 );
    if ( result_3 != NULL ) {
        point +=10;
       
    }
    
    char filtre_4[] = {".PPPb"};
    char * result_4 = strstr( liste, filtre_4 );
    if ( result_4 != NULL ) {
        point +=10;
        
    }
    
    char filtre_5[] = {"bPPP."};
    char * result_5 = strstr( liste, filtre_5 );
    if ( result_5 != NULL ) {
        point +=10;
        
    }
    
    char filtre_6[] = {".PPP."};
    char * result_6 = strstr( liste, filtre_6 );
    if ( result_6 != NULL ) {
        point +=10;
        
    }
    
    char filtre_7[] = {".PP."};
    char * result_7 = strstr( liste, filtre_7 );
    if ( result_7 != NULL ) {
        point +=5;
        
    }
    
    char filtre_8[] = {"bPP."};
    char * result_8 = strstr( liste, filtre_8 );
    if ( result_8 != NULL ) {
        point +=2;
        
    }
    
    char filtre_9[] = {".PPb"};
    char * result_9 = strstr( liste, filtre_9 );
    if ( result_9 != NULL ) {
        point +=2;
        
    }
    
    //point de l'adversaire
    memcpy (liste, liste_initiale, 10);
    conversion_liste(liste, longueur, adversaire, pion);
    
    
    char filtre_10[] = {"PPPP"};
    char * result_10 = strstr( liste, filtre_10 );
    if ( result_10 != NULL ) {
        point -=100;
        
    }
    
    char filtre_11[] = {".PPPb"};
    char * result_11 = strstr( liste, filtre_11 );
    if ( result_11 != NULL ) {
        point -=10;
        
    }
    
    char filtre_12[] = {"bPPP."};
    char * result_12 = strstr( liste, filtre_12 );
    if ( result_12 != NULL ) {
        point -=10;
        
    }
    
    char filtre_13[] = {".PPP."};
    char * result_13 = strstr( liste, filtre_13 );
    if ( result_13 != NULL ) {
        point -=10;
        
    }
    
    char filtre_14[] = {"P.PP"};
    char * result_14 = strstr( liste, filtre_14 );
    if ( result_14 != NULL ) {
        point -=20;
        
    }
    
    char filtre_15[] = {"PP.P"};
    char * result_15 = strstr( liste, filtre_15 );
    if ( result_15 != NULL ) {
        point -=20;
        
    }
    
    char filtre_16[] = {".PP."};
    char * result_16 = strstr( liste, filtre_16 );
    if ( result_16 != NULL ) {
        point -=20;
        
    }
	
	char filtre_17[] = {"bPP."};
    char * result_17 = strstr( liste, filtre_17 );
    if ( result_17 != NULL ) {
        point -=2;
        
    }
    
    char filtre_18[] = {".PPb"};
    char * result_18 = strstr( liste, filtre_18 );
    if ( result_18 != NULL ) {
        point -=2;
        
    }
    
    
    
    return point;
}


//effectuer une copie d'un tableau
void copie_grille(Grille original, Grille copie){
	int ligne, colonne;
	

	for(ligne = NB_LIGNE - 1 ; ligne >=0 ; ligne--){
        for(colonne = 0; colonne < NB_COLONNE; colonne++){
            copie[ligne][colonne]=original[ligne][colonne];
            
        }
    }
} 

//termine la fin d'une branche (node)
bool est_termine_noeud(Grille grille){
    int nbr_coups_possibles;
    Coups coups;
    
    memset( coups, -1, NB_COLONNE );
    
    
    
    nbr_coups_possibles = coups_possibles(grille, coups);
    
    if(nbr_coups_possibles = 0 || est_gagnant(grille, AI_PIECE) == true || est_gagnant(grille, JOUEUR_PIECE) == true){
        return true;
    }else{
        return false;
    }
    
}


//fonction IA minimax
//pour une grille de 7 colonne avec une profondeur de 6  7^6 coups = 117 649
//pour une grille de 7 colonne avec une profondeur de 7  7^7 coups = 823 543

struct Minimax_struct minimax(Grille grille, int profondeur, bool maximiseJoueur, char ordinateur){
    int i, ligne, colonne, new_score, nbr_coups_possibles;
	bool est_termine;
    int value;
	
	char adversaire;
    
    struct Minimax_struct minimax_value;
    
    
    Grille tmp_grille;    
    Coups coups;
    
      
    
    
    adversaire = AI_PIECE;
    if(ordinateur == AI_PIECE){
        adversaire = JOUEUR_PIECE;
    }
    
	//verifie si il y a encore des coups possibles ou si un des joueurs a gagné
    est_termine = est_termine_noeud(grille);
    if(profondeur == 0 || est_termine == true){
        if(est_termine == true){
			
			//je controle si l'ordinateur à gagné
            if(est_gagnant(grille, ordinateur) == true){
				
				//important mon score doit tenir compte de la profondeur
				//afin de prioriser un coup gagnant rapide
                minimax_value.score = 3000000 + profondeur;
                minimax_value.colonne = -1;
                    
                return minimax_value;
            }else if (est_gagnant(grille, adversaire) == true){
				
				//important mon score doit tenir compte de la profondeur
				//afin de prioriser un blocage
                minimax_value.score = -3000000 - profondeur;
                minimax_value.colonne = -1;
                
                return minimax_value;
            }else{
                //plus de coup possible
                minimax_value.score = 0;
                minimax_value.colonne = -1;
                
                return minimax_value;
            }
            
        }else{
            //profondeur est égale à zéro je retourne le score de l'ordinateur
            minimax_value.score = score(grille, ordinateur);
            minimax_value.colonne = -1;
            
            
            return minimax_value;
        }
    }
	
	//initilise ma liste de coups
    memset( coups, -1, NB_COLONNE );
    nbr_coups_possibles = coups_possibles(grille, coups);
    
    //tirage aleatoire
    srand(time(NULL));
    colonne = rand() % (nbr_coups_possibles);
	
	
    if(maximiseJoueur == true){
        //maximise
        value = -100000000;
        
        
        for(i=0; i< nbr_coups_possibles; i++){
            copie_grille(grille, tmp_grille);
            
            ligne = cherche_ligne(tmp_grille, coups[i]);
            jouer(tmp_grille, ligne, coups[i], ordinateur);
            
            minimax_value = minimax(tmp_grille, profondeur -1, false, ordinateur);
            
            if(DEBUG == true){
                printf("Profondeur : %d - Colonne : %d - score MAX : %d \n", profondeur, coups[i], minimax_value.score);
            }
            
            if(minimax_value.score > value){
                value = minimax_value.score;
                colonne = coups[i];
            }
        }
        
        minimax_value.score = value;
        minimax_value.colonne = colonne;
        
        return minimax_value;
        
        
    }else{
        //adversaire minimise
        value = 100000000;
                
        for(i=0; i< nbr_coups_possibles; i++){            
            copie_grille(grille, tmp_grille);
            
            ligne = cherche_ligne(tmp_grille, coups[i]);
            jouer(tmp_grille, ligne, coups[i], adversaire);
			
            minimax_value = minimax(tmp_grille, profondeur -1, true, ordinateur);
            if(DEBUG == true){
                printf("Profondeur : %d - Colonne : %d - score MIN : %d \n", profondeur, coups[i], new_score);
            }
			
            //on conserve le coup ou l'ordinateur fait le plus petit score donc le meilleur coup de l'adversaire
            if(minimax_value.score < value){
                value = minimax_value.score;
                colonne = coups[i];
            }
        }
        
        minimax_value.score = value;
        minimax_value.colonne = colonne;
        
        return minimax_value;
    }
}


int main()
{
    
    
    Grille grille;
    Coups coups;
    char gagnant = '.'; 
    char joueur_courant;
	int colonne, ligne;
    struct Minimax_struct minimax_value;
    
	
	//defini mon joueur courant
    joueur_courant = JOUEUR_PIECE;
    
    
    colonne = -1;
    ligne = -1;
    
	//initialise ma liste de coup
    memset( coups, -1, NB_COLONNE );
    
    initialiseGrille(grille);
    afficheGrille(grille);
    
    while (gagnant == '.'){
        if(joueur_courant == JOUEUR_PIECE){
            
            
			//mode joueur 
            //colonne = choisirColonne(grille, joueur_courant);
            //while (colonne == -1){
            //    colonne = choisirColonne(grille, joueur_courant);
            //}
            
            
            //mode automatique 
            minimax_value = minimax(grille, 6, true, JOUEUR_PIECE);
            colonne = minimax_value.colonne;
            
            
        }else{
            //mise en oeuvre de minimax
            minimax_value = minimax(grille, 7, true, AI_PIECE);
            colonne = minimax_value.colonne;
        }
        
        ligne = cherche_ligne(grille, colonne);
        jouer(grille, ligne, colonne, joueur_courant);
        afficheGrille(grille);
        
        if(est_gagnant(grille, joueur_courant) == true){
            gagnant = joueur_courant;
        }
        
        if(joueur_courant == JOUEUR_PIECE){
            joueur_courant = AI_PIECE;
        }else{
            joueur_courant = JOUEUR_PIECE;
        }
    }
    
    printf("le joueur %c est gagnant\n", gagnant);
    
    /*
    
    
    char grille[6][7] = {
        { "O.OXXXO" }, 
        { "O.OXXOX" }, 
        { "..OXX.." }, 
        { "...O..." }, 
        { "......." }, 
        { "......." }
        };
    afficheGrille(grille);
    
    int nbrpoint =  score(grille, 'X');
    printf("%d\n", nbrpoint);
    
    bool victoire = est_gagnant(grille, 'X');
    if(victoire == true){
        printf("gagnant !!!");
    }
    
    
    struct Minimax_struct minimax_value = minimax(grille, 6, true, 'X');
    printf("%d\n", minimax_value.colonne);
    
    */
    return 0;
}
 
