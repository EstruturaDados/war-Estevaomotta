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

// Definição da struct Territorio
struct Territorio {
    char nome[50];
    char corExercito[30];
    int numTropas;
};

// Protótipos das funções
void cadastrarTerritorios(struct Territorio *mapa, int quantidade);
void exibirMapa(struct Territorio *mapa, int quantidade);
void batalha(struct Territorio *mapa, int quantidade);
int rolarDado(int lados);
void limparBuffer();

int main() {
    struct Territorio *mapa;
    int quantidade = 5;
    int opcao;
    
    // Inicializar srand para números aleatórios
    srand(time(NULL));
    
    // Alocação dinâmica com calloc (inicializa com zeros)
    mapa = (struct Territorio*)calloc(quantidade, sizeof(struct Territorio));
    
    if(mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }
    
    printf("=== SISTEMA DE MAPA DE GUERRA - NIVEL AVANCADO ===\n\n");
    
    // Cadastro dos territórios
    cadastrarTerritorios(mapa, quantidade);
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Exibir estado do mapa\n");
        printf("2 - Realizar ataque\n");
        printf("3 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1:
                exibirMapa(mapa, quantidade);
                break;
            case 2:
                batalha(mapa, quantidade);
                break;
            case 3:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 3);
    
    // Liberar memória alocada
    free(mapa);
    
    return 0;
}

void cadastrarTerritorios(struct Territorio *mapa, int quantidade) {
    printf("\n--- CADASTRO DOS TERRITORIOS ---\n");
    
    for(int i = 0; i < quantidade; i++) {
        printf("\n--- Territorio %d ---\n", i+1);
        
        printf("Nome do territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        
        printf("Cor do exercito dominante: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;
        
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].numTropas);
        limparBuffer();
        
        // Validar se o número de tropas é positivo
        if(mapa[i].numTropas <= 0) {
            printf("Numero de tropas invalido! Definindo como 10.\n");
            mapa[i].numTropas = 10;
        }
    }
}

void exibirMapa(struct Territorio *mapa, int quantidade) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    printf("----------------------------------------------------------------\n");
    printf("| %-3s | %-25s | %-15s | %-10s |\n", "#", "TERRITORIO", "COR DO EXERCITO", "TROPAS");
    printf("----------------------------------------------------------------\n");
    
    for(int i = 0; i < quantidade; i++) {
        printf("| %-3d | %-25s | %-15s | %-10d |\n", 
               i+1, 
               mapa[i].nome, 
               mapa[i].corExercito, 
               mapa[i].numTropas);
    }
    
    printf("----------------------------------------------------------------\n");
}

void batalha(struct Territorio *mapa, int quantidade) {
    int atacante, defensor;
    int dadoAtaque, dadoDefesa;
    
    // Exibir territórios disponíveis
    printf("\n=== SIMULACAO DE BATALHA ===\n");
    exibirMapa(mapa, quantidade);
    
    // Escolha do território atacante
    printf("\nEscolha o numero do territorio ATACANTE (1 a %d): ", quantidade);
    scanf("%d", &atacante);
    limparBuffer();
    
    // Escolha do território defensor
    printf("Escolha o numero do territorio DEFENSOR (1 a %d): ", quantidade);
    scanf("%d", &defensor);
    limparBuffer();
    
    // Validar entradas
    if(atacante < 1 || atacante > quantidade || defensor < 1 || defensor > quantidade) {
        printf("\n*** Territorio invalido! ***\n");
        return;
    }
    
    // Converter para índice do vetor (0-based)
    atacante--;
    defensor--;
    
    // Verificar se não é o mesmo território
    if(atacante == defensor) {
        printf("\n*** Um territorio nao pode atacar a si mesmo! ***\n");
        return;
    }
    
    // Verificar se o atacante tem tropas suficientes
    if(mapa[atacante].numTropas <= 1) {
        printf("\n*** Territorio atacante NAO possui tropas suficientes para atacar! ***\n");
        printf("*** E necessario pelo menos 2 tropas para realizar um ataque. ***\n");
        return;
    }
    
    // Verificar se o defensor tem tropas
    if(mapa[defensor].numTropas <= 0) {
        printf("\n*** Territorio defensor ja foi conquistado! ***\n");
        return;
    }
    
    printf("\n--- INICIANDO BATALHA ---\n");
    printf("%s (%s) esta atacando %s (%s)!\n", 
           mapa[atacante].nome, mapa[atacante].corExercito,
           mapa[defensor].nome, mapa[defensor].corExercito);
    
    // Sortear dados
    dadoAtaque = rolarDado(6);
    dadoDefesa = rolarDado(6);
    
    printf("\n🎲 RESULTADO DOS DADOS:\n");
    printf("   Dado de ATAQUE (%s): %d\n", mapa[atacante].nome, dadoAtaque);
    printf("   Dado de DEFESA (%s): %d\n", mapa[defensor].nome, dadoDefesa);
    
    // Lógica da batalha (empate favorece atacante)
    if(dadoAtaque >= dadoDefesa) {
        printf("\n✅ ATACANTE VENCEU a batalha!\n");
        mapa[defensor].numTropas--;
        
        if(mapa[defensor].numTropas <= 0) {
            printf("🏆 CONQUISTA! %s foi conquistado por %s!\n", 
                   mapa[defensor].nome, mapa[atacante].nome);
            printf("   O territorio %s agora esta sob domínio do exercito %s!\n",
                   mapa[defensor].nome, mapa[atacante].corExercito);
            
            // Conquistador assume o território
            strcpy(mapa[defensor].corExercito, mapa[atacante].corExercito);
            mapa[defensor].numTropas = 1; // Fica com 1 tropa após conquista
            printf("   %s agora possui 1 tropa no territorio conquistado.\n", 
                   mapa[defensor].nome);
        } else {
            printf("   %s perdeu 1 tropa! Tropas restantes: %d\n", 
                   mapa[defensor].nome, mapa[defensor].numTropas);
        }
    } else {
        printf("\n❌ DEFENSOR VENCEU a batalha!\n");
        mapa[atacante].numTropas--;
        printf("   %s perdeu 1 tropa! Tropas restantes: %d\n", 
               mapa[atacante].nome, mapa[atacante].numTropas);
    }
    
    printf("\n--- ESTADO ATUALIZADO ---\n");
    exibirMapa(mapa, quantidade);
}

int rolarDado(int lados) {
    return (rand() % lados) + 1;
}

void limparBuffer() {
    while(getchar() != '\n');
}