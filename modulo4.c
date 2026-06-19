#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    PENDENTE,
    EM_CURSO,
    CONCLUIDO
} EstadoIncidente;

typedef struct {
    int numero;
    char referencia[30];
    char tipo[20];
    char descricao[120];
    int prioridade;
    char tecnico[40];
    EstadoIncidente estado;
    time_t criadoEm;
    time_t concluidoEm;
} Incidente;

typedef struct NoInc {
    Incidente inc;
    struct NoInc *prox;
} NoInc;

typedef struct NoFila {
    Incidente inc;
    struct NoFila *prox;
} NoFila;


// variaveis globais
NoInc *listaInc = NULL;
int totalInc = 0;
int proximoNumero = 1;

NoFila *frente = NULL;
NoFila *traseira = NULL;
int totalFila = 0;

char* prioridadeStr(int p) {
    if (p == 1) return "Baixa";
    if (p == 2) return "Media";
    if (p == 3) return "Alta";
    if (p == 4) return "Critica";
    return "?";
}

char* estadoStr(int e) {
    if (e == PENDENTE)  return "Pendente";
    if (e == EM_CURSO)  return "Em Curso";
    if (e == CONCLUIDO) return "Concluido";
    return "?";
}

void imprimirIncidente(Incidente inc) {
    char buf[30];
    struct tm *t = localtime(&inc.criadoEm);
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", t);

    printf("  Numero: %d\n", inc.numero);
    printf("  Referencia: %s\n", inc.referencia);
    printf("  Tipo: %s\n", inc.tipo);
    printf("  Descricao: %s\n", inc.descricao);
    printf("  Prioridade: %s\n", prioridadeStr(inc.prioridade));
    printf("  Tecnico: %s\n", inc.tecnico);
    printf("  Estado: %s\n", estadoStr(inc.estado));
    printf("  Criado em: %s\n", buf);
    if (inc.concluidoEm != 0) {
        struct tm *t2 = localtime(&inc.concluidoEm);
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", t2);
        printf("  Concluido em: %s\n", buf);
    }
    printf("  -----------------\n");
}


void adicionarLista(Incidente inc) {
    NoInc *novo = (NoInc*)malloc(sizeof(NoInc));
    novo->inc = inc;
    novo->prox = NULL;
    if (listaInc == NULL) {
        listaInc = novo;
    } else {
        NoInc *atual = listaInc;
        while (atual->prox != NULL) atual = atual->prox;
        atual->prox = novo;
    }
    totalInc++;
}

void enqueue(Incidente inc) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    novo->inc = inc;
    novo->prox = NULL;
    if (traseira == NULL) {
        frente = novo;
        traseira = novo;
    } else {
        traseira->prox = novo;
        traseira = novo;
    }
    totalFila++;
}

void processarProximo() {
    if (frente == NULL) {
        printf("A fila esta vazia!\n");
        return;
    }
    NoFila *no = frente;
    frente = frente->prox;
    if (frente == NULL) traseira = NULL;
    totalFila--;

    // atualiza estado na lista principal
    NoInc *atual = listaInc;
    while (atual != NULL) {
        if (atual->inc.numero == no->inc.numero) {
            atual->inc.estado = EM_CURSO;
            break;
        }
        atual = atual->prox;
    }
    printf("Incidente #%d esta agora Em Curso.\n", no->inc.numero);
    free(no);
}

// ---- criar incidentes ----

void criarIncidente(char ref[], char tipo[], char desc[], int prio, char tec[]) {
    Incidente inc;
    inc.numero = proximoNumero++;
    strcpy(inc.referencia, ref);
    strcpy(inc.tipo, tipo);
    strcpy(inc.descricao, desc);
    inc.prioridade = prio;
    strcpy(inc.tecnico, tec);
    inc.estado = PENDENTE;
    inc.criadoEm = time(NULL);
    inc.concluidoEm = 0;

    adicionarLista(inc);
    enqueue(inc);
}
 //esse pelo módulo 3
void criarIncidenteSensor(char codSensor[], char estadoSensor[]) {
    char desc[120];
    sprintf(desc, "Leitura anomala no sensor %s com estado %s",
            codSensor, estadoSensor);

    int prio = 3;
    if (strcmp(estadoSensor, "CRITICO") == 0 ||
        strcmp(estadoSensor, "FALHA_REDE") == 0)
        prio = 4;

    criarIncidente(codSensor, "Sensor", desc, prio, "Sistema");
    printf("Incidente #%d criado automaticamente para %s\n",
           proximoNumero - 1, codSensor);
}

// esse vai ser chamado de uma forma eu acho que pelo módulo 2 do alex
void criarIncidentePing(char codEquip[], char ip[]) {
    char desc[120];
    sprintf(desc, "Equipamento %s nao respondeu ao ping (%s)", codEquip, ip);
    criarIncidente(codEquip, "Ping", desc, 3, "Sistema");
    printf("Incidente #%d criado (ping falhou em %s)\n",
           proximoNumero - 1, ip);
}

void criarIncidenteManual() {
    char ref[30], tipo[20], desc[120], tec[40];
    int prio;

    printf("Referencia (equipamento/sensor): ");
    fgets(ref, sizeof(ref), stdin);
    ref[strcspn(ref, "\n")] = 0;

    printf("Tipo (Manual/Ping/Sensor): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = 0;

    printf("Descricao: ");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = 0;

    printf("Prioridade (1=Baixa 2=Media 3=Alta 4=Critica): ");
    scanf("%d", &prio);
    getchar();

    printf("Tecnico responsavel: ");
    fgets(tec, sizeof(tec), stdin);
    tec[strcspn(tec, "\n")] = 0;

    criarIncidente(ref, tipo, desc, prio, tec);
    printf("Incidente #%d criado!\n", proximoNumero - 1);
}

void concluirIncidente(int num) {
    NoInc *atual = listaInc;
    while (atual != NULL) {
        if (atual->inc.numero == num) {
            if (atual->inc.estado == CONCLUIDO) {
                printf("Este incidente ja esta concluido.\n");
                return;
            }
            atual->inc.estado = CONCLUIDO;
            atual->inc.concluidoEm = time(NULL);
            printf("Incidente #%d concluido!\n", num);
            return;
        }
        atual = atual->prox;
    }
    printf("Incidente nao encontrado.\n");
}

//listagen

void listarIncidentesPorEstado(int estado) {
    NoInc *atual = listaInc;
    int count = 0;
    while (atual != NULL) {
        if (atual->inc.estado == estado) {
            imprimirIncidente(atual->inc);
            count++;
        }
        atual = atual->prox;
    }
    if (count == 0)
        printf("Nenhum incidente %s.\n", estadoStr(estado));
}

void listarTodosInc() {
    if (listaInc == NULL) {
        printf("Nao ha incidentes.\n");
        return;
    }
    printf("\n--- Todos os incidentes (%d) ---\n", totalInc);
    NoInc *atual = listaInc;
    while (atual != NULL) {
        imprimirIncidente(atual->inc);
        atual = atual->prox;
    }
}

void listarPorReferencia(char ref[]) {
    printf("\n--- Incidentes de '%s' ---\n", ref);
    NoInc *atual = listaInc;
    int count = 0;
    while (atual != NULL) {
        if (strcmp(atual->inc.referencia, ref) == 0) {
            imprimirIncidente(atual->inc);
            count++;
        }
        atual = atual->prox;
    }
    if (count == 0) printf("Nenhum incidente para '%s'.\n", ref);
}

void listarPorPrioridade(int prio) {
    printf("\n--- Incidentes com prioridade %s ---\n", prioridadeStr(prio));
    NoInc *atual = listaInc;
    int count = 0;
    while (atual != NULL) {
        if (atual->inc.prioridade == prio) {
            imprimirIncidente(atual->inc);
            count++;
        }
        atual = atual->prox;
    }
    if (count == 0) printf("Nenhum com essa prioridade.\n");
}

void mostrarFila() {
    if (frente == NULL) {
        printf("A fila esta vazia.\n");
        return;
    }
    printf("\n--- Fila de atendimento (%d) ---\n", totalFila);
    NoFila *atual = frente;
    int i = 1;
    while (atual != NULL) {
        printf("  [%d] #%d - %s - Prioridade: %s\n",
               i++, atual->inc.numero,
               atual->inc.referencia,
               prioridadeStr(atual->inc.prioridade));
        atual = atual->prox;
    }
}

//ficheiro binario 

void guardarIncidentes() {
    FILE *f = fopen("incidentes.dat", "wb");
    if (f == NULL) {
        printf("Erro ao guardar incidentes!\n");
        return;
    }
    fwrite(&totalInc, sizeof(int), 1, f);
    fwrite(&proximoNumero, sizeof(int), 1, f);
    NoInc *atual = listaInc;
    while (atual != NULL) {
        fwrite(&atual->inc, sizeof(Incidente), 1, f);
        atual = atual->prox;
    }
    fclose(f);
    printf("Incidentes guardados!\n");
}

void carregarIncidentes() {
    FILE *f = fopen("incidentes.dat", "rb");
    if (f == NULL) {
        printf("Ficheiro incidentes.dat nao encontrado.\n");
        return;
    }
    int total;
    fread(&total, sizeof(int), 1, f);
    fread(&proximoNumero, sizeof(int), 1, f);
    Incidente inc;
    int count = 0;
    while (fread(&inc, sizeof(Incidente), 1, f) == 1) {
        adicionarLista(inc);
        if (inc.estado == PENDENTE) enqueue(inc);
        count++;
    }
    fclose(f);
    printf("Carregados %d incidentes.\n", count);
}

// menu

void menuModulo4() {
    int op;
    do {
        printf("\n=== MODULO 4 - Incidentes ===\n");
        printf("1 - Criar incidente manual\n");
        printf("2 - Ver fila de atendimento\n");
        printf("3 - Processar proximo da fila\n");
        printf("4 - Concluir incidente\n");
        printf("5 - Listar pendentes\n");
        printf("6 - Listar em curso\n");
        printf("7 - Listar concluidos\n");
        printf("8 - Listar todos\n");
        printf("9 - Pesquisar por equipamento/sensor\n");
        printf("10 - Listar por prioridade\n");
        printf("11 - Guardar em ficheiro binario\n");
        printf("12 - Carregar ficheiro binario\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1:  criarIncidenteManual(); break;
            case 2:  mostrarFila(); break;
            case 3:  processarProximo(); break;
            case 4: {
                int num;
                printf("Numero do incidente: ");
                scanf("%d", &num);
                getchar();
                concluirIncidente(num);
                break;
            }
            case 5:
                printf("\n--- Incidentes Pendentes ---\n");
                listarPorEstado(PENDENTE);
                break;
            case 6:
                printf("\n--- Incidentes Em Curso ---\n");
                listarPorEstado(EM_CURSO);
                break;
            case 7:
                printf("\n--- Incidentes Concluidos ---\n");
                listarPorEstado(CONCLUIDO);
                break;
            case 8:  listarTodosInc(); break;
            case 9: {
                char ref[30];
                printf("Referencia: ");
                fgets(ref, sizeof(ref), stdin);
                ref[strcspn(ref, "\n")] = 0;
                listarPorReferencia(ref);
                break;
            }
            case 10: {
                int p;
                printf("Prioridade (1-4): ");
                scanf("%d", &p);
                getchar();
                listarPorPrioridade(p);
                break;
            }
            case 11: guardarIncidentes(); break;
            case 12: carregarIncidentes(); break;
            case 0:  break;
            default: printf("Opcao invalida!\n");
        }
    } while(op != 0);
}
