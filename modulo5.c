#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char codigoEquip[30];
    char tipoConfig[40];
    char valorAntigo[60];
    char valorNovo[60];
    char tecnico[40];
    time_t dataHora;
} Config;

typedef struct NoConfig {
    Config c;
    struct NoConfig *prox;
} NoConfig;

NoConfig *topo = NULL;
int totalConfig = 0;

// imprimir uma configuracao
void imprimirConfig(Config c) {
    char buf[30];
    struct tm *t = localtime(&c.dataHora);
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", t);

    printf("  Equipamento: %s\n", c.codigoEquip);
    printf("  Tipo de config: %s\n", c.tipoConfig);
    printf("  Valor antigo: %s\n", c.valorAntigo);
    printf("  Valor novo: %s\n", c.valorNovo);
    printf("  Tecnico: %s\n", c.tecnico);
    printf("  Data: %s\n", buf);
    printf("  ----------------------\n");
}

// faz um (push)
void registarConfig(char codEquip[], char tipo[], char antigo[], char novo[], char tec[]) {
    Config c;
    strcpy(c.codigoEquip, codEquip);
    strcpy(c.tipoConfig, tipo);
    strcpy(c.valorAntigo, antigo);
    strcpy(c.valorNovo, novo);
    strcpy(c.tecnico, tec);
    c.dataHora = time(NULL);

    NoConfig *novoNo = (NoConfig*)malloc(sizeof(NoConfig));
    novoNo->c = c;
    novoNo->prox = topo;
    topo = novoNo;
    totalConfig++;

    printf("Configuracao registada com sucesso!\n");
}

// pede os dados e regista
void registarConfigManual() {
    char cod[30], tipo[40], antigo[60], novo[60], tec[40];

    printf("Codigo do equipamento: ");
    fgets(cod, sizeof(cod), stdin);
    cod[strcspn(cod, "\n")] = 0;

    printf("Tipo de configuracao (ex: IP, VLAN, Password): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = 0;

    printf("Valor anterior: ");
    fgets(antigo, sizeof(antigo), stdin);
    antigo[strcspn(antigo, "\n")] = 0;

    printf("Valor novo: ");
    fgets(novo, sizeof(novo), stdin);
    novo[strcspn(novo, "\n")] = 0;

    printf("Tecnico responsavel: ");
    fgets(tec, sizeof(tec), stdin);
    tec[strcspn(tec, "\n")] = 0;

    registarConfig(cod, tipo, antigo, novo, tec);
}

// mostra the last configuração feita (topo da pilha)
void verUltimaConfig() {
    if (topo == NULL) {
        printf("Nao ha configuracoes registadas.\n");
        return;
    }
    printf("\n--- Ultima configuracao ---\n");
    imprimirConfig(topo->c);
}

// mostra as N configuracoes mais recentes
void verNConfigs(int n) {
    if (topo == NULL) {
        printf("Nao ha configuracoes registadas.\n");
        return;
    }
    printf("\n--- %d configuracoes mais recentes ---\n", n);
    NoConfig *atual = topo;
    int i = 0;
    while (atual != NULL && i < n) {
        imprimirConfig(atual->c);
        atual = atual->prox;
        i++;
    }
}

// reverte a ultima configuracao - tira do topo da pilha (pop)
void reverterUltima() {
    if (topo == NULL) {
        printf("Nao ha nada para reverter!\n");
        return;
    }

    NoConfig *aRemover = topo;
    printf("\nA reverter a seguinte configuracao:\n");
    imprimirConfig(aRemover->c);

    printf("O valor do equipamento %s volta a ser: %s\n",
           aRemover->c.codigoEquip, aRemover->c.valorAntigo);

    topo = topo->prox;
    free(aRemover);
    totalConfig--;

    printf("Reversao concluida!\n");
}

// ve o historico todo de um equipamento
void historicoEquipamento(char cod[]) {
    printf("\n--- Historico de configuracoes de %s ---\n", cod);
    NoConfig *atual = topo;
    int count = 0;
    while (atual != NULL) {
        if (strcmp(atual->c.codigoEquip, cod) == 0) {
            imprimirConfig(atual->c);
            count++;
        }
        atual = atual->prox;
    }
    if (count == 0) printf("Nao ha configuracoes para esse equipamento.\n");
}

// limpa todo o registo de configuracoes
void limparConfigs() {
    char resposta;
    printf("Tem a certeza que quer apagar todas as configuracoes? (s/n): ");
    scanf(" %c", &resposta);
    getchar();

    if (resposta != 's' && resposta != 'S') {
        printf("Operacao cancelada.\n");
        return;
    }

    NoConfig *atual = topo;
    while (atual != NULL) {
        NoConfig *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    topo = NULL;
    totalConfig = 0;
    printf("Registo de configuracoes limpo!\n");
}

// guarda a pilha em ficheiro binario
void guardarConfigs() {
    FILE *f = fopen("configuracoes.dat", "wb");
    if (f == NULL) {
        printf("Erro ao guardar configuracoes!\n");
        return;
    }
    fwrite(&totalConfig, sizeof(int), 1, f);
    NoConfig *atual = topo;
    while (atual != NULL) {
        fwrite(&atual->c, sizeof(Config), 1, f);
        atual = atual->prox;
    }
    fclose(f);
    printf("Configuracoes guardadas!\n");
}

// carrega a pilha do ficheiro binario
void carregarConfigs() {
    FILE *f = fopen("configuracoes.dat", "rb");
    if (f == NULL) {
        printf("Ficheiro configuracoes.dat nao encontrado.\n");
        return;
    }
    int total;
    fread(&total, sizeof(int), 1, f);

    Config *buf = malloc(total * sizeof(Config));
    fread(buf, sizeof(Config), total, f);

    // ler ao contrario para que assim o topo de pilha fique igual
    for (int i = total - 1; i >= 0; i--) {
        NoConfig *novoNo = (NoConfig*)malloc(sizeof(NoConfig));
        novoNo->c = buf[i];
        novoNo->prox = topo;
        topo = novoNo;
        totalConfig++;
    }
    free(buf);
    fclose(f);
    printf("Carregadas %d configuracoes.\n", total);
}

void menuModulo5() {
    int op;
    do {
        printf("\n=== MODULO 5 - Configuracoes ===\n");
        printf("1 - Registar nova configuracao\n");
        printf("2 - Ver ultima configuracao\n");
        printf("3 - Ver N configuracoes mais recentes\n");
        printf("4 - Reverter ultima configuracao\n");
        printf("5 - Ver historico de um equipamento\n");
        printf("6 - Guardar em ficheiro binario\n");
        printf("7 - Carregar ficheiro binario\n");
        printf("8 - Limpar registo de configuracoes\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1: registarConfigManual(); break;
            case 2: verUltimaConfig(); break;
            case 3: {
                int n;
                printf("Quantas configuracoes? ");
                scanf("%d", &n);
                getchar();
                verNConfigs(n);
                break;
            }
            case 4: reverterUltima(); break;
            case 5: {
                char cod[30];
                printf("Codigo do equipamento: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = 0;
                historicoEquipamento(cod);
                break;
            }
            case 6: guardarConfigs(); break;
            case 7: carregarConfigs(); break;
            case 8: limparConfigs(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while(op != 0);
}
