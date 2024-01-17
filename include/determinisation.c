#include "determinisation.h"

void epsilonFermeture(int afn[][3], StateSet *stateset, int nbStateAFN){

    Queue *stateQueue = new_Queue();
    for(int i = 0; i < nbStateAFN; i++)
        if(stateSetContain(stateset, i))
            enqueue(stateQueue, i);
    
    int current_stateset;
    while(!isEmpty(stateQueue)){
        current_stateset = dequeue(stateQueue);
        if(afn[current_stateset][0] != '@') continue;

        if(!stateSetContain(stateset, afn[current_stateset][1])){
            addStateSet(stateset, afn[current_stateset][1]);
            enqueue(stateQueue, afn[current_stateset][1]);
        }

        if(afn[current_stateset][1] == afn[current_stateset][2]) continue;
        else if(stateSetContain(stateset, afn[current_stateset][2])) continue;

        addStateSet(stateset, afn[current_stateset][2]);
        enqueue(stateQueue, afn[current_stateset][2]);
    }

    freeQueue(stateQueue);
}

StateSetQueue *determinisation(int afn[][3], int nbStateAFN, int init, int verbose){
    StateSetQueue *stateset_queue = new_StateSetQueue(nbStateAFN);
    StateSet *stateset = new_StateSet(stateset_queue->nbChunkChar);
    StateSet *emptyState = new_StateSet(stateset_queue->nbChunkChar); 

    addStateSet(stateset, init);

    epsilonFermeture(afn, stateset, stateset_queue->nbStateAFN);

    addStateSetQueue(stateset_queue, stateset);

    if(verbose) printf("\n");
    
    while(stateset != NULL){
        stateset->final_type = stateSetContain(stateset, nbStateAFN-1);

        if(verbose){
            printf("Partition d'Etats : { ");
            for(int i = 0; i < stateset_queue->nbStateAFN; i++){
                if(!stateSetContain(stateset, i)) continue;
                printf("%d ", i);
            }
            printf("}\n");
        }

        for(int car = 'a'; car <= 'z'; car++){
            StateSet *newstate = new_StateSet(stateset_queue->nbChunkChar);
            
            for(int i = 0; i < stateset_queue->nbStateAFN; i++){
                if(!stateSetContain(stateset, i) || afn[i][0] != car) continue;

                addStateSet(newstate, afn[i][1]);
                addStateSet(newstate, afn[i][2]);
            }

            epsilonFermeture(afn, newstate, stateset_queue->nbStateAFN);

            if(compareStateSet(newstate, emptyState, stateset_queue->nbChunkChar)) continue;

            int index = stateSetInStateSetQueue(stateset_queue, newstate);
            
            if(index == -1){
                stateset->transition[car-'a'] = stateset_queue->nbStateAFD;
                addStateSetQueue(stateset_queue, newstate);
            }else stateset->transition[car-'a'] = index;

        }

        stateset = stateset->nextStateSet;
    }

    return stateset_queue;
}

void stateSetQueue2AFD(StateSetQueue *stateset_queue, int afd[][DIMAFD]){
    if(stateSetQueueIsEmpty(stateset_queue)) return;

    StateSet *current_stateset = stateset_queue->StateSetStart;
    int state_index = 0;
    while(current_stateset != NULL){
        afd[state_index][0] = getTypeStateSet(current_stateset);

        for(int i = 0; i < NBTRANS; i++)
            afd[state_index][i+1] = current_stateset->transition[i];

        state_index++;
        current_stateset = current_stateset->nextStateSet;
    }
}

void printStateSetQueue(StateSetQueue *stateset_queue){
    StateSet *stateset = stateset_queue->StateSetStart;
    for(int i = 0; i < stateset_queue->nbStateAFD; i++){
        printf("Etat : %d %s : ", i, stateset->final_type? "FINAL": "");
        for(int car = 'a'; car <= 'z'; car++){
            if(stateset->transition[car-'a']==-1) continue;
            printf("--%c--> %d, ", car, stateset->transition[car-'a']);
        }
        printf("\n");
        stateset = stateset->nextStateSet;
    }

    free(stateset);
}