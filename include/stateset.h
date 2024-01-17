#ifndef STATESET_H
#define STATESET_H

#ifndef NULL
#define NULL 0
#endif 

#include <stdlib.h>

#define NBTRANS     26
#define DIMAFD      NBTRANS+1
#define VERBOSE     1
#define NOVERBOSE   0

/**
 * @brief Enumerateur pour savoir si un etat est final ou non
 * 
 */
enum TypeState{
    NON_FINAL = 0,
    FINAL = 1
};

/**
 * @brief Structure d'ensemble d'etat contenant:
 *  - un ensemble d'etat (encode par bit)
 *  - une variable caracterisant si l'ensemble est final ou non
 *  - un vecteur de transition
 *  - un pointeur vers un autre ensemble d'etat
 *
 */
typedef struct StateSet{
    char *stateSetEncoding;
    enum TypeState final_type;
    char transition[26];

    struct StateSet *nextStateSet;
}StateSet;

/**
 * @brief Structure de queue d'ensemble d'etats contenant:
 *  - le nombre d'etat de l'AFD
 *  - le nombre d'etat de l'AFN
 *  - le nombre de char utilise pour encoder les ensembles d'etats
 *  - un pointeur vers l'avant de la queue 
 *  - un pointeur vers l'arriere de la queue
 *
 */
typedef struct StateSetQueue{
    int nbStateAFD;
    int nbStateAFN;
    int nbChunkChar;

    StateSet *StateSetStart;
    StateSet *StateSetEnd;    
}StateSetQueue;

/**
 * @brief Cree une nouvelle queue d'ensemble d'etat
 * 
 * @param nbStateAFN    Nombre d'etat dans l'AFN
 * @return StateSet*    Pointeur vers la queue d'ensemble d'etat cree
 */
StateSetQueue *new_StateSetQueue(int nbStateAFN);

/**
 * @brief Cree un nouvelle ensemble d'etat
 * 
 * @param nbChunkChar   Nombre de char necessaire pour encoder l'ensemble d'etat
 * @return StateSet*    Pointeur vers l'ensemble d'etat cree
 */
StateSet *new_StateSet(int nbChunkChar);

/**
 * @brief Copie un ensemble d'etat
 * 
 * @param stateset      Ensemble d'etat a copier
 * @param nbChunkChar   Nombre de char necessaire pour encoder l'ensemble d'etat
 * @return StateSet*    Pointeur vers l'ensemble d'etat copie
 */
StateSet *copyStateSet(StateSet *stateset, int nbChunkChar);

/**
 * @brief Libere l'espace memoire utilise par une queue d'ensemble d'etat
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 */
void freeStateSetQueue(StateSetQueue *stateset_queue);

/**
 * @brief Test si une queue d'ensemble d'etat est vide
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 * @return int              1 si c'est vide, 0 sinon
 */
int stateSetQueueIsEmpty(StateSetQueue *stateset_queue);

/**
 * @brief Recupere le nombre d'etat de l'AFD construit
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 * @return int              Nombre d'etat de l'AFD
 */
int getNbStateAFD(StateSetQueue *stateset_queue);

/**
 * @brief Recupere le nombre d'etat de l'AFN construit
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 * @return int              Nombre d'etat de l'AFN
 */
int getNbStateAFN(StateSetQueue *stateset_queue);

/**
 * @brief Recupere le type d'un etat (final ou non)
 * 
 * @param stateset          Pointeur vers un ensemble d'etat
 * @return enum TypeState   Type de l'ensemble d'etat (final ou non)
 */
enum TypeState getTypeStateSet(StateSet *stateset);

/**
 * @brief Test si un ensemble d'etat contient un etat specifique
 * 
 * @param stateset      Pointeur vers un ensemble d'etat
 * @param num_state     Etat a tester
 * @return int          1 si l'etat apprtient a l'ensemble, 0 sinon
 */
int stateSetContain(StateSet *stateset, int num_state);

/**
 * @brief Ajoute un etat a un ensemble d'etat
 * 
 * @param stateset  Pointeur vers un ensemble d'etat
 * @param num_state Etat que l'on souhaite ajouter a l'ensemble
 */
void addStateSet(StateSet *stateset, int num_state);

/**
 * @brief Ajoute un ensemble d'etat a une queue
 * 
 * @param stateset_queue    Pointeur vers la queue d'ensemble d'etat
 * @param newstateset          Pointeur vers l'ensemble d'etat a ajouter
 */
void addStateSetQueue(StateSetQueue *stateset_queue, StateSet *newstateset);

/**
 * @brief Calcul la longeur de l'ensemble d'etat
 * 
 * @param stateset      Ensemble d'etat
 * @param nbStateAFN    Nombre d'element max que l'ensemble peut encoder
 * @return int          Longeur de l'ensemble d'etat
 */
int lengthStateSet(StateSet *stateset, int nbStateAFN);

/**
 * @brief Retire le premier ensemble d'etat d'une queue d'ensemble d'etat
 * 
 * @param stateset_queue    Queue d'ensemble d'etat
 * @return StateSet*        Ensemble d'etat retire de la queue
 */
StateSet *dequeueStateSetQueue(StateSetQueue *stateset_queue);

/**
 * @brief Remplace un ensemble d'etat dans une queue par un ou plusieurs ensembles d'etat
 * 
 * @param stateset_queue    Queue d'ensemble d'etat
 * @param stateset          Pointeur de depart du ou des ensembles d'etat
 * @param index             Indice de l'ensemble d'etat a remplacer
 */
void replaceStateSetQueue(StateSetQueue *stateset_queue, StateSet *stateset, int index);

/**
 * @brief Compare 2 ensembles d'etat 
 * 
 * @param stateset1     Ensemble d'etat 1
 * @param stateset2     Ensemble d'etat 2
 * @param nbChunkChar   Nombre de char utilise pour encoder les ensembles
 * @return int          1 si les ensembles d'etat sont egaux, 0 sinon
 */
int compareStateSet(StateSet *stateset1, StateSet *stateset2, int nbChunkChar);

/**
 * @brief Compare 2 queue d'ensembles d'etat 
 * 
 * @param stateset_queue1   Queue d'ensemble d'etat 1
 * @param stateset_queue2   Queue d'ensemble d'etat 2
 * @return int              1 si les queues d'ensembles d'etat sont egaux, 0 sinon
 */
int compareStateSetQueue(StateSetQueue *stateset_queue1, StateSetQueue *stateset_queue2);

/**
 * @brief Test si un ensemble d'etat appartient deja a une queue
 * 
 * @param stateset_queue    Pointeur vers une queue d'ensemble d'etat
 * @param stateset          Pointeur vers un ensemble d'etat a tester
 * @return int              L'index de l'ensemble d'etat si l'ensemble appartient a la queue, -1 sinon
 */
int stateSetInStateSetQueue(StateSetQueue *stateset_queue, StateSet *stateset);

/**
 * @brief Calcul l'intersection entre 2 ensembles d'etat
 * 
 * @param statesetX     Ensemble d'etat X
 * @param statesetY     Ensemble d'etat Y
 * @param nbChunkChar   Nombre de char utilise pour encoder les ensembles
 * @return StateSet*    Pointeur vers l'ensemble intersection d'etat cree
 */
StateSet *interStateSet(StateSet *statesetX, StateSet *statesetY, int nbChunkChar);

/**
 * @brief Calcul un ensemble d'etat prive d'un autre ensemble d'etat
 * 
 * @param statesetX     Ensemble d'etat X
 * @param statesetY     Ensemble d'etat Y (que l'on souhaite retirer)
 * @param nbChunkChar   Nombre de char utilise pour encoder les ensembles
 * @return StateSet*    Pointeur vers l'ensemble d'etat X prive de Y cree
 */
StateSet *withoutStateSet(StateSet *statesetX, StateSet *statesetY, int nbChunkChar);

#endif // STATESET_H