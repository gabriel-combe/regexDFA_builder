#ifndef QUEUE_H
#define QUEUE_H

#ifndef NULL
#define NULL 0
#endif 

#include <stdlib.h>

/**
 * @brief Structure de node contenant:
 *  - une valeur entiere
 *  - un pointeur vers une autre node
 * 
 */
typedef struct Node{
    int val;
    struct Node *next;
}Node;

/**
 * @brief Structure de queue d'entier contenant:
 *  - la taille de la queue
 *  - un pointeur vers l'avant de la queue
 *  - un pointeur vers l'arriere de la queue
 * 
 */
typedef struct Queue{
    int size;
    Node *front;
    Node  *rear;
}Queue;

/**
 * @brief Cree une nouvelle node
 * 
 * @param val       La valeur a mettre dans la node
 * @return Node*    Pointeur vers la node cree
 */
Node *new_Node(int val);

/**
 * @brief Cree une nouvelle queue d'entier
 * 
 * @return Queue* Pointeur vers la queue cree
 */
Queue *new_Queue();

/**
 * @brief Test si la queue est vide
 * 
 * @param q     Pointeur vers une queue
 * @return int  1 si la queue est vide, 0 sinon
 */
int isEmpty(Queue *q);

/**
 * @brief Libere l'espace memoire utilise par une queue d'entier
 * 
 * @param q Pointeur vers une queue
 */
void freeQueue(Queue *q);

/**
 * @brief Ajoute une node a une queue d'entier
 * 
 * @param q         Pointeur vers une queue d'entier
 * @param val       Valeur a ajouter a la queue
 */
void enqueue(Queue *q, int val);

/**
 * @brief Retire la premiere node d'une queue et retourne la valeur de cette node
 * 
 * @param q         Pointeur vers une queue d'entier
 * @param result    Valeur de la node retiere
 */
int dequeue(Queue *q);

#endif // QUEUE_H