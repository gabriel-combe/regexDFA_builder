#ifndef THOMPSON_UTILS_H
#define THOMPSON_UTILS_H

#include "arbin.h"

#define SYMBOLE_TAB(symbole) (('a' <= symbole && symbole <= 'z') || symbole=='@')

/**
 * @brief Compte le nombre d'etat qui va etre cree lors de la convertion d'un arbre binaire vers un AFN
 * 
 * @param a     Arbre binaire representant une expression reguliere
 * @return int  Nombre d'etat de l'AFN
 */
int NombreEtat(Arbin a);

/**
 * @brief Convertit un arbre binaire en un AFN par l'algorithme de Thompson
 * 
 * @param a     Arbre binaire d'une expression reguliere a convertir
 * @param afn   Tableau representant l'AFN de l'expression reguliere
 * @param init  Etat initial de l'AFN
 * @return int  Etat final de l'AFN si pas d'erreur, -1 sinon
 */
int arbreVersAFN(Arbin a, int afn[][3], int init);

#endif // THOMPSON_UTILS_H