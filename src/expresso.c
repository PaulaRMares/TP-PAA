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
static unsigned char *visitado = NULL; // 0/1 para estado visitado
static bool g_imprimir_passos = true;  // controla impressão de cada passo
extern int total_pecas;


/* ---------------------------------------------------------------
   Estrutura de visitados:
   visitado[pecas][x][y] para evitar revisitar o mesmo estado
   (mesma célula com a mesma quantidade de peças).
   Será alocada como um vetor linear de tamanho altura*largura*5.
   --------------------------------------------------------------- */

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
    if (!g_imprimir_passos) return;
    int restantes = total_pecas - pecas;
    if (restantes < 0) restantes = 0;

    printf("Linha: %d, Coluna: %d; D: %d; pecas_restantes: %d\n",
           x, y, dur, restantes);
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

    bool coletou = false;
    char original = mapa[x][y];
    if (mapa[x][y] == 'P') {
        coletou = true;
        pecas++;
        if (pecas >= total_pecas) congelado = true;
        dur += dur_aum;
        mapa[x][y] = 'p';
    }

    imprimir_passo(x, y, dur, pecas);

    if (mapa[x][y] == 'F' && dur > 0) {
        encontrar_caminho = true;
        if (g_imprimir_passos) {
            if (pecas >= total_pecas) puts("A jornada sera finalizada sem mais desafios.");
            else                      puts("A tripulacao finalizou sua jornada.");
        }
        return true;
    }

    size_t idx = IDX(pecas > 4 ? 4 : pecas, x, y);
    if (visitado[idx]) {
        if (coletou) mapa[x][y] = original;
        return false;
    }
    visitado[idx] = 1;

    static const int DX[4] = {-1, 1, 0, 0};
    static const int DY[4] = {0, 0, -1, 1};

    for (int k = 0; k < 4; k++) {
        int nx = x + DX[k], ny = y + DY[k];
        if (!dentro(nx, ny)) continue;
        if (!permiteSair(mapa[x][y], DX[k], DY[k])) continue;
        if (!permiteEntrar(mapa[nx][ny], DX[k], DY[k])) continue;

        int ndur = dur;
        if (!congelado) ndur -= dur_dim;
        if (ndur <= 0) continue;

        if (dfs(nx, ny, ndur, pecas, congelado, prof + 1)) return true;
    }

    if (coletou) mapa[x][y] = original;
    return false;
}


/* ---------------------------------------------------------------
   Função pública: ponto de entrada do backtracking
   - prepara 'visitado'
   - inicia DFS a partir de 'X'
   - imprime análise (chamadas e profundidade)
   --------------------------------------------------------------- */
bool encontrarCaminho(Expresso *e, Analise *analise, bool imprimir_passos) {
    if (!mapa || altura <= 0 || largura <= 0) {
        puts("Mapa nao carregado.");
        if (analise) { analise->tempo_ms = 0.0; analise->chamadas_recursivas = 0; analise->profundidade_max = 0; }
        return false;
    }

    liberarVisitados();
    size_t total = (size_t)altura * (size_t)largura * 5u; // 0..4 peças
    visitado = (unsigned char *)calloc(total, sizeof(unsigned char));
    if (!visitado) {
        puts("Falha ao alocar visitado.");
        if (analise) { analise->tempo_ms = 0.0; analise->chamadas_recursivas = 0; analise->profundidade_max = 0; }
        return false;
    }

    // zera contadores e configura impressão
    chamadas_recursivas = 0;
    profundidade_max = 0;
    encontrar_caminho = false;
    g_imprimir_passos = imprimir_passos;

    // sincroniza parâmetros
    e->dur_atual = dur_atual;
    e->dur_dim   = dur_dim;
    e->dur_aum   = dur_aum;
    e->pecas     = 0;
    e->congelado = false;

    int sx = e->pos_x, sy = e->pos_y;

    // --- medição de tempo (CPU time) ---
    clock_t t0 = clock();

    bool ok = dfs(sx, sy, e->dur_atual, e->pecas, e->congelado, 1);

    clock_t t1 = clock();
    double elapsed_ms = 0.0;
    if (t1 != (clock_t)-1 && t0 != (clock_t)-1) {
        elapsed_ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC;
    }

    // preenche métricas de análise, se solicitado
    if (analise) {
        analise->tempo_ms = elapsed_ms;
        analise->chamadas_recursivas = chamadas_recursivas;
        analise->profundidade_max = profundidade_max;
    }

    if (!ok && g_imprimir_passos) {
        puts("Apesar da bravura a tripulacao falhou em sua jornada.");
    }

    return ok;
}