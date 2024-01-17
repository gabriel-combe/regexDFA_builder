#include "thompson-utils.h"

int NombreEtat(Arbin a){
    if(a==NULL)
        return 0;
    else if(ab_racine(a)=='|' || ab_racine(a)=='*')
        return 2 + NombreEtat(ab_sag(a)) + NombreEtat(ab_sad(a));
    else if(ab_racine(a)=='.')
        return NombreEtat(ab_sag(a)) + NombreEtat(ab_sad(a));
    return 2;
}

int arbreVersAFN(Arbin a, int afn[][3], int init){
    if(init == -1) return init;

    int tmpInit = init;
    if(SYMBOLE_TAB(ab_racine(a))){
        afn[init][0] = ab_racine(a);
        afn[init][1] = init+1;
        afn[init][2] = init+1;
        return init+1;
    }else if(ab_racine(a)=='.'){
        init = arbreVersAFN(ab_sag(a), afn, init);
        afn[init][0] = '@';
        afn[init][1] = init+1;
        afn[init][2] = init+1;
        return arbreVersAFN(ab_sad(a), afn, init+1);
    }else if(ab_racine(a)=='|'){
        // transition init vers afn partie gauche
        afn[init][0] = '@';
        afn[init][1] = init+1;

        // afn de la partie gauche
        init = arbreVersAFN(ab_sag(a), afn, init+1);

        // tansition init vers afn partie droite
        afn[tmpInit][2] = init+1;
        
        // sauvegarde de l'etat final de l'afn partie gauche
        tmpInit = init;

        // afn de la partie droite
        init = arbreVersAFN(ab_sad(a), afn, init+1);
        
        // transition etat final de l'afn partie gauche 
        // vers l'etat final global
        afn[tmpInit][0] = '@';
        afn[tmpInit][1] = init+1;
        afn[tmpInit][2] = init+1;

        // transition etat final de l'afn partie droite 
        // vers l'etat final global
        afn[init][0] = '@';
        afn[init][1] = init+1;
        afn[init][2] = init+1;

        return init+1;
    }else if(ab_racine(a)=='*'){
        // transition init vers afn partie gauche
        afn[init][0] = '@';
        afn[init][1] = init+1;

        // afn de la partie gauche
        init = arbreVersAFN(ab_sag(a), afn, init+1);

        // transition etat init 
        // vers l'etat final global (epsilon transition)
        afn[tmpInit][2] = init+1;

        // transition etat final de l'afn partie gauche 
        // vers l'etat final global
        afn[init][0] = '@';
        afn[init][1] = init+1;

        // transition etat final de la partie gauche 
        // vers l'etat initial de la partie gauche
        afn[init][2] = tmpInit+1;

        return init+1;
    }

    return -1;
}