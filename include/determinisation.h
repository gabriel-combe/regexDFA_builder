#ifndef DETERMINISATION_H
#define DETERMINISATION_H

#include <stdio.h>
#include "stateset.h"
#include "queue.h"

/**
 * @brief Calcul des epsilons transition a partir d'un ensemble d'etats
 * 
 * @param afn           Tableau representant un AFN
 * @param stateset      Ensemble d'etat sur lequel appliquer l'epsilon fermeture
 * @param nbStateAFN    Nombre d'etat de l'AFN
 */
void epsilonFermeture(int afn[][3], StateSet *stateset, int nbStateAFN);

/**
 * @brief Algorithme de determinisation d'un AFN
 * 
 * @param afn               Tableau representant un AFN
 * @param nbStateAFN        Nombre d'etat de l'AFN
 * @param init              Etat initial de l'AFN
 * @param verbose           Active l'affichage des partitions cree au cours de l'algorithme
 * @return StateSetQueue*   Queue d'ensemble d'etat genere apres determinisation 
 *                          (contient les tableaux de transition de l'AFD calcule)
 */
StateSetQueue *determinisation(int afn[][3], int nbStateAFN, int init, int verbose);

/**
 * @brief Convertit une queue d'ensemble d'etat en un tableau representant un AFD equivalent
 * 
 * @param stateset_queue    Queue d'ensemble d'etat a convertir
 * @param afd               Tableau de l'AFD a completer 
 */
void stateSetQueue2AFD(StateSetQueue *stateset_queue, int afd[][DIMAFD]);

/**
 * @brief Affiche le contenu d'une queue d'ensemble d'etat sous forme de transition d'un AFD
 * 
 * @param stateset_queue    Queue d'ensemble d'etat a afficher
 */
void printStateSetQueue(StateSetQueue *stateset_queue);

#endif // DETERMINISATION_H