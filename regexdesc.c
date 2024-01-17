/** @file exprdesc.c
*@author Gabriel Combe-Ounkham
*@brief Construction de l'arbre abstrait correspondant a une expression reguliere
*
* Ce fichier contient un constructeur d'arbre abstrait d'expressions reguliere
* compose de caracteres (a-z), d'epsilon (@), du mot vide (0), des opérateurs |, concatenation, * et du parenthésage ().
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "thompson-utils.h"
#include "determinisation.h"
#include "minimisation.h"

/* les macros sont des blocs : pas de ';' apres */
#define AVANCER {jeton=getchar();numcar++;}
#define TEST_AVANCE(prevu) {if (jeton==(prevu)) AVANCER else ERREUR_SYNTAXE}
#define TEST_SYMBOLE (('a' <= jeton && jeton <= 'z') || jeton=='@' || jeton=='0')
#define ERREUR_SYNTAXE {printf("\nMot non reconnu : erreur de syntaxe \
au caractere numero %d \n",numcar); exit(1);}
#define INVITE "Veuillez saisir une expression reguliere suivie de <Entree> SVP (q pour quitter) : \n"

void Z(int verbose); Arbin S(void); Arbin X(Arbin fg); Arbin E(void); Arbin R(Arbin fg); Arbin T(void); Arbin Y(Arbin fg); Arbin F(void); /* déclars */

int jeton;
int numcar=0;

/* AXIOME : X -> EOF|'q''\n'|E'\n'X */
void Z(int verbose){
    Arbin treeRegex;

    if(jeton==EOF)
        exit(0);
    else if(jeton=='q'){
        AVANCER
        if(jeton=='\n')
            return;
        else
            ERREUR_SYNTAXE
    }else{
        treeRegex = S();

        if(jeton=='\n'){
            printf("\nMot reconnu dont l'arbre abstrait est :\n");
            ab_afficher(treeRegex);

            int nbEtat = NombreEtat(treeRegex);
            printf("\nConstruction de l'AFN de %d etats ... \n\n", nbEtat);

            // Creation de la table associe a l'afn
            int afn[nbEtat][3];

            // Etat initial de l'AFN
            int init = 0;

            // Initialisation de l'etat final de l'afn
            afn[nbEtat-1][0] = '$';
            afn[nbEtat-1][1] = -1;
            afn[nbEtat-1][2] = -1;

            // Construction de la table des transitions de l'afn
            // en utilisant l'algorithme de Thompson
            int final = arbreVersAFN(treeRegex, afn, init);

            // Vide l'arbre de l'expression reguliere
            ab_vider(&treeRegex);

            // Affiche le tableau representant l'AFN
            printf("Etal Initial --> %d\nEtat Final --> %d\n\n", init, final);
            printf("Transitions de l'AFN : \n");
            for(int i = 0; i < nbEtat; i++)
                printf("Etat %d --%c--> (%d, %d)\n", i, afn[i][0], afn[i][1], afn[i][2]);

            // Determinise l'AFN
            printf("\nDeterminisation de l'AFN ...\n\n");
            StateSetQueue *stateset_queue = determinisation(afn, nbEtat, init, verbose);

            // Affiche la queue d'ensembles d'etat
            //printf("\nNombre d'etat de l'AFD %d \n", getNbStateAFD(stateset_queue));
            //printStateSetQueue(stateset_queue);

            // Convertit la queue d'ensembles d'etat
            // en un tableau representant l'AFD equivalent
            nbEtat = getNbStateAFD(stateset_queue);
            int afd[nbEtat][DIMAFD];
            stateSetQueue2AFD(stateset_queue, afd);

            freeStateSetQueue(stateset_queue);

            // Affiche le tableau representant l'AFD
            printf("Nombre d'etat de l'AFD %d : \n", nbEtat);
            for(int i = 0; i < nbEtat; i++){
                printf("Etat  : %d %s : ", i, afd[i][0]? "FINAL": "");
                for(int j = 1; j < DIMAFD; j++)
                    if(afd[i][j] != -1)
                        printf("--%c--> %d, ", j-1+'a', afd[i][j]);
                printf("\n");
            }
            printf("\n");

            // Complete l'AFD obtenu
            printf("Completion de l'AFD ...\n\n");
            int afdc[nbEtat+1][DIMAFD];
            AFDComplet(afd, afdc, nbEtat);

            // Affiche le tableau representant l'AFD complet
            printf("Nombre d'etat de l'AFD complet : %d \n", nbEtat+1);
            for(int i = 0; i < nbEtat+1; i++){
                printf("Etat  : %d %s : ", i, afdc[i][0]? "FINAL": "");
                for(int j = 1; j < DIMAFD; j++)
                    if(afdc[i][j] != -1)
                        printf("--%c--> %d, ", j-1+'a', afdc[i][j]);
                printf("\n");
            }
            printf("\n");

            // Minimise l'AFD complet
            printf("Minimisation de l'AFD ...\n\n");
            stateset_queue = AFDM(afdc, nbEtat+1);

            // Affiche l'AFD minimise
            // printPartition(stateset_queue);
            // printf("\n");

            // Annote les partitions en FINAL et NON_FINAL
            annotateFinal(stateset_queue, afdc);

            // Affiche l'AFD minimise annote
            printf("Ensemble des partitions obtenues apres minimisation : \n");
            printPartition(stateset_queue);

            // Convertit les partitions
            // en un tableau representant un AFD minimal
            int nbEtatAFDM = getNbStateAFD(stateset_queue)-1;
            int afdm[nbEtatAFDM][DIMAFD];
            stateSetQueue2AFDM(stateset_queue, afdc, afdm, nbEtat);

            freeStateSetQueue(stateset_queue);

            // Trouve l'etat initial de l'AFDM
            init = findInit(afdm, nbEtatAFDM);

            // Affiche le tableau representant l'AFD complet
            printf("\nNombre d'etat de l'AFD minimal : %d \n", nbEtatAFDM);
            printf("AFDM : Afd : l'etat initial est %d\n", init);
            for(int i = 0; i < nbEtatAFDM; i++){
                printf("Etat  : %d %s : ", i, afdm[i][0]? "FINAL": "");
                for(int j = 1; j < DIMAFD; j++)
                    if(afdm[i][j] != -1)
                        printf("--%c--> %d, ", j-1+'a', afdm[i][j]);
                printf("\n");
            }
            printf("\n\n");

            printf(INVITE);

            numcar=0;
            AVANCER;

            Z(verbose);
        }else ERREUR_SYNTAXE
    }
}

/* S -> EX */
Arbin S(void){
    return X(E());
}

/* X -> |EX */
Arbin X(Arbin fg){
    if(jeton=='|'){
        AVANCER
        Arbin fd = E();

        if(ab_racine(fg)=='0')
            return R(fd);
        else if(ab_racine(fd)=='0')
            return R(fg);

        return X(ab_construire('|', ab_copier(fg), fd));
    }else return fg;
}

/* E -> TR */
Arbin E(void){
    return R(T());
}

/* R -> TR */
Arbin R(Arbin fg){
    if(TEST_SYMBOLE || jeton=='(') {
        Arbin fd = T();

        if(ab_racine(fg)=='@')
            return R(fd);
        else if(ab_racine(fd)=='@')
            return R(fg);

        if(ab_racine(fg)=='0')
            return R(fg);
        else if(ab_racine(fd)=='0')
            return R(fd);

        return R(ab_construire('.', ab_copier(fg), fd));
    }else return fg;
}

/* T -> FY */
Arbin T(void){
    return Y(F());
}

/* Y -> *Y */
Arbin Y(Arbin fg){
    if (jeton=='*') {
        AVANCER
        if(ab_racine(fg)=='*')
            return Y(fg);
        else if(ab_racine(fg)=='@' || ab_racine(fg)=='0')
            return Y(ab_construire('@', ab_creer(), ab_creer()));
        return Y(ab_construire('*', ab_copier(fg), ab_creer()));
    }else return fg;
}

/* F -> (S)|a|b|...|z|@|0 */
Arbin F(void){
    Arbin regex = ab_creer();
    if (jeton=='(') {
        AVANCER
        regex = S();
        TEST_AVANCE(')')
    }else{
        if (TEST_SYMBOLE){
            regex = ab_construire(jeton, ab_creer(), ab_creer());
            AVANCER
        }else ERREUR_SYNTAXE
    }
    return regex;
}

int main(int argc, char *argv[]){
    int verbose = 0;
    if(argc > 2){
        printf("USAGE: ./%s [VERBOSE{0,1}]\n", argv[0]);
        exit(1);
    }else if(argc == 2 && (0<=atoi(argv[1]) && atoi(argv[1])<=1))
        verbose = atoi(argv[1]);

    printf(INVITE);
    AVANCER
    
    Z(verbose);
    
    return 0;
}