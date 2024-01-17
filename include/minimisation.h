#ifndef MINIMISATION_H
#define MINIMISATION_H

#include <stdio.h>

#include "stateset.h"
#include "queue.h"

/**
 * @brief Complete un AFD avec un etat puit
 * 
 * @param afd       Tableau representant un AFD
 * @param afdc      Tableau representant un AFD complet
 * @param nbEtat    Nombre d'etat de l'AFD
 */
void AFDComplet(int afd[][DIMAFD], int afdc[][DIMAFD], int nbEtat);

/**
 * @brief Annote les partitions d'ensemble d'etat avec FINAL si elles contiennent au moins un etat final et NON_FINAL sinon
 * 
 * @param stateset_queue    Queue d'ensemble d'etat a annoter
 * @param afdc              Tableau representant un AFD complet
 */
void annotateFinal(StateSetQueue *stateset_queue, int afdc[][DIMAFD]);

/**
 * @brief Algorithme de minimisation d'un AFD complet
 * 
 * @param afdc              Tableau representant un AFD complet
 * @param nbEtat            Nombre d'etat de l'AFD complet
 * @return StateSetQueue*   Pointeur vers la queue d'ensemble d'etat cree
 */
StateSetQueue *AFDM(int afdc[][DIMAFD], int nbEtat);

/**
 * @brief Convertit une queue d'ensemble d'etat en un tableau representant un AFD minimal equivalent
 * 
 * @param stateset_queue    Queue d'ensemble d'etat a convertir
 * @param afdc              Tableau representant un AFD complet
 * @param afdm              Tableau representant l'AFD minimal resultant de la convertion
 * @param nbEtatAFDC        Nombre d'etat de l'AFD complet
 */
void stateSetQueue2AFDM(StateSetQueue *stateset_queue, int afdc[][DIMAFD], int afdm[][DIMAFD], int nbEtatAFDC);

/**
 * @brief Trouve l'etat initial d'un AFD
 * 
 * @param afd       Tableau representant un AFD
 * @param nbEtat    Nombre d'etat de l'AFD
 * @return int      Numero de l'etat initial
 */
int findInit(int afd[][DIMAFD], int nbEtat);

/**
 * @brief Affiche les partitions d'etat d'une queue d'ensemble d'etat
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 */
void printPartition(StateSetQueue *stateset_queue);

#endif // MINIMISATION_H