#include "stateset.h"

StateSetQueue *new_StateSetQueue(int nbStateAFN){
    StateSetQueue *stateset_queue = (StateSetQueue*) malloc(sizeof(StateSetQueue));
    stateset_queue->nbStateAFD = 0;
    stateset_queue->nbStateAFN = nbStateAFN;
    stateset_queue->nbChunkChar = (int)nbStateAFN/sizeof(char);
    stateset_queue->StateSetStart = stateset_queue->StateSetEnd = NULL;
    return stateset_queue;
}

StateSet *new_StateSet(int nbChunkChar){
    StateSet *stateset = (StateSet*) malloc(sizeof(StateSet));
    stateset->stateSetEncoding = calloc(nbChunkChar, sizeof(char));
    stateset->final_type = NON_FINAL;
    stateset->nextStateSet = NULL;
    for(int i = 0; i < NBTRANS; i++)
        stateset->transition[i] = -1;
    return stateset;
}

StateSet *copyStateSet(StateSet *stateset, int nbChunkChar){
    StateSet *copy = new_StateSet(nbChunkChar);
    for(int i = 0; i < nbChunkChar; i++)
        copy->stateSetEncoding[i] = stateset->stateSetEncoding[i];
    copy->final_type = stateset->final_type;
    copy->nextStateSet = stateset->nextStateSet;
    for(int i = 0; i < NBTRANS; i++)
        copy->transition[i] = stateset->transition[i];
    return copy;
}

void freeStateSetQueue(StateSetQueue *stateset_queue){
    while (stateset_queue->StateSetStart->nextStateSet != NULL){
        StateSet *tmp = stateset_queue->StateSetStart;
        stateset_queue->StateSetStart = stateset_queue->StateSetStart->nextStateSet;
        free(tmp);
    }
    free(stateset_queue->StateSetStart);
    free(stateset_queue);
}

int stateSetQueueIsEmpty(StateSetQueue *stateset_queue){ return stateset_queue->nbStateAFD == 0; }

int getNbStateAFD(StateSetQueue *stateset_queue){ return stateset_queue->nbStateAFD; }

int getNbStateAFN(StateSetQueue *stateset_queue){ return stateset_queue->nbStateAFN; }

enum TypeState getTypeStateSet(StateSet *stateset){ return stateset->final_type; }

int stateSetContain(StateSet *stateset, int num_state){
    int chunkSelect = (int)num_state/sizeof(char);
    int bitSelect = (num_state-(chunkSelect*sizeof(char)));
    return stateset->stateSetEncoding[chunkSelect] & (1 << bitSelect);
}

void addStateSet(StateSet *stateset, int num_state){
    int chunkSelect = (int)num_state/sizeof(char);
    int bitSelect = (num_state-(chunkSelect*sizeof(char)));
    stateset->stateSetEncoding[chunkSelect] = stateset->stateSetEncoding[chunkSelect] | (1 << bitSelect);
}

void addStateSetQueue(StateSetQueue *stateset_queue, StateSet *newstateset){
    if(stateSetQueueIsEmpty(stateset_queue)){ stateset_queue->StateSetStart = stateset_queue->StateSetEnd = newstateset; }
    else{
        stateset_queue->StateSetEnd->nextStateSet = newstateset;
        stateset_queue->StateSetEnd = newstateset;
    }

    stateset_queue->nbStateAFD++;
}

int lengthStateSet(StateSet *stateset, int nbStateAFN){
    int length = 0;

    for(int i = 0; i < nbStateAFN; i++)
        if(stateSetContain(stateset, i))
            length++;
    
    return length;
}

StateSet *dequeueStateSetQueue(StateSetQueue *stateset_queue){
    if(stateSetQueueIsEmpty(stateset_queue)) return NULL;

    StateSet *head = stateset_queue->StateSetStart;
    stateset_queue->StateSetStart = head->nextStateSet;
    stateset_queue->nbStateAFD--;

    return head;
}

void replaceStateSetQueue(StateSetQueue *stateset_queue, StateSet *stateset, int index){
    StateSet *tmp = stateset_queue->StateSetStart;

    StateSet *prevStateSet = NULL;
    while(index && tmp != NULL){
        prevStateSet = tmp;
        tmp = tmp->nextStateSet;
        index--;
    }

    if(index) return;

    if(prevStateSet == NULL)
        stateset_queue->StateSetStart = stateset;
    else
        prevStateSet->nextStateSet = stateset;
        
    while(stateset->nextStateSet != NULL){
        stateset = stateset->nextStateSet;
        stateset_queue->nbStateAFD++;
    }
    stateset->nextStateSet = tmp->nextStateSet;

    if(tmp->nextStateSet == NULL)
        stateset_queue->StateSetEnd = stateset;
    
    // free(tmp);
}

int compareStateSet(StateSet *stateset1, StateSet *stateset2, int nbChunkChar){
    for(int i = 0; i < nbChunkChar; i++)
        if(stateset1->stateSetEncoding[i] ^ stateset2->stateSetEncoding[i]) return 0;
    return 1;
}

int compareStateSetQueue(StateSetQueue *stateset_queue1, StateSetQueue *stateset_queue2){
    if(stateSetQueueIsEmpty(stateset_queue1) || stateSetQueueIsEmpty(stateset_queue2)) return 0;
    else if(getNbStateAFD(stateset_queue1) != getNbStateAFD(stateset_queue2)) return 0;
    else if(stateset_queue1->nbChunkChar != stateset_queue2->nbChunkChar) return 0;

    StateSet *stateset1 = stateset_queue1->StateSetStart;
    StateSet *stateset2 = stateset_queue2->StateSetStart;
    while(stateset1 != NULL){
        if(!compareStateSet(stateset1, stateset2, stateset_queue1->nbChunkChar)) return 0;
        stateset1 = stateset1->nextStateSet;
        stateset2 = stateset2->nextStateSet;
    }

    return 1;
}

int stateSetInStateSetQueue(StateSetQueue *stateset_queue, StateSet *stateset){
    int index = 0;

    StateSet *tmp = stateset_queue->StateSetStart;
    while(tmp != NULL){
        if(compareStateSet(tmp, stateset, stateset_queue->nbChunkChar)) return index;
        tmp = tmp->nextStateSet;
        index++;
    }

    return -1;
}

StateSet *interStateSet(StateSet *statesetX, StateSet *statesetY, int nbChunkChar){
    StateSet *inter = new_StateSet(nbChunkChar);

    for(int i = 0; i < nbChunkChar; i++)
        inter->stateSetEncoding[i] = statesetX->stateSetEncoding[i] & statesetY->stateSetEncoding[i];
    
    return inter;
}

StateSet *withoutStateSet(StateSet *statesetX, StateSet *statesetY, int nbChunkChar){
    StateSet *without = new_StateSet(nbChunkChar);

    StateSet *inter = interStateSet(statesetX, statesetY, nbChunkChar);

    for(int i = 0; i < nbChunkChar; i++)
        without->stateSetEncoding[i] = statesetX->stateSetEncoding[i] & ~inter->stateSetEncoding[i];
    
    free(inter);
    return without;
}