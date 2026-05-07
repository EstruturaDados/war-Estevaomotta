// Inclusão da biblioteca padrão
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Para calloc, srand, rand
#include <time.h>    // Para time
#include <stdbool.h> // Para tipo bool

#define num_Equipe 2
#define MAPA_LINHAS 5
#define MAPA_COLUNAS 5

// definir a estrutura da Equipe
struct Equipe {
    char nome[50];
    int num_Tropas;
    int pontuacao;
    int tropas_Alocadas[MAPA_LINHAS][MAPA_COLUNAS]; // Mapa de alocação de tropas
};

// prototipos das funções
void cadastrar_Equipe(struct Equipe *equipes);
void batalha(struct Equipe *equipes);
void exibir_Pontuacao(struct Equipe *equipes);
void exibir_mapaGeral(struct Equipe *equipes);
void exibir_mapaTropas(struct Equipe *equipes, int indice_Equipe);
void alocar_Tropas(struct Equipe *equipes);
int rolar_Dado(int lados);
void limpar_Buffer();
void inicializar_Mapas(struct Equipe *equipes);
bool posicao_Valida(int linha, int coluna, int tropas, struct Equipe *equipe);

int main() {
    struct Equipe *equipes;  // Ponteiro para alocação dinâmica
    int quantidade = num_Equipe;
    int opcao;

    // inicializar srand para números aleatórios
    srand(time(NULL));

    // alocacao dinamica com calloc (inicializa com zeros)
    equipes = (struct Equipe*)calloc(quantidade, sizeof(struct Equipe));

    if(equipes == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }
    
    printf("=== SISTEMA DE BATALHA NAVAL - NIVEL AVANCADO ===\n\n");

    // cadastro das equipes
    cadastrar_Equipe(equipes);
    
    // Inicializar os mapas de tropas
    inicializar_Mapas(equipes);
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Exibir estado do mapa geral\n");
        printf("2 - Exibir mapa de tropas de uma equipe\n");
        printf("3 - Alocar tropas no mapa\n");
        printf("4 - Realizar ataque\n");
        printf("5 - Exibir pontuacao\n");
        printf("6 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_Buffer();
        
        switch(opcao) {
            case 1:
                exibir_mapaGeral(equipes);
                break;
            case 2: {
                int equipe;
                printf("Digite o numero da equipe (1 ou 2): ");
                scanf("%d", &equipe);
                limpar_Buffer();
                if(equipe >= 1 && equipe <= num_Equipe) {
                    exibir_mapaTropas(equipes, equipe-1);
                } else {
                    printf("Equipe invalida!\n");
                }
                break;
            }
            case 3:
                alocar_Tropas(equipes);
                break;
            case 4:
                batalha(equipes);
                break;
            case 5:
                exibir_Pontuacao(equipes);
                break;
            case 6:
                printf("Saindo do jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 6);
    
    // Liberar memória alocada
    free(equipes);
    
    return 0;
}

void inicializar_Mapas(struct Equipe *equipes) {
    for(int i = 0; i < num_Equipe; i++) {
        for(int j = 0; j < MAPA_LINHAS; j++) {
            for(int k = 0; k < MAPA_COLUNAS; k++) {
                equipes[i].tropas_Alocadas[j][k] = 0;
            }
        }
    }
    printf("Mapas inicializados com sucesso!\n");
}

void cadastrar_Equipe(struct Equipe *equipes) {
    for(int i = 0; i < num_Equipe; i++) {
        printf("\n=== CADASTRO DA EQUIPE %d ===\n", i+1);
        printf("Digite o nome da equipe: ");
        fgets(equipes[i].nome, sizeof(equipes[i].nome), stdin);
        equipes[i].nome[strcspn(equipes[i].nome, "\n")] = 0; // Remove o \n
        
        printf("Digite o numero de tropas: ");
        scanf("%d", &equipes[i].num_Tropas);
        limpar_Buffer();
        
        equipes[i].pontuacao = 0;
        printf("Equipe %s cadastrada com %d tropas!\n", equipes[i].nome, equipes[i].num_Tropas);
    }
}

bool posicao_Valida(int linha, int coluna, int tropas, struct Equipe *equipe) {
    if(linha < 0 || linha >= MAPA_LINHAS || coluna < 0 || coluna >= MAPA_COLUNAS) {
        printf("Posicao invalida! Fora dos limites do mapa (0-%d).\n", MAPA_LINHAS-1);
        return false;
    }
    
    if(tropas <= 0) {
        printf("Numero de tropas deve ser positivo!\n");
        return false;
    }
    
    if(tropas > equipe->num_Tropas) {
        printf("Tropas insuficientes! Voce tem apenas %d tropas disponiveis.\n", equipe->num_Tropas);
        return false;
    }
    
    return true;
}

void alocar_Tropas(struct Equipe *equipes) {
    int equipe_Indice, linha, coluna, tropas;
    
    printf("\n=== ALOCACAO DE TROPAS NO MAPA ===\n");
    
    // Exibir o mapa atual
    for(int i = 0; i < num_Equipe; i++) {
        printf("\nMapa da equipe %d (%s):\n", i+1, equipes[i].nome);
        exibir_mapaTropas(equipes, i);
    }
    
    printf("\nEscolha a equipe (1 ou 2): ");
    scanf("%d", &equipe_Indice);
    limpar_Buffer();
    
    equipe_Indice--;
    if(equipe_Indice < 0 || equipe_Indice >= num_Equipe) {
        printf("Equipe invalida!\n");
        return;
    }
    
    printf("Equipe: %s\n", equipes[equipe_Indice].nome);
    printf("Tropas disponiveis: %d\n", equipes[equipe_Indice].num_Tropas);
    printf("Digite a linha (0 a %d): ", MAPA_LINHAS-1);
    scanf("%d", &linha);
    printf("Digite a coluna (0 a %d): ", MAPA_COLUNAS-1);
    scanf("%d", &coluna);
    printf("Digite o numero de tropas a alocar: ");
    scanf("%d", &tropas);
    limpar_Buffer();
    
    if(posicao_Valida(linha, coluna, tropas, &equipes[equipe_Indice])) {
        equipes[equipe_Indice].tropas_Alocadas[linha][coluna] += tropas;
        equipes[equipe_Indice].num_Tropas -= tropas;
        printf("%d tropas alocadas na posicao (%d,%d) da equipe %s!\n", 
               tropas, linha, coluna, equipes[equipe_Indice].nome);
        printf("Tropas restantes: %d\n", equipes[equipe_Indice].num_Tropas);
    }
}

void exibir_mapaTropas(struct Equipe *equipes, int indice_Equipe) {
    printf("\n   ");
    for(int j = 0; j < MAPA_COLUNAS; j++) {
        printf(" %2d  ", j);
    }
    printf("\n");
    
    for(int i = 0; i < MAPA_LINHAS; i++) {
        printf("%2d ", i);
        for(int j = 0; j < MAPA_COLUNAS; j++) {
            if(equipes[indice_Equipe].tropas_Alocadas[i][j] > 0) {
                printf("[%2d]", equipes[indice_Equipe].tropas_Alocadas[i][j]);
            } else {
                printf("[  ]");
            }
        }
        printf("\n");
    }
    printf("\nLegenda: [XX] = numero de tropas alocadas\n");
}

void exibir_mapaGeral(struct Equipe *equipes) {
    printf("\n=== MAPA GERAL DE BATALHA ===\n\n");
    
    for(int e = 0; e < num_Equipe; e++) {
        printf("Equipe %d: %s\n", e+1, equipes[e].nome);
        printf("Tropas restantes: %d\n", equipes[e].num_Tropas);
        printf("Pontuacao: %d\n", equipes[e].pontuacao);
        exibir_mapaTropas(equipes, e);
        printf("------------------------\n");
    }
}

void batalha(struct Equipe *equipes) {
    int atacante, defensor;
    int linha, coluna;
    
    printf("\n=== BATALHA ===\n");
    printf("Escolha a equipe atacante (1 ou 2): ");
    scanf("%d", &atacante);
    printf("Escolha a equipe defensora (1 ou 2): ");
    scanf("%d", &defensor);
    limpar_Buffer();
    
    atacante--;  // Ajusta para índice 0
    defensor--;
    
    if(atacante == defensor) {
        printf("Uma equipe nao pode atacar a si mesma!\n");
        return;
    }
    
    // Mostrar mapa do defensor para escolher posicao de ataque
    printf("\nMapa do defensor (%s):\n", equipes[defensor].nome);
    exibir_mapaTropas(equipes, defensor);
    
    printf("\nEscolha a posicao para atacar:\n");
    printf("Digite a linha (0 a %d): ", MAPA_LINHAS-1);
    scanf("%d", &linha);
    printf("Digite a coluna (0 a %d): ", MAPA_COLUNAS-1);
    scanf("%d", &coluna);
    limpar_Buffer();
    
    if(linha < 0 || linha >= MAPA_LINHAS || coluna < 0 || coluna >= MAPA_COLUNAS) {
        printf("Posicao invalida!\n");
        return;
    }
    
    int tropas_Defensoras = equipes[defensor].tropas_Alocadas[linha][coluna];
    
    printf("\n=== BATALHA: %s vs %s ===\n", equipes[atacante].nome, equipes[defensor].nome);
    printf("Atacando posicao (%d,%d) que possui %d tropas defensoras\n", linha, coluna, tropas_Defensoras);
    
    // Lançamento de dados
    int ataque = rolar_Dado(20) + (equipes[atacante].num_Tropas / 10);
    int defesa = rolar_Dado(20) + (tropas_Defensoras / 10);
    
    printf("Forca de ataque: %d\n", ataque);
    printf("Forca de defesa: %d\n", defesa);
    
    if(ataque > defesa) {
        int pontos = (ataque - defesa) * 10;
        int tropas_Perdidas = tropas_Defensoras / 2;
        
        equipes[atacante].pontuacao += pontos;
        equipes[defensor].tropas_Alocadas[linha][coluna] -= tropas_Perdidas;
        if(equipes[defensor].tropas_Alocadas[linha][coluna] < 0) {
            equipes[defensor].tropas_Alocadas[linha][coluna] = 0;
        }
        
        printf("%s venceu a batalha!\n", equipes[atacante].nome);
        printf("Ganhou %d pontos!\n", pontos);
        printf("Defensor perdeu %d tropas na posicao atacada!\n", tropas_Perdidas);
        
    } else if(defesa > ataque) {
        int pontos = (defesa - ataque) * 10;
        int tropas_Perdidas = tropas_Defensoras / 4;
        
        equipes[defensor].pontuacao += pontos;
        equipes[atacante].num_Tropas -= tropas_Perdidas;
        if(equipes[atacante].num_Tropas < 0) {
            equipes[atacante].num_Tropas = 0;
        }
        
        printf("%s venceu a batalha!\n", equipes[defensor].nome);
        printf("Ganhou %d pontos!\n", pontos);
        printf("Atacante perdeu %d tropas do exercito principal!\n", tropas_Perdidas);
        
    } else {
        printf("Empate! Ninguem ganha pontos.\n");
        printf("Ambas as equipes perdem 10%% de suas tropas!\n");
        
        int perdas_Atacante = equipes[atacante].num_Tropas / 10;
        int perdas_Defensor = tropas_Defensoras / 10;
        
        equipes[atacante].num_Tropas -= perdas_Atacante;
        equipes[defensor].tropas_Alocadas[linha][coluna] -= perdas_Defensor;
        
        printf("Atacante perdeu %d tropas\n", perdas_Atacante);
        printf("Defensor perdeu %d tropas na posicao\n", perdas_Defensor);
    }
    
    // Verificar vitoria
    int tropas_Total_Defensor = 0;
    for(int i = 0; i < MAPA_LINHAS; i++) {
        for(int j = 0; j < MAPA_COLUNAS; j++) {
            tropas_Total_Defensor += equipes[defensor].tropas_Alocadas[i][j];
        }
    }
    tropas_Total_Defensor += equipes[defensor].num_Tropas;
    
    if(tropas_Total_Defensor <= 0) {
        printf("\n*** PARABENS! %s ELIMINOU TODAS AS TROPAS DE %s! ***\n", 
               equipes[atacante].nome, equipes[defensor].nome);
        equipes[atacante].pontuacao += 500;
    }
}

void exibir_Pontuacao(struct Equipe *equipes) {
    printf("\n=== PONTUACAO DAS EQUIPES ===\n");
    for(int i = 0; i < num_Equipe; i++) {
        printf("Equipe %s: %d pontos\n", equipes[i].nome, equipes[i].pontuacao);
        
        // Calcular total de tropas no mapa
        int tropas_Mapa = 0;
        for(int j = 0; j < MAPA_LINHAS; j++) {
            for(int k = 0; k < MAPA_COLUNAS; k++) {
                tropas_Mapa += equipes[i].tropas_Alocadas[j][k];
            }
        }
        printf("  Tropas no mapa: %d\n", tropas_Mapa);
        printf("  Tropas em reserva: %d\n", equipes[i].num_Tropas);
        printf("  Total de tropas: %d\n", tropas_Mapa + equipes[i].num_Tropas);
    }
}

int rolar_Dado(int lados) {
    return (rand() % lados) + 1;
}

void limpar_Buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}