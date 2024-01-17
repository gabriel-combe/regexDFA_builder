#include "minimisation.h"

void AFDComplet(int afd[][DIMAFD], int afdc[][DIMAFD], int nbEtat){
    afdc[nbEtat][0] = 0;
    for(int i = 1; i < DIMAFD; i++)
        afdc[nbEtat][i] = nbEtat;

    for(int i = 0; i < nbEtat; i++)
        for(int j = 0; j < DIMAFD; j++){
            if(afd[i][j] == -1)
                afdc[i][j] = nbEtat;
            else
                afdc[i][j] = afd[i][j];
        }
}

void annotateFinal(StateSetQueue *stateset_queue, int afdc[][DIMAFD]){
    if(stateSetQueueIsEmpty(stateset_queue)) return;

    StateSet *state = stateset_queue->StateSetStart;
    while(state != NULL){
        for(int j = 0; j < getNbStateAFN(stateset_queue); j++)
            if(stateSetContain(state, j) && afdc[j][0])
                state->final_type = FINAL;
        state = state->nextStateSet;
    }
}

StateSetQueue *AFDM(int afdc[][DIMAFD], int nbEtat){
    StateSetQueue *stateset_queueP = new_StateSetQueue(nbEtat);
    StateSetQueue *stateset_queueW = new_StateSetQueue(nbEtat);
    StateSet *state_final = new_StateSet(stateset_queueP->nbChunkChar);
    StateSet *state_nonfinal = new_StateSet(stateset_queueP->nbChunkChar);

    state_final->final_type = FINAL;

    for(int i = 0; i < nbEtat; i++)
        if(afdc[i][0])
            addStateSet(state_final, i);
        else
            addStateSet(state_nonfinal, i);
    
    addStateSetQueue(stateset_queueP, state_final);
    addStateSetQueue(stateset_queueP, state_nonfinal);

    addStateSetQueue(stateset_queueW, state_final);
    addStateSetQueue(stateset_queueW, state_nonfinal);

    while(!stateSetQueueIsEmpty(stateset_queueW)){
        StateSet *A = dequeueStateSetQueue(stateset_queueW);

        for(int i = 1; i < DIMAFD; i++){
            StateSet *X = new_StateSet(stateset_queueP->nbChunkChar);
            for(int j = 0; j < nbEtat; j++)
                if(stateSetContain(A, afdc[j][i]))
                    addStateSet(X, j);
            
            StateSet *Y = stateset_queueP->StateSetStart;
            int state_index = 0;
            while(Y != NULL){
                StateSet *inter = interStateSet(X, Y, stateset_queueP->nbChunkChar);
                int lengthInter = lengthStateSet(inter, stateset_queueP->nbStateAFN);
                if(!lengthInter){
                    Y = Y->nextStateSet;
                    state_index++;
                    continue; 
                }

                StateSet *without = withoutStateSet(Y, X, stateset_queueP->nbChunkChar);
                int lengthWithout = lengthStateSet(without, stateset_queueP->nbStateAFN);
                if(!lengthWithout){
                    Y = Y->nextStateSet;
                    state_index++;
                    continue; 
                }

                StateSet *interCopyP = copyStateSet(inter, stateset_queueP->nbChunkChar);
                StateSet *withoutCopyP = copyStateSet(without, stateset_queueP->nbChunkChar);
                interCopyP->nextStateSet = withoutCopyP;
                replaceStateSetQueue(stateset_queueP, interCopyP, state_index++);
                state_index++;

                int indexComp = stateSetInStateSetQueue(stateset_queueW, Y);
                if(indexComp != -1){
                    StateSet *interCopyW = copyStateSet(inter, stateset_queueP->nbChunkChar);
                    StateSet *withoutCopyW = copyStateSet(without, stateset_queueP->nbChunkChar);
                    interCopyW->nextStateSet = withoutCopyW;
                    replaceStateSetQueue(stateset_queueW, interCopyW, indexComp);
                }
                else{
                    if(lengthInter <= lengthWithout){
                        addStateSetQueue(stateset_queueW, inter);
                    }else addStateSetQueue(stateset_queueW, without);
                }

                Y = withoutCopyP->nextStateSet;
            }
        }
    }

    return stateset_queueP;
}

void stateSetQueue2AFDM(StateSetQueue *stateset_queue, int afdc[][DIMAFD], int afdm[][DIMAFD], int nbEtatAFDC){
    if(stateSetQueueIsEmpty(stateset_queue)) return;

    int stateStart;
    int stateEnd;
    StateSet *current_stateset;
    for(int i = 0; i < getNbStateAFN(stateset_queue)-1; i++){
        stateStart = 0;
        current_stateset = stateset_queue->StateSetStart;
        while(!stateSetContain(current_stateset, i) && current_stateset->nextStateSet != NULL){
            stateStart++;
            current_stateset = current_stateset->nextStateSet;
        }

        afdm[stateStart][0] = afdc[i][0];
        for(int j = 1; j < DIMAFD; j++){
            stateEnd = 0;
            if(afdc[i][j] == nbEtatAFDC){ afdm[stateStart][j] = -1; continue; }

            current_stateset = stateset_queue->StateSetStart;
            while(!stateSetContain(current_stateset, afdc[i][j]) && current_stateset->nextStateSet != NULL){
                stateEnd++;
                current_stateset = current_stateset->nextStateSet;
            }

            afdm[stateStart][j] = stateEnd;
        }
    }
}

int findInit(int afd[][DIMAFD], int nbEtat){
    for(int i = 0; i < nbEtat; i++)
        if(!afd[i][0]) return i;
    return -1;
}


void printPartition(StateSetQueue *stateset_queue){
    StateSet *stateset = stateset_queue->StateSetStart;
    for(int i = 0; i < getNbStateAFD(stateset_queue); i++){
        printf("Etat : %d %s : { ", i, stateset->final_type? "FINAL": "");
        for(int j = 0; j < getNbStateAFN(stateset_queue); j++){
            if(!stateSetContain(stateset, j)) continue;
            printf("%d ", j);
        }
        printf("}\n");
        stateset = stateset->nextStateSet;
    }

    free(stateset);
}