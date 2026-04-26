// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NUM_TERRITORIOS 5
#define DADO_LADOS 6
#define TROPAS_INICIAIS 20
#define MAX_NOME 50
#define MAX_COR 30

// Estruturas

typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numTropas;
} Territorio;

typedef struct {
    int tipo;  // 1: Destruir exercito Verde, 2: Conquistar 3 territorios
    int alvo;
    int progresso;
    bool concluida;
} Missao;

// Protótipos das funções
void inicializarJogo(Territorio *mapa, Missao *missao);
void inicializarTerritorios(Territorio *mapa);
void sortearMissao(Missao *missao, const Territorio *mapa);
void exibirMenu();
void exibirMapa(const Territorio *mapa);
void exibirStatusMissao(const Missao *missao);
bool atacar(Territorio *mapa, Missao *missao);
bool realizarBatalha(Territorio *atacante, Territorio *defensor);
void conquistarTerritorio(Territorio *defensor, const Territorio *atacante);
int rolarDado();
void limparBuffer();
void verificarMissao(Territorio *mapa, Missao *missao);
bool verificarVitoria(const Missao *missao);
void exibirMensagemVitoria(const Missao *missao);
bool verificarDestruirExercito(const Territorio *mapa, const char *cor);
int contarTerritoriosConquistados(const Territorio *mapa, const char *corJogador);

int main() {
    Territorio *mapa;
    Missao missao;
    int opcao;
    bool jogoAtivo = true;
    
    // Alocar memória
    mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if(mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }
    
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializar jogo
    inicializarJogo(mapa, &missao);
    
    // Loop principal do jogo
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1:
                if(atacar(mapa, &missao)) {
                    verificarMissao(mapa, &missao);
                    if(verificarVitoria(&missao)) {
                        exibirMensagemVitoria(&missao);
                        jogoAtivo = false;
                    }
                }
                break;
            case 2:
                verificarMissao(mapa, &missao);
                if(verificarVitoria(&missao)) {
                    exibirMensagemVitoria(&missao);
                    jogoAtivo = false;
                } else {
                    exibirStatusMissao(&missao);
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                jogoAtivo = false;
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(jogoAtivo);
    
    // Liberar memória
    free(mapa);
    
    return 0;
}

void inicializarJogo(Territorio *mapa, Missao *missao) {
    printf("========================================\n");
    printf("   SISTEMA DE MAPA DE GUERRA - MISOES   \n");
    printf("========================================\n\n");
    
    inicializarTerritorios(mapa);
    sortearMissao(missao, mapa);
    
    printf("=== JOGO INICIADO ===\n");
    exibirMapa(mapa);
    exibirStatusMissao(missao);
}

void inicializarTerritorios(Territorio *mapa) {
    const char *nomes[] = {"Vale do Silencio", "Montanhas Sombrias", 
                           "Planicie Ardente", "Floresta Encantada", 
                           "Deserto dos Ventos"};
    const char *cores[] = {"Azul", "Verde", "Vermelho", "Roxo", "Amarelo"};
    
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].corExercito, cores[i]);
        mapa[i].numTropas = TROPAS_INICIAIS;
    }
}

void sortearMissao(Missao *missao, const Territorio *mapa) {
    // Sortear tipo de missão: 1 ou 2
    missao->tipo = (rand() % 2) + 1;
    missao->progresso = 0;
    missao->concluida = false;
    
    if(missao->tipo == 1) {
        // Missão: Destruir o exército Verde
        missao->alvo = 1; // Verde
        printf("\n🎯 MISSAO ATRIBUIDA: Destruir o exercito VERDE!\n");
        printf("   Elimine todas as tropas do exercito Verde.\n\n");
    } else {
        // Missão: Conquistar 3 territórios
        missao->alvo = 3;
        printf("\n🎯 MISSAO ATRIBUIDA: Conquistar 3 territorios!\n");
        printf("   Expanda seu dominio conquistando novos territorios.\n\n");
    }
}

void exibirMenu() {
    printf("\n========================================\n");
    printf("              MENU PRINCIPAL            \n");
    printf("========================================\n");
    printf("1 - ATACAR\n");
    printf("2 - VERIFICAR MISSAO\n");
    printf("0 - SAIR\n");
    printf("========================================\n");
}

void exibirMapa(const Territorio *mapa) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    printf("----------------------------------------------------------------\n");
    printf("| %-3s | %-25s | %-15s | %-10s |\n", "#", "TERRITORIO", "COR DO EXERCITO", "TROPAS");
    printf("----------------------------------------------------------------\n");
    
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-3d | %-25s | %-15s | %-10d |\n", 
               i+1, 
               mapa[i].nome, 
               mapa[i].corExercito, 
               mapa[i].numTropas);
    }
    printf("----------------------------------------------------------------\n");
}

void exibirStatusMissao(const Missao *missao) {
    printf("\n📋 STATUS DA MISSAO:\n");
    if(missao->tipo == 1) {
        printf("   Missao: Destruir o exercito VERDE\n");
        printf("   Progresso: %s\n", missao->concluida ? "CONCLUIDA" : "PENDENTE");
    } else {
        printf("   Missao: Conquistar %d territorios\n", missao->alvo);
        printf("   Progresso: %d/%d territorios conquistados\n", 
               missao->progresso, missao->alvo);
    }
}

bool atacar(Territorio *mapa, Missao *missao) {
    int atacante, defensor;
    
    exibirMapa(mapa);
    
    printf("\n--- SELECAO DE ATAQUE ---\n");
    printf("Escolha o territorio ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &atacante);
    limparBuffer();
    
    printf("Escolha o territorio DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &defensor);
    limparBuffer();
    
    // Validar entradas
    if(atacante < 1 || atacante > NUM_TERRITORIOS || 
       defensor < 1 || defensor > NUM_TERRITORIOS) {
        printf("\n*** Territorio invalido! ***\n");
        return false;
    }
    
    atacante--;
    defensor--;
    
    if(atacante == defensor) {
        printf("\n*** Um territorio nao pode atacar a si mesmo! ***\n");
        return false;
    }
    
    // Verificar se o atacante tem tropas suficientes
    if(mapa[atacante].numTropas <= 1) {
        printf("\n*** Tropas insuficientes para atacar! (minimo 2) ***\n");
        return false;
    }
    
    // Verificar se o defensor está ativo
    if(mapa[defensor].numTropas <= 0) {
        printf("\n*** Territorio defensor ja foi conquistado! ***\n");
        return false;
    }
    
    return realizarBatalha(&mapa[atacante], &mapa[defensor]);
}

bool realizarBatalha(Territorio *atacante, Territorio *defensor) {
    int dadoAtaque, dadoDefesa;
    bool conquistou = false;
    
    printf("\n--- BATALHA: %s (%s) vs %s (%s) ---\n",
           atacante->nome, atacante->corExercito,
           defensor->nome, defensor->corExercito);
    
    // Sortear dados
    dadoAtaque = rolarDado();
    dadoDefesa = rolarDado();
    
    printf("\n🎲 RESULTADO DOS DADOS:\n");
    printf("   ATAQUE (%s): %d\n", atacante->nome, dadoAtaque);
    printf("   DEFESA (%s): %d\n", defensor->nome, dadoDefesa);
    
    // Lógica da batalha (empate favorece atacante)
    if(dadoAtaque >= dadoDefesa) {
        printf("\n✅ ATACANTE VENCEU a batalha!\n");
        defensor->numTropas--;
        
        if(defensor->numTropas <= 0) {
            conquistou = true;
            conquistarTerritorio(defensor, atacante);
        } else {
            printf("   %s perdeu 1 tropa! Tropas restantes: %d\n", 
                   defensor->nome, defensor->numTropas);
        }
    } else {
        printf("\n❌ DEFENSOR VENCEU a batalha!\n");
        atacante->numTropas--;
        printf("   %s perdeu 1 tropa! Tropas restantes: %d\n", 
               atacante->nome, atacante->numTropas);
    }
    
    return conquistou;
}

void conquistarTerritorio(Territorio *defensor, const Territorio *atacante) {
    printf("\n🏆 CONQUISTA! %s foi conquistado por %s!\n", 
           defensor->nome, atacante->nome);
    printf("   O territorio %s agora esta sob dominio do exercito %s!\n",
           defensor->nome, atacante->corExercito);
    
    strcpy(defensor->corExercito, atacante->corExercito);
    defensor->numTropas = 1;
    printf("   %s agora possui 1 tropa no territorio conquistado.\n", 
           defensor->nome);
}

int rolarDado() {
    return (rand() % DADO_LADOS) + 1;
}

void limparBuffer() {
    while(getchar() != '\n');
}

void verificarMissao(Territorio *mapa, Missao *missao) {
    if(missao->concluida) return;
    
    if(missao->tipo == 1) {
        // Verificar se exército Verde foi destruído
        if(verificarDestruirExercito(mapa, "Verde")) {
            missao->concluida = true;
        }
    } else {
        // Verificar quantos territórios foram conquistados
        // Considera que o jogador é o exército Azul (posição 0)
        missao->progresso = contarTerritoriosConquistados(mapa, "Azul");
        if(missao->progresso >= missao->alvo) {
            missao->concluida = true;
        }
    }
}

bool verificarVitoria(const Missao *missao) {
    return missao->concluida;
}

void exibirMensagemVitoria(const Missao *missao) {
    printf("\n========================================\n");
    printf("         🎉 VITORIA! 🎉                 \n");
    printf("========================================\n");
    
    if(missao->tipo == 1) {
        printf("   MISSAO CUMPRIDA: Exercito VERDE destruido!\n");
        printf("   A paz reina no continente!\n");
    } else {
        printf("   MISSAO CUMPRIDA: %d territorios conquistados!\n", missao->alvo);
        printf("   Seu imperio se expandiu com sucesso!\n");
    }
    printf("========================================\n");
    printf("          PARABENS, COMANDANTE!          \n");
    printf("========================================\n");
}

bool verificarDestruirExercito(const Territorio *mapa, const char *cor) {
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        if(strcmp(mapa[i].corExercito, cor) == 0 && mapa[i].numTropas > 0) {
            return false; // Ainda existe exercito dessa cor com tropas
        }
    }
    return true; // Exercito destruído
}

int contarTerritoriosConquistados(const Territorio *mapa, const char *corJogador) {
    int count = 0;
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        if(strcmp(mapa[i].corExercito, corJogador) == 0) {
            count++;
        }
    }
    return count;
}