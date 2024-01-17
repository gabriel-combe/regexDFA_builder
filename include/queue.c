#include "queue.h"

Node *new_Node(int val){
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = NULL;
    return new_node;
}

Queue *new_Queue(){
    Queue *q = (Queue*) malloc(sizeof(Queue));
    q->size = 0;
    q->front = q->rear = NULL;
    return q;
}

int isEmpty(Queue *q){ return q->size == 0; }

void freeQueue(Queue *q){
    if(isEmpty(q)){ free(q); return; }

    while (q->front->next != NULL){
        Node *tmp = q->front;
        q->front = q->front->next;
        free(tmp);
    }
    free(q->front);
    free(q);
}


void enqueue(Queue *q, int val){
    Node *new_node = new_Node(val);

    if(isEmpty(q)){ q->front = q->rear = new_node;}
    else{
        q->rear->next = new_node;
        q->rear = new_node;
    }

    q->size++;
}

int dequeue(Queue *q){
    if(isEmpty(q)){ return -1; }

    int res = q->front->val;
    
    q->front = q->front->next;
    q->size--;

    return res;
}