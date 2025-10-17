#include "../headers/mapa.h"
#include "../headers/expresso.h"






/* ===============================================================
   Dependências globais vindas de mapa.c (já existem lá)
   ---------------------------------------------------------------
   - mapa: matriz de caracteres do mapa
   - altura, largura: dimensões do mapa
   - dur_atual, dur_dim, dur_aum: parâmetros D, D', A (também ficam em Expresso)
   - chamadas_recursivas: contador de chamadas (modo análise)
   - encontrar_caminho: flag de sucesso (global)
   =============================================================== */
extern char **mapa;
extern int altura, largura;
extern int dur_atual, dur_dim, dur_aum;
extern int chamadas_recursivas;
extern bool encontrar_caminho;

/* ---------------------------------------------------------------
   Contadores de análise
   --------------------------------------------------------------- */
static int profundidade_max = 0;

/* ---------------------------------------------------------------
   Estrutura de visitados:
   visitado[pecas][x][y] para evitar revisitar o mesmo estado
   (mesma célula com a mesma quantidade de peças).
   Será alocada como um vetor linear de tamanho altura*largura*5.
   --------------------------------------------------------------- */
static unsigned char *visitado = NULL; // 0 = não visitado, 1 = visitado
static inline size_t IDX(int p, int x, int y) {
    return ((size_t)p * altura + (size_t)x) * (size_t)largura + (size_t)y;
}

void liberarVisitados(void) {
    free(visitado);
    visitado = NULL;
}

/* ---------------------------------------------------------------
   Utilidades de movimento
   --------------------------------------------------------------- */

/* Verifica se (x,y) está dentro do mapa */
static inline bool dentro(int x, int y) {
    return (x >= 0 && x < altura && y >= 0 && y < largura);
}

/* Retorna se a célula permite sair na direção (dx,dy) */
static bool permiteSair(char c, int dx, int dy) {
    // normalizamos o tipo de trilha da célula atual
    // 'X' (início), 'P' (peça) e 'F' (fim) se comportam como '+'
    if (c == 'X' || c == 'P' || c == 'F') c = '+';

    if (c == '-') {
        return (dy != 0 && dx == 0); // esquerda/direita
    } else if (c == '|') {
        return (dx != 0 && dy == 0); // cima/baixo
    } else if (c == '+') {
        return (dx != 0 || dy != 0); // qualquer direção cardinal
    }
    // '.' e qualquer outra coisa bloqueia
    return false;
}

/* Retorna se a célula de destino permite ENTRAR vindo do oposto */
static bool permiteEntrar(char c, int dx, int dy) {
    // destino se comporta igual: 'X','P','F' como '+'
    if (c == 'X' || c == 'P' || c == 'F') c = '+';

    // Para "entrar", é equivalente a a célula destino "sair" do oposto
    // Se estou indo (dx,dy), o oposto é (-dx, -dy).
    int ox = -dx, oy = -dy;

    if (c == '-') {
        return (oy != 0 && ox == 0);
    } else if (c == '|') {
        return (ox != 0 && oy == 0);
    } else if (c == '+') {
        return (ox != 0 || oy != 0);
    }
    return false;
}

/* ---------------------------------------------------------------
   Impressão do passo (exigida pelo enunciado)
   --------------------------------------------------------------- */
static void imprimir_passo(int x, int y, int dur, int pecas) {
    int restantes = 4 - pecas;
    printf("Linha: %d, Coluna: %d; D: %d; pecas_restantes: %d\n",
           x, y, dur, restantes < 0 ? 0 : restantes);
}

/* ---------------------------------------------------------------
   DFS/backtracking principal
   Parâmetros mutáveis (passados por valor):
     - x, y: posição atual
     - dur: durabilidade atual
     - pecas: quantas peças já coletou
     - congelado: se a durabilidade parou de diminuir (>=4 peças)
   --------------------------------------------------------------- */
static bool dfs(int x, int y, int dur, int pecas, bool congelado, int prof) {
    chamadas_recursivas++;
    if (prof > profundidade_max) profundidade_max = prof;

    // imprime o passo atual
    imprimir_passo(x, y, dur, pecas);

    // condição de sucesso: chegou ao 'F' e tem durabilidade > 0
    if (mapa[x][y] == 'F' && dur > 0) {
        encontrar_caminho = true;
        // mensagem final conforme situação das peças
        if (pecas >= 4) {
            puts("A jornada sera finalizada sem mais desafios.");
        } else {
            puts("A tripulacao finalizou sua jornada.");
        }
        return true;
    }

    // coleta de peça (se houver) — marcamos com 'p' para não coletar de novo
    bool coletou = false;
    char original = mapa[x][y];
    if (mapa[x][y] == 'P') {
        coletou = true;
        pecas++;
        if (pecas >= 4) congelado = true;       // ao completar 4, congela perda
        dur += dur_aum;                          // ganha A na coleta
        mapa[x][y] = 'p';                        // marca como "peça já coletada"
    }

    // se esse estado já foi visitado, aborta (evita ciclos)
    size_t idx = IDX(pecas > 4 ? 4 : pecas, x, y);
    if (visitado[idx]) {
        // desfaz marca da peça (se coletou aqui agora)
        if (coletou) mapa[x][y] = original;
        return false;
    }
    visitado[idx] = 1;

    // direções cardinais
    static const int DX[4] = {-1, 1, 0, 0}; // cima, baixo, esq, dir
    static const int DY[4] = {0, 0, -1, 1};

    // tenta as 4 direções
    for (int k = 0; k < 4; k++) {
        int nx = x + DX[k];
        int ny = y + DY[k];

        // verifica fronteira
        if (!dentro(nx, ny)) continue;

        // verifica se a célula atual permite SAIR nessa direção
        if (!permiteSair(mapa[x][y], DX[k], DY[k])) continue;

        // verifica se a célula de destino permite ENTRAR vindo do oposto
        if (!permiteEntrar(mapa[nx][ny], DX[k], DY[k])) continue;

        // calcula nova durabilidade
        int ndur = dur;
        if (!congelado) {
            ndur -= dur_dim;     // aplica D' a cada passo enquanto não congelado
        }

        // passo inválido se acabar a durabilidade
        if (ndur <= 0) continue;

        // chamada recursiva
        if (dfs(nx, ny, ndur, pecas, congelado, prof + 1)) {
            // se conseguiu, propaga sucesso
            return true;
        }
    }

    // desfaz a coleta se foi feita aqui (backtrack)
    if (coletou) mapa[x][y] = original;

    return false; // sem saída daqui
}

/* ---------------------------------------------------------------
   Função pública: ponto de entrada do backtracking
   - prepara 'visitado'
   - inicia DFS a partir de 'X'
   - imprime análise (chamadas e profundidade)
   --------------------------------------------------------------- */
bool encontrarCaminho(Expresso *e) {
    // sanity check
    if (!mapa || altura <= 0 || largura <= 0) {
        puts("Mapa nao carregado.");
        return false;
    }

    // aloca visitado: 5 camadas (0..4 pecas)
    liberarVisitados();
    size_t total = (size_t)altura * (size_t)largura * 5u;
    visitado = (unsigned char *)calloc(total, sizeof(unsigned char));
    if (!visitado) {
        puts("Falha ao alocar visitado.");
        return false;
    }

    // zera contadores globais/locais
    chamadas_recursivas = 0;
    profundidade_max = 0;
    encontrar_caminho = false;

    // garante que a struct e os globais estão sincronizados
    e->dur_atual = dur_atual;
    e->dur_dim   = dur_dim;
    e->dur_aum   = dur_aum;
    e->pecas     = 0;
    e->congelado = false;

    // encontra posição inicial (já salva em lerArquivo)
    int sx = e->pos_x;
    int sy = e->pos_y;

    // inicia DFS
    bool ok = dfs(sx, sy, e->dur_atual, e->pecas, e->congelado, 1);

    // relatório de análise (sempre imprime)
    printf("Chamadas recursivas: %d\n", chamadas_recursivas);
    printf("Profundidade maxima: %d\n", profundidade_max);

    if (!ok) {
        puts("Apesar da bravura a tripulacao falhou em sua jornada.");
    }

    return ok;
}