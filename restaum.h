/* Pilhas adapatadas para restaum */
typedef struct
{
    int lin_a;
    int col_a;
    /* posicoes da peca que foi movimentada*/
    int direcao;
}tripla;

typedef struct {
  tripla * v;
  int topo;
  int max;
} pilha;


pilha * criaPilha();
void aumentaPilha (pilha *);
void destroiPilha (pilha *);
void empilha (pilha *, tripla);
tripla* desempilha (pilha * p, tripla* aux);
int pilhaVazia (pilha *);
tripla* criaTripla();
