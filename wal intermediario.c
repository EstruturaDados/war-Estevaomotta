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
#include <string.h>

#define NUM_TERRITORIOS 5

// Definição da struct Territorio
struct Territorio {
    char nome[50];
    char corExercito[30];
    int numTropas;
};

int main() {
    struct Territorio mapa[NUM_TERRITORIOS];
    int i;
    
    printf("=== SISTEMA DE MAPA DE GUERRA ===\n");
    printf("Cadastre os %d territorios:\n\n", NUM_TERRITORIOS);
    
    // Cadastro dos territórios
    for(i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Territorio %d ---\n", i+1);
        
        // Limpar o buffer do teclado antes do fgets
        while(getchar() != '\n');
        
        // Entrada do nome do território
        printf("Nome do territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        // Remover o '\n' do final da string
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        
        // Entrada da cor do exército
        printf("Cor do exercito dominante: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;
        
        // Entrada do número de tropas
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].numTropas);
        
        printf("\n");
    }
    
    // Exibição do estado atual do mapa
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    printf("--------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-15s | %-10s |\n", "#", "TERRITORIO", "COR DO EXERCITO", "TROPAS");
    printf("--------------------------------------------------------------\n");
    
    for(i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-3d | %-20s | %-15s | %-10d |\n", 
               i+1, 
               mapa[i].nome, 
               mapa[i].corExercito, 
               mapa[i].numTropas);
    }
    
    printf("--------------------------------------------------------------\n");
    
    return 0;
}