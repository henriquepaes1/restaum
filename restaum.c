#include <stdlib.h>
#include <stdio.h>
#include "restaum.h"


pilha * criaPilha(){
   int i;
   pilha * p = malloc(sizeof(pilha));
   p->topo = 0;
   p->max = 10; /* posicoes do vetor */
   p->v = malloc(p->max * sizeof(tripla));

   for(i = 0; i < p->max; i++){
      p->v[i].lin_a = 0;
      p->v[i].col_a = 0;
      p->v[i].direcao = 0;
   }

   return p;
}

void aumentaPilha (pilha * p){ /*aumenta o tamanho do vetor pilha */
   int i;
   int new_max;
   new_max = 5 + p->max;
   tripla * v2 = malloc(new_max * sizeof(tripla));

   for(i = 0; i < new_max; i++){
      v2[i].lin_a = 0;
      v2[i].col_a = 0;
      v2[i].direcao = 0;
   }

   for(i = 0; i < p->max; i++){
      v2[i].lin_a = p->v[i].lin_a;
      v2[i].col_a = p->v[i].col_a;
      v2[i].direcao = p->v[i].direcao;
   }
   free(p->v);
   p->v = v2;
   p->max = new_max;
   return;
}

void destroiPilha (pilha * p){
   free(p->v);
   free(p);
}

void empilha (pilha * p, tripla x){
   if(p->topo == p->max){
      aumentaPilha(p);
   }
   p->v[p->topo].lin_a = x.lin_a;
   p->v[p->topo].col_a = x.col_a;
   p->v[p->topo].direcao = x.direcao;
   p->topo++;
}

int pilhaVazia (pilha *p){
   return(p->topo == 0);
}

tripla* desempilha (pilha * p, tripla* aux){
   if(!pilhaVazia(p)){
      aux->lin_a = p->v[p->topo - 1].lin_a;
      aux->col_a = p->v[p->topo - 1].col_a;
      aux->direcao = p->v[p->topo - 1].direcao;
      p->topo--;
   }

   else{
      aux->lin_a = 0;
      aux->col_a = 0;
      aux->direcao = 0;
      printf("Erro: pilha vazia");
   }

   return aux;
}

tripla * criaTripla(){
    tripla * tripla_ok;
    tripla_ok = malloc(sizeof(tripla));
    tripla_ok->lin_a = 0;
    tripla_ok->col_a = 0;
    tripla_ok->direcao = 0;

    return tripla_ok;
}
