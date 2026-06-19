#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>

#define MAXCHAR 150
#define NOME_FICHEIRO "registros_equipamentos.dat"

typedef struct dataverificacao {
    int dia;
    int mes;
    int ano;
} dataverificacao;

// usa-se "struct EQUIPAMENTOS *" e nao "equipamento *" porque o typedef
// so fica valido depois de a struct estar completamente definida
typedef struct EQUIPAMENTOS {
    int CIU;
    char *nome;
    char *tipo;
    char *marca;
    char *modelo;
    char *IP;
    char *mac;
    char *localizacao;
    char *estado;
    dataverificacao Data;
    struct EQUIPAMENTOS *ant;
    struct EQUIPAMENTOS *prox;
} equipamento;

char *duplicarString(const char *origem) {
    char *copia = (char *)malloc(strlen(origem) + 1);
    if (copia != NULL) {
        strcpy(copia, origem);
    }
    return copia;
}

void lerTexto(const char *pergunta, char *destino, int tamanho) {
    printf("%s", pergunta);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

int anoBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

// valida dia/mes/ano, incluindo o numero de dias correto por mes e anos bissextos
int dataValida(int dia, int mes, int ano) {
    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (ano < 1900 || ano > 2100) {
        return 0;
    }
    if (mes < 1 || mes > 12) {
        return 0;
    }

    int maxDia = diasNoMes[mes - 1];
    if (mes == 2 && anoBissexto(ano)) {
        maxDia = 29;
    }

    return dia >= 1 && dia <= maxDia;
}

// pede dia/mes/ano em ciclo ate o utilizador introduzir uma data valida
void lerData(int *dia, int *mes, int *ano) {
    do {
        printf(" Dia: ");
        scanf("%d", dia);
        printf(" Mes: ");
        scanf("%d", mes);
        printf(" Ano: ");
        scanf("%d", ano);
        getchar();

        if (!dataValida(*dia, *mes, *ano)) {
            printf("\nData invalida! Tenta novamente.\n");
        }
    } while (!dataValida(*dia, *mes, *ano));
}

// devolve o nome do estado correspondente a uma opcao do menu (1 a 4),
// ou NULL se a opcao for invalida; usada em perguntarEstado() e em
// listarPorEstado() para nao repetir os 4 nomes em dois sitios diferentes
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
        printf("\nQual o estado do equipamento?\n");
        printf(" 1 - Operacional\n");
        printf(" 2 - Em Falha\n");
        printf(" 3 - Em Manutencao\n");
        printf(" 4 - Desativado\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        estado = nomeEstado(op);
        if (estado == NULL) {
            printf("\nValor invalido!\n");
        }
    } while (estado == NULL);
    return duplicarString(estado);
}

equipamento *criarEquipamento(int *proxCIU) {
    equipamento *novo = (equipamento *)malloc(sizeof(equipamento));
    char temp[MAXCHAR];
    novo->CIU = *proxCIU;
    (*proxCIU)++;
    printf("\n----------------------------------------------");
    printf("\nCodigo Interno Unico (CIU): %d\n", novo->CIU);
    lerTexto("Nome do equipamento: ", temp, MAXCHAR);
    novo->nome = duplicarString(temp);
    lerTexto("Tipo de equipamento: ", temp, MAXCHAR);
    novo->tipo = duplicarString(temp);
    lerTexto("Marca: ", temp, MAXCHAR);
    novo->marca = duplicarString(temp);
    lerTexto("Modelo: ", temp, MAXCHAR);
    novo->modelo = duplicarString(temp);
    lerTexto("Endereco MAC: ", temp, MAXCHAR);
    novo->mac = duplicarString(temp);
    lerTexto("Endereco IP: ", temp, MAXCHAR);
    novo->IP = duplicarString(temp);
    printf("Data da ultima verificacao:\n");
    lerData(&novo->Data.dia, &novo->Data.mes, &novo->Data.ano);
    lerTexto("Localizacao do dispositivo: ", temp, MAXCHAR);
    novo->localizacao = duplicarString(temp);
    novo->estado = perguntarEstado();
    novo->ant = NULL;
    novo->prox = NULL;
    return novo;
}

void inserirNoFim(equipamento **cabeca, equipamento *novo) {
    if (*cabeca == NULL) {
        *cabeca = novo;
        return;
    }
    equipamento *atual = *cabeca;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }
    atual->prox = novo;
    novo->ant = atual;
}

void Adicionar(equipamento **cabeca, int *proxCIU) {
    int resp;
    do {
        equipamento *novo = criarEquipamento(proxCIU);
        inserirNoFim(cabeca, novo);
        do {
            printf("\nDeseja introduzir mais equipamentos? (Sim - 1 ; Nao - 0): ");
            scanf("%d", &resp);
            getchar();
            if (resp != 0 && resp != 1) {
                printf("\nResposta invalida!\n");
            }
        } while (resp != 0 && resp != 1);
    } while (resp == 1);
    printf("\nInsercao terminada.\n");
}

void imprimirEquipamento(equipamento *e) {
    printf("\nCodigo Interno Unico: %d", e->CIU);
    printf("\nNome do Equipamento: %s", e->nome);
    printf("\nTipo do Equipamento: %s", e->tipo);
    printf("\nEndereco de IP: %s", e->IP);
    printf("\nEndereco MAC: %s", e->mac);
    printf("\nMarca: %s", e->marca);
    printf("\nModelo: %s", e->modelo);
    printf("\nData de verificacao: %02d/%02d/%04d", e->Data.dia, e->Data.mes, e->Data.ano);
    printf("\nLocalizacao: %s", e->localizacao);
    printf("\nEstado: %s\n", e->estado);
    printf("-------------------------------------\n");
}

void listar(equipamento *cabeca) {
    if (cabeca == NULL) {
        printf("\nLista de equipamentos vazia.\n");
        return;
    }
    printf("\n-----Listagem dos equipamentos------\n");
    equipamento *atual = cabeca;
    while (atual != NULL) {
        imprimirEquipamento(atual);
        atual = atual->prox;
    }
}

// comparacao de strings sem distinguir maiusculas/minusculas (portavel,
// nao depende de strcasecmp que nao existe em C standard / Windows)
int igualSemCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

void listarPorTipo(equipamento *cabeca) {
    if (cabeca == NULL) {
        printf("\nLista de equipamentos vazia.\n");
        return;
    }

    char tipo[MAXCHAR];
    lerTexto("Tipo a pesquisar (ex: Router, Switch, ...): ", tipo, MAXCHAR);

    int encontrados = 0;
    printf("\n-----Equipamentos do tipo '%s'------\n", tipo);
    equipamento *atual = cabeca;
    while (atual != NULL) {
        if (igualSemCase(atual->tipo, tipo)) {
            imprimirEquipamento(atual);
            encontrados++;
        }
        atual = atual->prox;
    }

    if (encontrados == 0) {
        printf("\nNenhum equipamento encontrado com o tipo '%s'.\n", tipo);
    }
}

void listarPorEstado(equipamento *cabeca) {
    if (cabeca == NULL) {
        printf("\nLista de equipamentos vazia.\n");
        return;
    }

    int op;
    const char *estado;
    do {
        printf("\nListar equipamentos com que estado?\n");
        printf(" 1 - Operacional\n");
        printf(" 2 - Em Falha\n");
        printf(" 3 - Em Manutencao\n");
        printf(" 4 - Desativado\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        estado = nomeEstado(op);
        if (estado == NULL) {
            printf("\nValor invalido!\n");
        }
    } while (estado == NULL);

    int encontrados = 0;
    printf("\n-----Equipamentos com estado '%s'------\n", estado);
    equipamento *atual = cabeca;
    while (atual != NULL) {
        if (igualSemCase(atual->estado, estado)) {
            imprimirEquipamento(atual);
            encontrados++;
        }
        atual = atual->prox;
    }

    if (encontrados == 0) {
        printf("\nNenhum equipamento encontrado com o estado '%s'.\n", estado);
    }
}

int contarEquipamentos(equipamento *cabeca) {
    int n = 0;
    while (cabeca != NULL) {
        n++;
        cabeca = cabeca->prox;
    }
    return n;
}

void liberarLista(equipamento *cabeca) {
    equipamento *atual = cabeca;
    while (atual != NULL) {
        equipamento *proximo = atual->prox;
        free(atual->nome);
        free(atual->tipo);
        free(atual->marca);
        free(atual->modelo);
        free(atual->IP);
        free(atual->mac);
        free(atual->localizacao);
        free(atual->estado);
        free(atual);
        atual = proximo;
    }
}

// devolve o CIU do ultimo equipamento da lista (0 se a lista estiver vazia)
int obterUltimoCIULista(equipamento *cabeca) {
    if (cabeca == NULL) {
        return 0;
    }
    while (cabeca->prox != NULL) {
        cabeca = cabeca->prox;
    }
    return cabeca->CIU;
}

// grava uma string no ficheiro como "tamanho + conteudo", sem desperdicar
// espaco com um tamanho fixo e sem limite de comprimento
void escreverString(char *str, FILE *fb) {
    int len = strlen(str);
    fwrite(&len, sizeof(int), 1, fb);
    fwrite(str, sizeof(char), len, fb);
}

// le uma string gravada por escreverString, alocando exatamente o espaco
// necessario (mantem a alocacao dinamica tambem ao carregar do ficheiro)
char *lerString(FILE *fb) {
    int len;
    fread(&len, sizeof(int), 1, fb);
    char *str = (char *)malloc(len + 1);
    fread(str, sizeof(char), len, fb);
    str[len] = '\0';
    return str;
}

// le apenas os primeiros 4 bytes do ficheiro (o cabeçalho com o ultimo CIU
// gravado), sem precisar de percorrer o resto do ficheiro
int obterProximoCIU(void) {
    FILE *fb = fopen(NOME_FICHEIRO, "rb");
    if (fb == NULL) {
        return 1;
    }
    int ultimoCIU;
    if (fread(&ultimoCIU, sizeof(int), 1, fb) != 1) {
        fclose(fb);
        return 1;
    }
    fclose(fb);
    return ultimoCIU + 1;
}

void carregar(equipamento **cabeca) {
    FILE *fb = fopen(NOME_FICHEIRO, "rb");
    if (fb == NULL) {
        return;
    }
    int cabecalho;
    if (fread(&cabecalho, sizeof(int), 1, fb) != 1) {
        fclose(fb);
        return; // ficheiro existe mas nao tem nem o cabecalho completo
    }
    int ciu;
    while (fread(&ciu, sizeof(int), 1, fb) == 1) {
        equipamento *novo = (equipamento *)malloc(sizeof(equipamento));
        novo->CIU = ciu;
        novo->nome        = lerString(fb);
        novo->tipo        = lerString(fb);
        novo->marca       = lerString(fb);
        novo->modelo      = lerString(fb);
        novo->IP          = lerString(fb);
        novo->mac         = lerString(fb);
        novo->localizacao = lerString(fb);
        novo->estado      = lerString(fb);
        fread(&novo->Data, sizeof(dataverificacao), 1, fb);
        novo->ant = NULL;
        novo->prox = NULL;
        inserirNoFim(cabeca, novo);
    }
    fclose(fb);
    printf("\n%d equipamento(s) carregado(s) de '%s'.\n",
           contarEquipamentos(*cabeca), NOME_FICHEIRO);
}

void guardar(equipamento *cabeca) {
    FILE *fb = fopen(NOME_FICHEIRO, "wb"); // "wb" reescreve o ficheiro do zero com a lista atual
    if (fb == NULL) {
        printf("\nErro ao abrir o ficheiro para gravacao!\n");
        return;
    }

    int ultimoCIU = obterUltimoCIULista(cabeca);
    fwrite(&ultimoCIU, sizeof(int), 1, fb); // cabecalho: usado por obterProximoCIU()

    equipamento *atual = cabeca;
    while (atual != NULL) {
        fwrite(&atual->CIU, sizeof(int), 1, fb);
        escreverString(atual->nome, fb);
        escreverString(atual->tipo, fb);
        escreverString(atual->marca, fb);
        escreverString(atual->modelo, fb);
        escreverString(atual->IP, fb);
        escreverString(atual->mac, fb);
        escreverString(atual->localizacao, fb);
        escreverString(atual->estado, fb);
        fwrite(&atual->Data, sizeof(dataverificacao), 1, fb);
        atual = atual->prox;
    }

    fclose(fb);
    printf("\nEquipamentos gravados em '%s' com sucesso.\n", NOME_FICHEIRO);
}

// percorre a lista a procura de um equipamento com o CIU indicado
equipamento *procurarPorCIU(equipamento *cabeca, int ciu) {
    while (cabeca != NULL) {
        if (cabeca->CIU == ciu) {
            return cabeca;
        }
        cabeca = cabeca->prox;
    }
    return NULL;
}

void removerEquipamento(equipamento **cabeca) {
    int ciu, confirmacao;

    printf("\nIndique o CIU do equipamento a remover: ");
    scanf("%d", &ciu);
    getchar();

    equipamento *alvo = procurarPorCIU(*cabeca, ciu);
    if (alvo == NULL) {
        printf("\nNao existe nenhum equipamento com o CIU %d.\n", ciu);
        return;
    }

    // TODO (Modulo 4): impedir a remocao se existirem incidentes tecnicos
    // pendentes associados a este CIU, assim que o modulo de incidentes existir

    printf("\nEquipamento encontrado: %s (%s)", alvo->nome, alvo->tipo);
    printf("\nConfirma a remocao? (Sim - 1 ; Nao - 0): ");
    scanf("%d", &confirmacao);
    getchar();

    if (confirmacao != 1) {
        printf("\nRemocao cancelada.\n");
        return;
    }

    if (alvo->ant != NULL) {
        alvo->ant->prox = alvo->prox;
    } else {
        *cabeca = alvo->prox; // alvo era a cabeca da lista
    }

    if (alvo->prox != NULL) {
        alvo->prox->ant = alvo->ant;
    }

    free(alvo->nome);
    free(alvo->tipo);
    free(alvo->marca);
    free(alvo->modelo);
    free(alvo->IP);
    free(alvo->mac);
    free(alvo->localizacao);
    free(alvo->estado);
    free(alvo);

    printf("\nEquipamento com CIU %d removido com sucesso.\n", ciu);
}

void alterarEquipamento(equipamento *cabeca) {
    int ciu;
    printf("\nIndique o CIU do equipamento a alterar: ");
    scanf("%d", &ciu);
    getchar();

    equipamento *alvo = procurarPorCIU(cabeca, ciu);
    if (alvo == NULL) {
        printf("\nNao existe nenhum equipamento com o CIU %d.\n", ciu);
        return;
    }

    int opcao;
    char temp[MAXCHAR];

    do {
        printf("\n--- A alterar equipamento CIU %d ---\n", alvo->CIU);
        printf(" 1 - Nome (atual: %s)\n", alvo->nome);
        printf(" 2 - Tipo (atual: %s)\n", alvo->tipo);
        printf(" 3 - Marca (atual: %s)\n", alvo->marca);
        printf(" 4 - Modelo (atual: %s)\n", alvo->modelo);
        printf(" 5 - Endereco IP (atual: %s)\n", alvo->IP);
        printf(" 6 - Endereco MAC (atual: %s)\n", alvo->mac);
        printf(" 7 - Localizacao (atual: %s)\n", alvo->localizacao);
        printf(" 8 - Estado (atual: %s)\n", alvo->estado);
        printf(" 9 - Data de verificacao (atual: %02d/%02d/%04d)\n",
               alvo->Data.dia, alvo->Data.mes, alvo->Data.ano);
        printf(" 0 - Terminar alteracao\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                lerTexto("Novo nome: ", temp, MAXCHAR);
                free(alvo->nome);
                alvo->nome = duplicarString(temp);
                break;
            case 2:
                lerTexto("Novo tipo: ", temp, MAXCHAR);
                free(alvo->tipo);
                alvo->tipo = duplicarString(temp);
                break;
            case 3:
                lerTexto("Nova marca: ", temp, MAXCHAR);
                free(alvo->marca);
                alvo->marca = duplicarString(temp);
                break;
            case 4:
                lerTexto("Novo modelo: ", temp, MAXCHAR);
                free(alvo->modelo);
                alvo->modelo = duplicarString(temp);
                break;
            case 5:
                lerTexto("Novo endereco IP: ", temp, MAXCHAR);
                free(alvo->IP);
                alvo->IP = duplicarString(temp);
                break;
            case 6:
                lerTexto("Novo endereco MAC: ", temp, MAXCHAR);
                free(alvo->mac);
                alvo->mac = duplicarString(temp);
                break;
            case 7:
                lerTexto("Nova localizacao: ", temp, MAXCHAR);
                free(alvo->localizacao);
                alvo->localizacao = duplicarString(temp);
                break;
            case 8:
                free(alvo->estado);
                alvo->estado = perguntarEstado();
                break;
            case 9:
                printf("Nova data de verificacao:\n");
                lerData(&alvo->Data.dia, &alvo->Data.mes, &alvo->Data.ano);
                break;
            case 0:
                printf("\nAlteracao terminada.\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}

int main(void) {
    setlocale(LC_ALL, "Portuguese");
    equipamento *dispositivos = NULL;
    int op;
    carregar(&dispositivos);
    int proxCIU = obterProximoCIU();
    printf("Proximo Codigo Interno Unico disponivel: %d\n", proxCIU);
    do {
        printf("\n-------------------------");
        printf("\n-------Menu de testes----");
        printf("\n1 - Adicionar Equipamento");
        printf("\n2 - Remover Equipamento");
        printf("\n3 - Alterar Dados / Estado de Equipamento");
        printf("\n4 - Listar todos os Equipamentos");
        printf("\n5 - Listar Equipamentos por Tipo");
        printf("\n6 - Listar Equipamentos por Estado");
        printf("\n7 - Pesquisar Equipamentos");
        printf("\n8 - Sair");
        printf("\n-------------------------");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &op);
        getchar();
        switch (op) {
            case 1:
                Adicionar(&dispositivos, &proxCIU);
                break;
            case 2:
                removerEquipamento(&dispositivos);
                break;
            case 3:
                alterarEquipamento(dispositivos);
                break;
            case 4:
                listar(dispositivos);
                break;
            case 5:
                listarPorTipo(dispositivos);
                break;
            case 6:
                listarPorEstado(dispositivos);
                break;
            case 7:
                printf("\nFuncionalidade ainda nao implementada.\n");
                break;
            case 8:
                printf("\nA gravar e a sair...\n");
                guardar(dispositivos);
                break;
            default:
                printf("\nValor invalido!!\n");
                break;
        }
    } while (op != 8);
    liberarLista(dispositivos);
    return 0;
}
