#include <stdio.h>
#include "restaum.c"


int **criaTabuleiro(int, int);
int **criaGabarito(int**, int, int);
int confere(int**, int**, int, int);
int checaMovimento(int** , int, int, int, int, int, tripla*);
int resta_um(int**, int**, int, int);
void executaMovimento(int**, tripla);
void desfazMovimento(int**, tripla);
void imprimeSolucao(pilha* );
void liberaMatriz(int**, int);
void liberaTripla(tripla*);

int main(){
    int m, n, i, j, jogo;
    /* onde m e n sao as linhas e colunas da matriz do tabuleiro, respectivamente*/
    int** tabuleiro;
    int** gabarito;
    printf("Digite numero de linhas: \n");
    scanf("%d", &m);
    printf("Digite numero de colunas: \n");
    scanf("%d", &n);

    tabuleiro = criaTabuleiro(m, n);
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%d", &tabuleiro[i][j]);
        }
    }
    gabarito = criaGabarito(tabuleiro, m, n);
    printf("\n");
    /* a partir desse ponto, o tabuleiro ja esta pronto para o inicio do jogo*/
    jogo = resta_um(tabuleiro, gabarito, m, n);
    if(!jogo){
        printf("Impossivel\n");
    }
    liberaMatriz(tabuleiro, m);
    liberaMatriz(gabarito, m);
    return 0;
}

int** criaTabuleiro(int linhas, int colunas){ /* aloca matriz do tabuleiro baseada na entrada do usuario*/
    int i;
    int** tab;
    tab = malloc(linhas * sizeof(int*));

    for(i = 0; i < linhas; i++){
        tab[i] = malloc(colunas * sizeof(int));
    }
    return tab;
}


/*Checa os arredores da posição e ve se tem uma tripla valida, se tiver, salva os movimentos na tripla*/

int checaMovimento(int** tab, int max_col, int max_lin, int linha, int coluna, int direcao, tripla* tripla){
    int ok = 0;
    /*Todas as direcoes possuem uma "trava de seguranca", que consiste em ver se as posicoes que vao ser conferidas*/
    /* pertencem ao tabuleiro, vendo se essas não estão fora da memoria alocada para o tabuleiro*/

    if (direcao == 1){/* direcao 1, para cima*/
        if(linha > 1 && tab[linha - 1][coluna] == 1 && tab[linha - 2][coluna] == -1){
            ok = 1;
            tripla->col_a = coluna;
            tripla->lin_a = linha;
            tripla->direcao = direcao;
        }
    }
    else if (direcao == 2){/* direcao 2, para direita*/
        if(coluna < max_col - 2 && tab[linha][coluna + 1] > 0 && tab[linha][coluna + 2] < 0){
            ok = 1;
            tripla->col_a = coluna;
            tripla->lin_a = linha;
            tripla->direcao = direcao;
        }
    }
    else if (direcao == 3){/* direcao 3, para baixo*/
        if(linha < max_lin - 2 && tab[linha + 1][coluna] == 1 && tab[linha + 2][coluna] == -1){
            ok = 1;
            tripla->col_a = coluna;
            tripla->lin_a = linha;
            tripla->direcao = direcao;
        }
    }
    else if(direcao == 4){ /* direcao 4, para a esquerda */
        if(coluna > 1 && tab[linha][coluna - 1] == 1 && tab[linha][coluna - 2] == -1){
            ok = 1;
            tripla->col_a = coluna;
            tripla->lin_a = linha;
            tripla->direcao = direcao;
        }
    }
    return ok;
}

int resta_um(int** tab, int** gab, int linhas, int colunas){ /* algoritmo do jogo*/
    int dir, mover, i, j, possivel, a, b;
    tripla* tripla_atual;
    tripla* aux; /* tripla auxiliar para quando for desempilhar */
    pilha* movimentos;
    tripla_atual = criaTripla();
    aux = criaTripla();
    movimentos = criaPilha();
    dir = 1;
    a= 0; b = 0; /* essas variaveis funcionam como um cursor da posicao que esta sendo analisada*/


    while(!confere(tab, gab, linhas, colunas)){ /* enquanto o tabuleiro nao for igual ao gabarito*/
        mover = 0;
        possivel = 0;
        for(i = a; i < linhas && mover == 0; i++){ /* quando mover, eh necessario voltar para o comeco */
            for(j = b; j < colunas && mover == 0; j++){
                mover = 0;
                if(tab[i][j] == 1){
                    dir = 1;
                    while(dir <= 4 && mover == 0){ /* checa se um pino tem algum movimento possivel*/
                        mover = checaMovimento(tab, colunas, linhas, i, j, dir, tripla_atual);
                        dir++;
                    }
                    if(mover){
                        empilha(movimentos, *tripla_atual);
                        executaMovimento(tab, *tripla_atual); /* realiza o movimento no tabuleiro*/
                        dir = 1; /* reseta a direcao para a proxima iteracao*/
                        possivel = 1; /* foi possivel fazer um movimento com uma das pecas do tabuleiro*/
                    }
                }
            }
            b = 0; /* zera colunas para aumentar a linha */
        }
        a = 0;
        b = 0;

        /* ao sair do loop, todo o tabuleiro foi percorrido, se nenhum movimento possivel for encontrado, aplica o backtrack*/

        if(possivel == 0){
            if(pilhaVazia(movimentos)){ /* se a pilha estiver vazia, nenhuma decisao pode ser mudada */
                return 0;
            }
            mover = 0;
            tripla_atual = desempilha(movimentos, aux);/* desempilhar */
            desfazMovimento(tab, *tripla_atual); /* remove a peca*/
            dir = tripla_atual->direcao + 1;
            /* as linhas abaixo, aproveitam as variaveis i e j ja criadas para representar a linha e coluna da peca anterior */
            i = tripla_atual->lin_a;
            j = tripla_atual->col_a;
            while(dir <= 4 && mover == 0){ /* checa se um pino tem algum movimento possivel*/
                mover = checaMovimento(tab, colunas, linhas, i, j, dir, tripla_atual);
                dir++;
            }
            if(mover){ /*se achar um movimento possivel: muda e segue a partir da proxima peca*/
                empilha(movimentos, *tripla_atual);
                executaMovimento(tab, *tripla_atual);
                mover = 0;
                a = 0;
                b = 0;
            }
            else{
                if(j >= colunas - 1){
                    a = i + 1; /*Se tiver ultrapassado a ultima coluna, volta na proxima linha */
                    b = 0;
                }
                else if(i >= linhas - 1 && j >= colunas - 1){/* se for a ultima posicao possivel */
                    a = 0;
                    b = 0;
                }
                else{ /* se nao, so anda para a proxima coluna */
                    a = i;
                    b = j + 1;
                }

            }
        }
    }
    imprimeSolucao(movimentos); /* se chegar nesse ponto, tem solucao, nao retornou 0 com a pilha vazia */
    liberaTripla(tripla_atual);
    destroiPilha(movimentos);
    return 1;
}

int** criaGabarito(int**tab, int linhas, int colunas){ /* a partir da entrada do usuario, define como deve ser o resultado */
    int** gab;
    int i, j;

    gab = malloc(linhas * sizeof(int*));
     for(i = 0; i < linhas; i++){
        gab[i] = malloc(colunas * sizeof(int));
    }

    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            if(tab[i][j] == 0){
                gab[i][j] = 0;
            }
            else{
                gab[i][j] = tab[i][j] * -1; /* a saida esperada eh o tabuleiro fornecido pelo usuario, invertido e mantendo os zeros*/
            }
        }
    }

    return gab;
}

int confere(int**tab, int**gab, int linhas, int colunas){/* ve se na etapa atual o tabuleiro fornecido eh igual ao gabarito */
    int i, j;

    for(i = 0; i < linhas; i++){
        for(j = 0; j < colunas; j++){
            if(gab[i][j] != tab[i][j]){
                return 0;
            }
        }
    }
  return 1;
}

/* A funcao executa movimenta assume que a direcao calculada possui movimento valido*/
void executaMovimento(int**tab, tripla atual){ /* executa movimento especificado pela tripla no tabuleiro*/
    tab[atual.lin_a][atual.col_a] = -1; /* a posicao atual ficara vazia */

    if (atual.direcao == 1){/* direcao 1, para cima*/
        tab[atual.lin_a - 1][atual.col_a] = -1; /*Posicao "B" do enunciado para a direcao 1*/
        tab[atual.lin_a - 2][atual.col_a] = 1; /*Posicao "C" do enunciado para a direcao 1*/
        /*Esse mesmo padrao se repete para as outras direcoes, mudando apenas linha e coluna*/
    }
    else if (atual.direcao == 2){/* direcao 2, para direita*/
        tab[atual.lin_a][atual.col_a + 1] = -1;
        tab[atual.lin_a][atual.col_a + 2] = 1;
    }
    else if (atual.direcao == 3){/* direcao 3, para baixo*/
        tab[atual.lin_a + 1][atual.col_a] = -1;
        tab[atual.lin_a + 2][atual.col_a] = 1;
    }
    else{ /* direcao 4, para a esquerda */
        tab[atual.lin_a][atual.col_a - 1] = -1;
        tab[atual.lin_a][atual.col_a - 2] = 1;
    }

    return;
}

void desfazMovimento(int** tab, tripla atual){ /*desfaz um movimento executado pela tripla no tabuleiro*/
    tab[atual.lin_a][atual.col_a] = 1; /* a posicao em que a peca estava volta a ser ocupada*/

    /* Muda os entornos de acordo com o movimento a ser desfeito*/

    if (atual.direcao == 1){/* direcao 1, para cima*/
        tab[atual.lin_a - 1][atual.col_a] = 1;
        tab[atual.lin_a - 2][atual.col_a] = -1;

    }
    else if (atual.direcao == 2){/* direcao 2, para direita*/
        tab[atual.lin_a][atual.col_a + 1] = 1;
        tab[atual.lin_a][atual.col_a + 2] = -1;
    }
    else if (atual.direcao == 3){/* direcao 2, para baixo*/
        tab[atual.lin_a + 1][atual.col_a] = 1;
        tab[atual.lin_a + 2][atual.col_a] = -1;
    }
    else{ /* direcao 4, para a esquerda */
        tab[atual.lin_a][atual.col_a - 1] = 1;
        tab[atual.lin_a][atual.col_a - 2] = -1;
    }
    return;
}

void imprimeSolucao(pilha* triplas){ /* imprimindo a solucao no formato especificado no enunciado */
    int i, c_lin, c_col, max;


    for(i = 0; i < triplas->topo; i++){
        if (triplas->v[i].direcao == 1){/* direcao 1, para cima*/
            c_lin = triplas->v[i].lin_a - 2;
            c_col = triplas->v[i].col_a;
        }
        else if (triplas->v[i].direcao == 2){/* direcao 2, para direita*/
            c_lin = triplas->v[i].lin_a;
            c_col = triplas->v[i].col_a + 2;
        }
        else if (triplas->v[i].direcao == 3){/* direcao 2, para baixo*/
            c_lin = triplas->v[i].lin_a + 2;
            c_col = triplas->v[i].col_a;
        }
        else if (triplas->v[i].direcao == 4){ /* direcao 4, para a esquerda */
            c_lin = triplas->v[i].lin_a;
            c_col = triplas->v[i].col_a - 2;
        }
        printf("(%d:%d)-(%d:%d)\n", triplas->v[i].lin_a, triplas->v[i].col_a, c_lin, c_col);
    }
    return;

}

void liberaMatriz(int** matriz, int lin){
    int i;
    for(i = 0; i < lin; i++){
        free(matriz[i]);
    }
    free(matriz);

}

void liberaTripla(tripla* x){
    free(x);
}


