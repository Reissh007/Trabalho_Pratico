#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#define MAXCHAR 256
#define NOME_FICHEIRO "registros_equipamentos.dat" //nome do ficheiro em que estao guardados os dados

#define PENDENTE  0
#define EM_CURSO  1
#define CONCLUIDO 2

typedef struct dataverificacao {
    int dia, mes, ano;
} dataverificacao;

typedef struct EQUIPAMENTOS {
    int CIU;
    char *nome, *tipo, *marca, *modelo, *IP, *mac, *localizacao, *estado;
    dataverificacao Data;
    struct EQUIPAMENTOS *ant, *prox;
} equipamento;

//  Usavel /util


char *duplicarString(const char *origem) { //devolve copia 
    char *copia = (char *)malloc(strlen(origem) + 1);
    if (copia) strcpy(copia, origem);
    return copia;
}

void lerTexto(const char *pergunta, char *destino, int tamanho) {
    printf("%s", pergunta);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

int anoBissexto(int ano) { //
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int dataValida(int dia, int mes, int ano) {
    int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (ano < 1980 || ano > 2100 || mes < 1 || mes > 12) return 0;
    int max = dias[mes - 1];
    if (mes == 2 && anoBissexto(ano)) max = 29;
    return dia >= 1 && dia <= max;
}

void lerData(int *dia, int *mes, int *ano) {
    do {
        printf(" Dia: "); scanf("%d", dia);
        printf(" Mes: "); scanf("%d", mes);
        printf(" Ano: "); scanf("%d", ano);
        getchar();
        if (!dataValida(*dia, *mes, *ano)) printf("Data invalida. Tente novamente.\n");
    } while (!dataValida(*dia, *mes, *ano));
}

const char *nomeEstado(int op) {
    switch (op) {
        case 1: return "Operacional";
        case 2: return "Em Falha";
        case 3: return "Em Manutencao";
        case 4: return "Desativado";
        default: return NULL;
    }
}

char *perguntarEstado(void) {
    int op;
    const char *estado;
    do {
        printf("\n 1-Operacional  2-Em Falha  3-Em Manutencao  4-Desativado\n");
        printf("Estado: "); scanf("%d", &op); getchar();
        estado = nomeEstado(op);
        if (!estado) printf("Opcao invalida.\n");
    } while (!estado);
    return duplicarString(estado);
}

int igualSemCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

dataverificacao obterDataAtual(void) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    dataverificacao d = { t->tm_mday, t->tm_mon + 1, t->tm_year + 1900 }; /* CORRIGIDO: tm_year conta a partir de 1900, nao 1980 */
    return d;
}

void obterDataHoraStr(char *buf, int tam) {
    time_t agora = time(NULL);
    strftime(buf, tam, "%Y-%m-%d %H:%M:%S", localtime(&agora));
}

void registarLog(const char *linha) {
    FILE *f = fopen("log_monitorizacao.txt", "a");
    if (!f) return;
    fprintf(f, "%s\n", linha);
    fclose(f);
}

// Modulo 1 Equipamentos(menu principal)

equipamento *procurarPorCIU(equipamento *cabeca, int ciu) {
    while (cabeca) {
        if (cabeca->CIU == ciu) return cabeca;
        cabeca = cabeca->prox;
    }
    return NULL;
}

void inserirNoFim(equipamento **cabeca, equipamento *novo) {
    if (!*cabeca) { *cabeca = novo; return; }
    equipamento *a = *cabeca;
    while (a->prox) a = a->prox;
    a->prox = novo;
    novo->ant = a;
}

equipamento *criarEquipamento(int *proxCIU) {
    equipamento *novo = (equipamento *)malloc(sizeof(equipamento));
    char temp[MAXCHAR];
    novo->CIU = (*proxCIU)++;
    printf("\n[CIU: %d]\n", novo->CIU);
    lerTexto("Nome: ",        temp, MAXCHAR); 
	novo->nome       = duplicarString(temp);
    lerTexto("Tipo: ",        temp, MAXCHAR); 
	novo->tipo       = duplicarString(temp);
    lerTexto("Marca: ",       temp, MAXCHAR); 
	novo->marca      = duplicarString(temp);
    lerTexto("Modelo: ",      temp, MAXCHAR); 
	novo->modelo     = duplicarString(temp);
    lerTexto("MAC: ",         temp, MAXCHAR); 
	novo->mac        = duplicarString(temp);
    lerTexto("IP: ",          temp, MAXCHAR); 
	novo->IP         = duplicarString(temp);
    lerTexto("Localizacao: ", temp, MAXCHAR); 
	novo->localizacao= duplicarString(temp);
    printf("Data da ultima verificacao:\n");
    lerData(&novo->Data.dia, &novo->Data.mes, &novo->Data.ano);
    novo->estado = perguntarEstado();
    novo->ant = novo->prox = NULL;
    return novo;
}

void Adicionar(equipamento **cabeca, int *proxCIU) {
    int resp;
    do {
        inserirNoFim(cabeca, criarEquipamento(proxCIU));
        do {
            printf("Adicionar mais? (1-Sim / 0-Nao): ");
            scanf("%d", &resp); getchar();
        } while (resp != 0 && resp != 1);
    } while (resp == 1);
}

void imprimirEquipamento(equipamento *e) {
    printf("\nCIU: %d | %s | %s | IP: %s | MAC: %s", e->CIU, e->nome, e->tipo, e->IP, e->mac);
    printf("\nMarca: %s | Modelo: %s | Local: %s", e->marca, e->modelo, e->localizacao);
    printf("\nData: %02d/%02d/%04d | Estado: %s\n", e->Data.dia, e->Data.mes, e->Data.ano, e->estado);
    printf("---------------------------------------------\n");
}

void listar(equipamento *cabeca) {
    if (!cabeca) { printf("\nSem equipamentos registados.\n"); return; }
    printf("\n=== Equipamentos ===\n");
    while (cabeca) { imprimirEquipamento(cabeca); 
	cabeca = cabeca->prox; }
}

void listarPorTipo(equipamento *cabeca) {
    if (!cabeca) { printf("\nSem equipamentos registados.\n"); return; }
    char tipo[MAXCHAR];
    lerTexto("Tipo a pesquisar: ", tipo, MAXCHAR);
    int n = 0;
    while (cabeca) {
        if (igualSemCase(cabeca->tipo, tipo)) { imprimirEquipamento(cabeca); n++; }
        cabeca = cabeca->prox;
    }
    if (!n) printf("Nenhum resultado para '%s'.\n", tipo);
}

void listarPorEstado(equipamento *cabeca) {
    if (!cabeca) { printf("\nSem equipamentos registados.\n"); return; }
    int op;
    const char *estado;
    do {
        printf("\n 1-Operacional \n 2-Em Falha \n 3-Em Manutencao \n 4-Desativado\n");
        printf("Estado: "); scanf("%d", &op); getchar();
        estado = nomeEstado(op);
        if (!estado) printf("Opcao invalida.\n");
    } while (!estado);
    int n = 0;
    while (cabeca) {
        if (igualSemCase(cabeca->estado, estado)) { imprimirEquipamento(cabeca); n++; }
        cabeca = cabeca->prox;
    }
    if (!n) printf("Nenhum equipamento com estado '%s'.\n", estado);
}

void pesquisarEquipamento(equipamento *cabeca) {
    if (!cabeca) { printf("\nSem equipamentos registados.\n"); return; }
    int op;
    printf("\n 1-CIU  2-IP  3-MAC\n");
    printf("Pesquisar por: "); scanf("%d", &op); getchar();
    if (op == 1) {
        int ciu; printf("CIU: "); scanf("%d", &ciu); getchar();
        equipamento *e = procurarPorCIU(cabeca, ciu);
        if (e) imprimirEquipamento(e); else printf("CIU %d nao encontrado.\n", ciu);
        return;
    }
    char termo[MAXCHAR];
    if      (op == 2) {lerTexto("IP: ",  termo, MAXCHAR);}
    else if (op == 3) {	lerTexto("MAC: ", termo, MAXCHAR);}
    else { printf("Opcao invalida.\n"); return; }
    int n = 0;
    equipamento *a = cabeca;
    while (a) {
        if ((op == 2 && igualSemCase(a->IP, termo)) || (op == 3 && igualSemCase(a->mac, termo)))
            { imprimirEquipamento(a); n++; }
        a = a->prox;
    }
    if (!n) printf("Nenhum resultado.\n");
}

void removerEquipamento(equipamento **cabeca) {
    int ciu, conf;
    printf("CIU a remover: ");
	scanf("%d", &ciu); 
	getchar();
    equipamento *alvo = procurarPorCIU(*cabeca, ciu);
    if (!alvo) { printf("CIU %d nao encontrado.\n", ciu); return; }
    printf("Remover '%s'? (1-Sim / 0-Nao): ", alvo->nome);
    scanf("%d", &conf); getchar();
    if (conf != 1) { printf("Cancelado.\n"); return; }
    if (alvo->ant)  {alvo->ant->prox = alvo->prox;}else *cabeca = alvo->prox;
    if (alvo->prox) {alvo->prox->ant = alvo->ant;}
    free(alvo->nome); free(alvo->tipo); free(alvo->marca); free(alvo->modelo);
    free(alvo->IP);   free(alvo->mac);  free(alvo->localizacao); free(alvo->estado);
    free(alvo);
    printf("Equipamento %d removido.\n", ciu);
}

void alterarEquipamento(equipamento *cabeca) {
    int ciu; printf("CIU a alterar: "); scanf("%d", &ciu); getchar();
    equipamento *e = procurarPorCIU(cabeca, ciu);
    if (!e) { printf("CIU %d nao encontrado.\n", ciu); return; }
    int op; char temp[MAXCHAR];
    do {
        printf("\n[CIU %d — %s]\n", e->CIU, e->nome);
        printf(" 1-Nome  2-Tipo  3-Marca  4-Modelo  5-IP  6-MAC  7-Localizacao  8-Estado  9-Data  0-Sair\n");
        printf("Opcao: "); scanf("%d", &op); getchar();
        switch (op) {
            case 1: lerTexto("Nome: ", temp, MAXCHAR); free(e->nome);     
			  e->nome = duplicarString(temp);
			   break;
            case 2: lerTexto("Tipo: ", temp, MAXCHAR); free(e->tipo);       
			e->tipo = duplicarString(temp); 
			break;
            case 3: lerTexto("Marca: ",temp, MAXCHAR); free(e->marca);     
			 e->marca = duplicarString(temp);
			  break;
            case 4: lerTexto("Modelo: ",temp, MAXCHAR); free(e->modelo);     
			e->modelo = duplicarString(temp); 
			break;
            case 5: lerTexto("IP: ",temp, MAXCHAR); free(e->IP);        
			e->IP = duplicarString(temp); 
			break;
            case 6: lerTexto("MAC: ", temp, MAXCHAR); free(e->mac);     
			   e->mac= duplicarString(temp);
			    break;
            case 7: lerTexto("Localizacao: ", temp, MAXCHAR); 
			free(e->localizacao);
			e->localizacao= duplicarString(temp);
			 break;
            case 8: free(e->estado); 
			e->estado = perguntarEstado(); 
			break;
            case 9: printf("Nova data:\n"); 
			lerData(&e->Data.dia, &e->Data.mes, &e->Data.ano); 
			break;
            case 0: 
			break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

int contarEquipamentos(equipamento *cabeca) {
    int n = 0; while (cabeca) { n++; cabeca = cabeca->prox; } return n;
    
}



void liberarLista(equipamento *cabeca) {
    while (cabeca) {
        equipamento *prox = cabeca->prox;
        free(cabeca->nome); free(cabeca->tipo); free(cabeca->marca); free(cabeca->modelo);
        free(cabeca->IP);   free(cabeca->mac);  free(cabeca->localizacao); free(cabeca->estado);
        free(cabeca);
        cabeca = prox;
    }
}

// funcoes - Ficheiros 

int obterUltimoCIULista(equipamento *cabeca) {
    if (!cabeca) return 0;
    while (cabeca->prox) cabeca = cabeca->prox;
    return cabeca->CIU;
}

void escreverString(char *str, FILE *fb) {
    int len = strlen(str);
    fwrite(&len, sizeof(int), 1, fb);
    fwrite(str, sizeof(char), len, fb);
}

char *lerString(FILE *fb) {
    int len; fread(&len, sizeof(int), 1, fb);
    char *str = (char *)malloc(len + 1);
    fread(str, sizeof(char), len, fb);
    str[len] = '\0';
    return str;
}

int obterProximoCIU(void) {
    FILE *fb = fopen(NOME_FICHEIRO, "rb");
    if (!fb) return 1;
    int ciu;
    if (fread(&ciu, sizeof(int), 1, fb) != 1) { fclose(fb); return 1; }
    fclose(fb);
    return ciu + 1;
}

void carregar(equipamento **cabeca) {
    FILE *fb = fopen(NOME_FICHEIRO, "rb");
    if (!fb) return;
    int cab;
    if (fread(&cab, sizeof(int), 1, fb) != 1) { fclose(fb); return; }
    int ciu;
    while (fread(&ciu, sizeof(int), 1, fb) == 1) {
        equipamento *novo = (equipamento *)malloc(sizeof(equipamento));
        novo->CIU         = ciu;
        novo->nome        = lerString(fb);
        novo->tipo        = lerString(fb);
        novo->marca       = lerString(fb);
        novo->modelo      = lerString(fb);
        novo->IP          = lerString(fb);
        novo->mac         = lerString(fb);
        novo->localizacao = lerString(fb);
        novo->estado      = lerString(fb);
        fread(&novo->Data, sizeof(dataverificacao), 1, fb);
        novo->ant = novo->prox = NULL;
        inserirNoFim(cabeca, novo);
    }
    fclose(fb);
    printf("%d equipamento(s) carregado(s).\n", contarEquipamentos(*cabeca));
}

void guardar(equipamento *cabeca) {
    FILE *fb = fopen(NOME_FICHEIRO, "wb");
    if (!fb) { printf("Erro ao guardar.\n"); return; }
    int ciu = obterUltimoCIULista(cabeca);
    fwrite(&ciu, sizeof(int), 1, fb);
    equipamento *a = cabeca;
    while (a) {
        fwrite(&a->CIU, sizeof(int), 1, fb);
        escreverString(a->nome, fb);        escreverString(a->tipo, fb);
        escreverString(a->marca, fb);       escreverString(a->modelo, fb);
        escreverString(a->IP, fb);          escreverString(a->mac, fb);
        escreverString(a->localizacao, fb); escreverString(a->estado, fb);
        fwrite(&a->Data, sizeof(dataverificacao), 1, fb);
        a = a->prox;
    }
    fclose(fb);
    printf("Dados guardados.\n");
}

// Menu dos Equipamentos Modulo 1

/* CORRIGIDO: parametros tinham de ser ponteiro-para-ponteiro (equipamento**) e
   ponteiro para int (int*) para poder alterar a lista e o proxCIU em main();
   o corpo da funcao referia "dispositivos"/"&proxCIU" que nao existiam neste
   ambito (so existiam em main). Agora usa os proprios parametros. */
void menuequipamentos(equipamento **dispositivos, int *proxCIU){
	int op;
    printf("Proximo CIU: %d\n", *proxCIU);
	 do {
        printf("\n=== Equipamentos ===\n");
        printf(" 1 - Adicionar equipamento\n");
        printf(" 2 - Remover equipamento\n");
        printf(" 3 - Alterar equipamento\n");
        printf(" 4 - Listar todos\n");
        printf(" 5 - Listar por tipo\n");
        printf(" 6 - Listar por estado\n");
        printf(" 7 - Pesquisar\n");
        printf(" 8 - Guardar e sair\n");
        printf("Opcao: "); scanf("%d", &op); getchar();
        switch (op) {
            case 1: Adicionar(dispositivos, proxCIU);     break;
            case 2: removerEquipamento(dispositivos);     break;
            case 3: alterarEquipamento(*dispositivos);    break;
            case 4: listar(*dispositivos);                break;
            case 5: listarPorTipo(*dispositivos);         break;
            case 6: listarPorEstado(*dispositivos);       break;
            case 7: pesquisarEquipamento(*dispositivos);  break;
            case 8: guardar(*dispositivos);               break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 8);

}

// Modulo 2 (teste de ping)

int executarPing(const char *ip, int so) {
    char cmd[MAXCHAR * 2];
    if (so == 1) snprintf(cmd, sizeof(cmd), "ping -n 4 %s > resultado_ping.txt 2>&1", ip);
    else         snprintf(cmd, sizeof(cmd), "ping -c 4 %s > resultado_ping.txt 2>&1", ip);
    printf("A executar: %s\n", cmd);
    return system(cmd);
}

int analisarResultadoPing(int so) {
    FILE *f = fopen("resultado_ping.txt", "r");
    if (!f) { printf("Erro ao abrir resultado_ping.txt\n"); return 0; }
    char linha[MAXCHAR];
    int respondeu = 0;
    while (fgets(linha, sizeof(linha), f)) {
        if (so == 1) {
            if (strstr(linha, "Received = ")) {
                int n = 0;
                sscanf(strstr(linha, "Received = ") + strlen("Received = "), "%d", &n);
                if (n > 0) { respondeu = 1; break; }
            }
        } else {
            if (strstr(linha, " received")) {
                int n = 0; sscanf(linha, "%d", &n);
                if (n > 0) { respondeu = 1; break; }
            }
            if (strstr(linha, "0% packet loss")) { respondeu = 1; break; }
        }
    }
    fclose(f);
    return respondeu;
}

void testarConectividade(equipamento *cabeca, int ciu, int so) {
    equipamento *e = procurarPorCIU(cabeca, ciu);
    if (!e) { printf("CIU %d nao encontrado.\n", ciu); return; }

    printf("\nA testar %s (IP: %s)...\n", e->nome, e->IP);
    executarPing(e->IP, so);
    int ok = analisarResultadoPing(so); /* CORRIGIDO: estava "int ok = (so);", o que ignorava
                                            o resultado real do ping e usava apenas a flag do SO */

    e->Data = obterDataAtual();

    if (ok) {
        printf("[OK] %s respondeu.\n", e->nome);
    } else {
        printf("[FALHA] %s nao respondeu. Estado -> 'Em Falha'.\n", e->nome);
        free(e->estado);
        e->estado = duplicarString("Em Falha");
        // criarIncidenteAutomatico(e->CIU, "Sem resposta ao ping");  /* Modulo 4 */
    }

    char datahora[MAXCHAR], entrada[MAXCHAR * 3];
    obterDataHoraStr(datahora, sizeof(datahora));
    snprintf(entrada, sizeof(entrada), "[%s] CIU:%d | %s | IP:%s | %s",
             datahora, e->CIU, e->nome, e->IP, ok ? "OK" : "FALHA");
    registarLog(entrada);
}

void testarRedeCompleta(equipamento *cabeca, int so) {
    if (!cabeca) { printf("Sem equipamentos registados.\n"); return; }
    char datahora[MAXCHAR];
    obterDataHoraStr(datahora, sizeof(datahora));
    char cab[MAXCHAR * 2];
    snprintf(cab, sizeof(cab), "\n=== TESTE GERAL — %s ===", datahora);
    registarLog(cab);
    int total = 0, ok = 0, falha = 0;
    equipamento *a = cabeca;
    while (a) {
        printf("\n[CIU %d] %s (%s)\n", a->CIU, a->nome, a->IP);
        executarPing(a->IP, so);
        int res = analisarResultadoPing(so);
        a->Data = obterDataAtual();
        char entrada[MAXCHAR * 3];
        if (res) {
            printf("  OK\n"); ok++;
            snprintf(entrada, sizeof(entrada), "  CIU:%d | %s | OK", a->CIU, a->nome);
        } else {
            printf("  FALHA\n"); falha++;
            free(a->estado); a->estado = duplicarString("Em Falha");
            // criarIncidenteAutomatico(a->CIU, "Falha no teste geral");  /* Modulo 4 */
            snprintf(entrada, sizeof(entrada), "  CIU:%d | %s | FALHA", a->CIU, a->nome);
        }
        registarLog(entrada);
        total++; a = a->prox;
    }
    printf("\nTotal: %d | OK: %d | Falha: %d\n", total, ok, falha);
    char resumo[MAXCHAR];
    snprintf(resumo, sizeof(resumo), "  Resumo: total=%d ok=%d falha=%d", total, ok, falha);
    registarLog(resumo);
}

void verLog(void) {
    FILE *f = fopen("log_monitorizacao.txt", "r");
    if (!f) { printf("Log vazio ou inexistente.\n"); return; }
    char linha[MAXCHAR * 4];
    printf("\n=== log_monitorizacao.txt ===\n");
    while (fgets(linha, sizeof(linha), f)) printf("%s", linha);
    fclose(f);
}
// MENU THE CONECTIVIDADE
void menuConectividade(equipamento *cabeca, int so) {
    int op, ciu;
    do {
        printf("\n-- Conectividade --\n");
        printf(" 1 - Ping a um equipamento\n");
        printf(" 2 - Ping a todos\n");
        printf(" 3 - Ver log\n");
        printf(" 0 - Voltar\n");
        printf("Opcao: "); scanf("%d", &op); getchar();
        switch (op) {
            case 1:
                if (!cabeca) { printf("Sem equipamentos.\n"); break; }
                listar(cabeca);
                printf("CIU: "); scanf("%d", &ciu); getchar();
                testarConectividade(cabeca, ciu, so);
                break;
            case 2: testarRedeCompleta(cabeca, so); break;
            case 3: verLog(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}



// -- Modulo 3 --
void importarSensores();
void listarTodos();
void listarAnomalos();
void pesquisarCodigo(char cod[]);
void listarRecentes(int n);
void guardarBinario();
void carregarBinario();

// -- Modulo 4 --
void criarIncidenteManual();
void mostrarFila();
void processarProximo();
void concluirIncidente(int num);
void listarIncidentesPorEstado(int estado); /* CORRIGIDO: renomeada de "listarPorEstado" para nao
                                                colidir com a funcao homonima de equipamentos
                                                (assinaturas incompativeis = erro de compilacao).
                                                E necessario aplicar a mesma renomeacao em modulo4.c */
void listarTodosInc();
void listarPorReferencia(char ref[]);
void listarPorPrioridade(int prio);
void guardarIncidentes();
void carregarIncidentes();

// -- Modulo 5 --
void registarConfigManual();
void verUltimaConfig();
void verNConfigs(int n);
void reverterUltima();
void historicoEquipamento(char cod[]);
void guardarConfigs();
void carregarConfigs();
void limparConfigs();

// Estados do modulo 4 (enum replicado para o menu)



// MENU MODULO 3 - Sensores

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

        switch (op) {
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
    } while (op != 0);
}


// MENU MODULO 4 - Incidentes

void menuModulo4() {
    int op;
    do {
        printf("\n=== MODULO 4 - Incidentes ===\n");
        printf("1  - Criar incidente manual\n");
        printf("2  - Ver fila de atendimento\n");
        printf("3  - Processar proximo da fila\n");
        printf("4  - Concluir incidente\n");
        printf("5  - Listar pendentes\n");
        printf("6  - Listar em curso\n");
        printf("7  - Listar concluidos\n");
        printf("8  - Listar todos\n");
        printf("9  - Pesquisar por equipamento/sensor\n");
        printf("10 - Listar por prioridade\n");
        printf("11 - Guardar em ficheiro binario\n");
        printf("12 - Carregar ficheiro binario\n");
        printf("0  - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch (op) {
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
                listarIncidentesPorEstado(PENDENTE);
                break;
            case 6:
                printf("\n--- Incidentes Em Curso ---\n");
                listarIncidentesPorEstado(EM_CURSO);
                break;
            case 7:
                printf("\n--- Incidentes Concluidos ---\n");
                listarIncidentesPorEstado(CONCLUIDO);
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
    } while (op != 0);
}

// MENU MODULO 5 - Configuracoes
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

        switch (op) {
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
    } while (op != 0);
}





//  Main / Menu Principal

int main(void) {
    setlocale(LC_ALL, "Portuguese");
    equipamento *dispositivos = NULL;
    carregar(&dispositivos);
    int proxCIU = obterProximoCIU();
    int so;
    int op; 
    do {
        printf("Sistema operativo: (1-Windows / 0-Linux): ");
        scanf("%d", &so);
        getchar();
    } while (so != 0 && so != 1);
	do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf(" 1 - EQUIPAMENTOS 	/ MODULO 1\n");
        printf(" 2 - CONECTIVIDADE / MODULO 2\n");
        printf(" 3 - SENSORES 	   / MODULO 3\n");
        printf(" 4 - INCIDENTES   / MODULO 4\n");
        printf(" 5 - CONFIGURAÇÕES / MODULO 5\n");
        printf(" 6 - sair\n");
        printf("Opcao: "); scanf("%d", &op); getchar();
        switch (op) {
            case 1: menuequipamentos(&dispositivos, &proxCIU);  break; 
            case 2: menuConectividade(dispositivos, so);     	break;
            case 3: menuModulo3();							    break;
            case 4: menuModulo4();                           	break; 
            case 5: menuModulo5();          					break; 
            case 6:	printf("SAINDO...");								 	break; 
			default: printf("Opcao invalida.\n");
        }
    } while (op != 6);

    liberarLista(dispositivos);
    return 0;
}
