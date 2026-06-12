#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <type.h>

// variaveis globais da lista de sensores
No *lista = NULL;
int totalSensores = 0;

// declaracao das funcoes do modulo 4 para criar incidentes
void criarIncidenteSensor(char codSensor[], char estadoSensor[]);

// adiciona sensor no inicio da lista
void adicionarSensor(Sensor s) {
    No *novo = (No*)malloc(sizeof(No));
    novo->s = s;
    novo->prox = lista;
    lista = novo;
    totalSensores++;
}

void imprimirSensor(Sensor s) {
    printf("  Codigo: %s\n", s.codigo);
    printf("  Tipo: %s\n", s.tipo);
    printf("  Valor: %.1f %s\n", s.valor, s.unidade);
    printf("  Estado: %s\n", s.estado);
    printf("  ----------------------\n");
}

int eAnomalo(char estado[]) {
    if (strcmp(estado, "AVISO") == 0) return 1;
    if (strcmp(estado, "CRITICO") == 0) return 1;
    if (strcmp(estado, "FALHA_REDE") == 0) return 1;
    return 0;
}

void importarSensores() {
    FILE *f = fopen("sensores_rack.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir sensores_rack.txt!\n");
        return;
    }

    char linha[200];
    int count = 0;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;

        Sensor s;
        char valorStr[10];

        char *tok = strtok(linha, ";");
        if (tok == NULL) continue;
        strcpy(s.codigo, tok);

        tok = strtok(NULL, ";");
        if (tok == NULL) continue;
        strcpy(s.tipo, tok);

        tok = strtok(NULL, ";");
        if (tok == NULL) continue;
        strcpy(valorStr, tok);
        s.valor = atof(valorStr);

        tok = strtok(NULL, ";");
        if (tok == NULL) continue;
        strcpy(s.unidade, tok);

        tok = strtok(NULL, ";");
        if (tok == NULL) continue;
        strcpy(s.estado, tok);

        adicionarSensor(s);
        count++;
        printf("Leitura importada: %s - %.1f%s - %s\n",
               s.codigo, s.valor, s.unidade, s.estado);

        // se for anomalo cria incidente automatico
        if (eAnomalo(s.estado)) {
            criarIncidenteSensor(s.codigo, s.estado);
        }
    }

    fclose(f);

    // registo no log
    FILE *log = fopen("log_sensores.txt", "a");
    if (log != NULL) {
        time_t agora = time(NULL);
        char buf[30];
        struct tm *t = localtime(&agora);
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", t);
        fprintf(log, "[%s] Importadas %d leituras\n", buf, count);
        fclose(log);
    }

    printf("Total importado: %d sensores\n", count);
}

void listarTodos() {
    if (lista == NULL) {
        printf("Nao ha sensores registados.\n");
        return;
    }
    printf("\n--- Lista de todos os sensores (%d) ---\n", totalSensores);
    No *atual = lista;
    while (atual != NULL) {
        imprimirSensor(atual->s);
        atual = atual->prox;
    }
}

void listarAnomalos() {
    printf("\n--- Leituras anomalas ---\n");
    No *atual = lista;
    int found = 0;
    while (atual != NULL) {
        if (eAnomalo(atual->s.estado)) {
            imprimirSensor(atual->s);
            found++;
        }
        atual = atual->prox;
    }
    if (found == 0) printf("Nenhuma leitura anomala.\n");
}

void pesquisarCodigo(char cod[]) {
    printf("\n--- Pesquisa por %s ---\n", cod);
    No *atual = lista;
    int found = 0;
    while (atual != NULL) {
        if (strcmp(atual->s.codigo, cod) == 0) {
            imprimirSensor(atual->s);
            found++;
        }
        atual = atual->prox;
    }
    if (found == 0) printf("Sensor nao encontrado.\n");
}

void listarRecentes(int n) {
    printf("\n--- %d leituras mais recentes ---\n", n);
    No *atual = lista;
    int i = 0;
    while (atual != NULL && i < n) {
        imprimirSensor(atual->s);
        i++;
        atual = atual->prox;
    }
}

void guardarBinario() {
    FILE *f = fopen("leituras_sensores.dat", "wb");
    if (f == NULL) {
        printf("Erro ao guardar ficheiro binario!\n");
        return;
    }
    fwrite(&totalSensores, sizeof(int), 1, f);
    No *atual = lista;
    while (atual != NULL) {
        fwrite(&atual->s, sizeof(Sensor), 1, f);
        atual = atual->prox;
    }
    fclose(f);
    printf("Sensores guardados com sucesso!\n");
}

void carregarBinario() {
    FILE *f = fopen("leituras_sensores.dat", "rb");
    if (f == NULL) {
        printf("Ficheiro leituras_sensores.dat nao encontrado.\n");
        return;
    }
    int total;
    fread(&total, sizeof(int), 1, f);
    Sensor *buf = malloc(total * sizeof(Sensor));
    fread(buf, sizeof(Sensor), total, f);
    // inserir ao contrario para manter a ordem original
    for (int i = total - 1; i >= 0; i--) {
        adicionarSensor(buf[i]);
    }
    free(buf);
    fclose(f);
    printf("Carregados %d sensores.\n", total);
}

void menuModulo3() {
    int op;
    do {
        printf("\n=== MODULO 3 - Sensores ===\n");
        printf("1 - Importar sensores_rack.txt\n");
        printf("2 - Listar todos\n");
        printf("3 - Listar N mais recentes\n");
        printf("4 - Pesquisar por codigo\n");
        printf("5 - Ver leituras anomalas\n");
        printf("6 - Guardar em ficheiro binario\n");
        printf("7 - Carregar ficheiro binario\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1: importarSensores(); break;
            case 2: listarTodos(); break;
            case 3: {
                int n;
                printf("Quantas leituras? ");
                scanf("%d", &n);
                getchar();
                listarRecentes(n);
                break;
            }
            case 4: {
                char cod[30];
                printf("Codigo do sensor: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = 0;
                pesquisarCodigo(cod);
                break;
            }
            case 5: listarAnomalos(); break;
            case 6: guardarBinario(); break;
            case 7: carregarBinario(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while(op != 0);
}
