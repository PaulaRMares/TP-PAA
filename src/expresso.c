#include "../headers/mapa.h"
#include "../headers/expresso.h"

//  ===============================================================
//  Dependências globais vindas de mapa.c
//  ===============================================================

extern char **mapa;
extern bool **visitado_no_caminho; // Matriz 2D para controle de ciclo
extern int altura, largura;
extern int dur_dim, dur_aum;     // D' e A
extern int total_pecas;          // Total de 'P' no mapa
extern bool encontrar_caminho;   // Flag de sucesso

//  ===============================================================
//  Variáveis estáticas (locais deste arquivo)
//  ===============================================================

//  Ponteiro para a struct de análise (definido em encontrarCaminho)
static Analise *g_analise = NULL;

//  Controla se a impressão de passos está ativa
static bool g_imprimir_passos = true;

//  ===============================================================
//  Funções Auxiliares de Movimento e Impressão
//  ===============================================================

//  Verifica se (x,y) está dentro do mapa
static inline bool dentro(int x, int y) {
    return (x >= 0 && x < altura && y >= 0 && y < largura);
}

//  Retorna se a célula atual permite sair na direção (dx,dy)
static bool permiteSair(char c, int dx, int dy) {
    //  'X', 'P' e 'F' se comportam como '+'
    if (c == 'X' || c == 'P' || c == 'F' || c == 'p') c = '+';

    if (c == '-') {
        return (dy != 0 && dx == 0); // esquerda/direita
    } else if (c == '|') {
        return (dx != 0 && dy == 0); // cima/baixo
    } else if (c == '+') {
        return (dx != 0 || dy != 0); // qualquer direção cardinal
    }
    //  Célula vazia ' ' ou qualquer outra bloqueia
    return false;
}

//  Retorna se a célula de destino (nx, ny) permite ENTRAR vindo do oposto
static bool permiteEntrar(char c, int dx, int dy) {
    //  'X', 'P' e 'F' se comportam como '+'
    if (c == 'X' || c == 'P' || c == 'F') c = '+';

    //  Para "entrar", é equivalente a a célula destino "sair" do oposto
    //  Se estou indo (dx,dy), o oposto é (-dx, -dy).
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

//  Impressão do passo (exigida pelo enunciado)
static void imprimir_passo(int x, int y, int dur, int pecas_coletadas) {
    if (!g_imprimir_passos) return;
    
    int restantes = total_pecas - pecas_coletadas;
    if (restantes < 0) restantes = 0;

    //  Rapazeada, nessa parte a doc fala uma coisa e o print mostra outra,
    //  então confiei no meu coração mesmo, qualquer coisa é só remover o "+1"

    //  ATENÇÃO: O exemplo de saída na doc está matematicamente inconsistente.
    
    printf("Linha: %d, Coluna: %d; D: %d, pecas restantes: %d\n",
           x + 1, y + 1, dur, restantes); // +1 para ser 1-indexado
}

/* ===============================================================
   Função Principal de Backtracking (DFS)
   ===============================================================
   
   Explicação rápida da função recursiva de busca (DFS) que implementa o backtracking.
   
   'x' é a posição X (linha) atual
   'y' é a posição Y (coluna) atual
   'dur' é a durabilidade atual
   'pecas' são as peças coletadas até agora
   'congelado' define se a perda de durabilidade está desativada
   'prof' é a profundidade atual da recursão (para análise)
   
   Por fim, retorna true se encontrou um caminho, false caso contrário.
 */

static bool dfs(int x, int y, int dur, int pecas, bool congelado, int prof) {
    // --- 1. Atualiza Modo de Análise ---
    if (g_analise) {
        g_analise->chamadas_recursivas++;
        if (prof > g_analise->profundidade_max) {
            g_analise->profundidade_max = prof;
        }
    }

    // --- 2. Controle de Ciclo (Backtracking) ---
    // Se já passamos por esta célula *neste caminho atual*, é um ciclo.
    if (visitado_no_caminho[x][y]) {
        return false;
    }
    // Marca a célula como visitada PARA ESTE CAMINHO
    visitado_no_caminho[x][y] = true;

    // --- 3. Processa Célula Atual (Coleta Peça) ---
    bool coletou = false;
    char original = mapa[x][y]; // Guarda o caractere original ('P', 'F', '+', etc.)

    if (original == 'P') {
        coletou = true;
        pecas++;

        // CORREÇÃO: Regra é congelar com 4 peças, não 'total_pecas'
        // obs: minha interpretação da doc
        if (pecas >= 4) {
            congelado = true;
        }
        dur += dur_aum;
        mapa[x][y] = 'p'; // Marca como "coletada temporariamente"
    }

    // --- 4. Imprime o Passo ---
    // A impressão ocorre *depois* de atualizar a durabilidade da peça
    imprimir_passo(x, y, dur, pecas);

    // --- 5. Condição de Parada (Sucesso) ---
    if (original == 'F') {
        encontrar_caminho = true;
        if (g_imprimir_passos) {
            if (pecas >= total_pecas) {
                printf("#caso 2: pegou todas as peças antes do destino\n");
                puts("A jornada será finalizada sem mais desafios");
            } else {
                printf("#caso 1: chegou ao final sem estar reparado\n");
                puts("A tripulação finalizou sua jornada");
            }
        }
        // NÃO desmarca visitado[x][y] se for o FIM
        return true;
    }

    // --- 6. Exploração Recursiva (4 Direções) ---
    static const int DX[4] = {-1, 1, 0, 0}; // Cima, Baixo
    static const int DY[4] = {0, 0, -1, 1}; // Esquerda, Direita

    for (int k = 0; k < 4; k++) {
        int nx = x + DX[k];
        int ny = y + DY[k];

        // Verifica se é uma posição válida e se o movimento é permitido
        if (!dentro(nx, ny)) continue;
        if (!permiteSair(mapa[x][y], DX[k], DY[k])) continue;
        if (!permiteEntrar(mapa[nx][ny], DX[k], DY[k])) continue;

        // Calcula nova durabilidade
        int ndur = dur;
        if (!congelado) {
            ndur -= dur_dim;
        }

        // Condição de Poda: Não pode fazer o movimento se a durabilidade zerar
        if (ndur <= 0) continue;

        // Chama recursivamente
        if (dfs(nx, ny, ndur, pecas, congelado, prof + 1)) {
            return true; // Sucesso, propaga o 'true'
        }
    }

    // --- 7. Backtrack ---
    // Se chegou aqui, nenhum dos 4 caminhos deu certo.
    // Desfaz as alterações para que outros caminhos possam tentar.
    
    visitado_no_caminho[x][y] = false; // <<< PASSO ESSENCIAL DO BACKTRACKING
    
    if (coletou) {
        mapa[x][y] = original; // Devolve a peça 'P' ao mapa
    }

    return false;
}

/* ===============================================================
   Função Pública de Entrada
   =============================================================== */

bool encontrarCaminho(Expresso *e, Analise *analise, bool imprimir_passos) {
    if (!mapa || altura <= 0 || largura <= 0) {
        puts("Mapa nao carregado.");
        if (analise) {
             analise->chamadas_recursivas = 0;
             analise->profundidade_max = 0;
             analise->tempo_ms = 0.0;
        }
        return false;
    }

    //  Configura as variáveis estáticas para o 'dfs'
    g_imprimir_passos = imprimir_passos;
    g_analise = analise; // Permite ao 'dfs' atualizar a struct de análise
    encontrar_caminho = false;

    //  Zera as métricas de análise antes de começar
    if (g_analise) {
        g_analise->chamadas_recursivas = 0;
        g_analise->profundidade_max = 0;
        g_analise->tempo_ms = 0.0;
    }
    
    //  Posição inicial vem da struct 'e' (lida em lerArquivo)
    int sx = e->pos_x;
    int sy = e->pos_y;
    
    e->pecas = 0; // Garante que a contagem de peças comece do zero
    bool congelado_inicial = false; // Começa sem congelamento

    // --- Medição de Tempo ---
    clock_t t0 = clock();

    bool ok = dfs(sx, sy, e->dur_atual, e->pecas, congelado_inicial, 1);

    clock_t t1 = clock();
    // --- Fim da Medição ---

    double elapsed_ms = 0.0;
    if (t1 != (clock_t)-1 && t0 != (clock_t)-1) {
        elapsed_ms = 1000.0 * (double)(t1 - t0) / (double)CLOCKS_PER_SEC;
    }

    //  Preenche a métrica de tempo
    if (g_analise) {
        g_analise->tempo_ms = elapsed_ms;
    }

    //  Imprime a mensagem de falha, se for o caso
    if (!ok && g_imprimir_passos) {
        puts("Apesar da bravura a tripulação falhou em sua jornada.");
    }

    return ok;
}