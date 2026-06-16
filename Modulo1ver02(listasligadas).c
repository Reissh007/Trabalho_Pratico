#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define MAXCHAR 150

// MODULO 1 
// Simplificações do sistema
//CIU - codigo interno unico

// ESTRUTURA DATA
typedef struct dataverificacao{
	int dia;
	int mes;
	int ano;
	char *dataver;
};

//ESTRUTURA DOS EQUIPAMENTOS 
typedef struct EQUIPAMENTOS {
    int CIU;
    char *nome;
    char *tipo;
    char *marca;
    char *modelo;
    char *IP;
    char *mac;
    char *localizacao;
    dataverificacao *Data;
    EQUIPAMENTOS *ant;
    EQUIPAMENTOS *prox;
} equipamento;

// FUNÇAO TESTE DE PING,coloquei para uso no modulo 2
int testip(char *ip, int so) {
    char comando[MAXCHAR];
    
    if (so == 1) {
        printf(" -MS-WINDOWS- \n"); 
        sprintf(comando, "ping -n 4 %s", ip);
    } else {
        printf(" -LINUX- \n");
        sprintf(comando, "ping -c 4 %s", ip);
    }

    printf("A iniciar o ping para %s...\n\n", ip);
    int status = system(comando);

    if (status == 0) {
        printf("\nO host respondeu com sucesso!\n");
        return 0;
    } else {
        printf("\nFalha ao contactar o host ou erro no comando.\n");
        return 1;
    }
}

// Adiciona equipamentos 
void Adicionar(equipamento *E) {
//verificação do inicio

    char temp[MAXCHAR];
    int resp;

    
        printf("----------------------------------------------");
        printf("\n Codigo Interno Unico (CIU): %d ", *i); // Usa *i para ver o valor
        
        E.CIU = *i; // Atribui o CIU automaticamente

        printf("\n Nome Do equipamento: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.nome = (char*)malloc((strlen(temp) + 1) * sizeof(char)); 
        strcpy(E.nome, temp);

        printf("\n Tipo de equipamento: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.tipo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.tipo, temp);

        printf("\n Marca: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.marca = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.marca, temp);

        printf("\n Modelo: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.modelo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.modelo, temp);
		// Teste de Ping movido para Testeping_experimento
      
        printf("\n Modelo: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.modelo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.modelo, temp);
        
        printf("\n Endereço MAC: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.mac = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.mac, temp);
        
        printf("\n Endereço IP: "); 
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.IP = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.IP, temp);
        
        printf("\n Data de verificação: /n Dia: ");
        scanf("%d",E.Data.dia);
        printf("/n Mes: ");
        scanf("%d",E.Data.mes);
    	printf("/n Ano: ");
		scanf("%d",E.Data.ano);
		sprintf(temp,"%d/%d/%d",E.Data.dia);
		E.Data.dataver =(char*)malloc(strlen(temp) * sizeof(char));
		strcpy(E.Data.dataver, temp);
		
    
        // Validação da Resposta de Continuidade
        do {     
            printf("\nDeseja Introduzir mais Equipamentos? (Sim - 1 ; Não - 0): ");
            // Inverti a lógica (1 para Sim, 0 para Não) para fazer mais sentido com o fluxo do while
            scanf("%d", &resp); // Corrigido: %d e &resp
            getchar(); // Limpa o buffer do teclado para o próximo fgets
            
			if (resp == 1){
				Adicionar(E.prox ); break;//forma recursiva 
			}
            if (resp == 0) {
                printf("\n A terminar a inserção... \n"); break;} 
			if (resp != 1) {
                printf("\n Resposta invalida \n");
            }
        } while (resp != 0 && resp != 1); 
} 
void verificarlista(equipamento *E){
	if (E == NULL){
		printf("\n Lista vazia, Ininiciando inserção");
		Adicionar(E); break;
	}
	if (E != NULL){
		printf("\n Inicio cheio? \nVerificar anterior");
			if (E.ant == NULL ){
				printf("\n Anterior vazio");
				printf("\n Verificar proximo ");
				verificarlista(E.prox); break;}
				
			else {printf("Anterior nao vazio, meio de uma lista? ");
			 prinff("Verificar proximo? ainda em progresso...");break;}
	}
	
}
// LISTAR 
void listar(equipamento *E){
	int n;
	printf("\n-----Listagem dos equipamentos------")
		if ( E != NULL){
			for (n=0; n<i; n++){
				printf("\nCodigo Interno Unico: %d", E[n].CIU);
				printf("\nNome do Equipamento: %s", E[n].nome);
				printf("\nTipo do Equipamento:%s", E[n].tipo);
				printf("\nEndereço de IP:%s", E[n].IP);
				printf("\nMarca:%s", E[n].marca);
				printf("\nModelo:%s", E[n].modelo);
				// ainda nao feito printf("\nData de verificacao:%s", E[n].dataverificacao);
				//printf("\nMAC:%s", E[n].mac);
				//printf("\nLocalização:%s", E[n].localizacao); 
				}
			}
	else {
		printf("/nLista chegou ao fim");
	}
}

// MAIN
int main() {
		setlocale(LC_ALL, "Portuguese");// exemplo que a professora Tania deu.
    int so = 1; // Padrão: Windows. Pode ser alterado conforme o sistema.
    int op;
    int n = 0; // Número total de equipamentos inseridos
    int k;
    
    // Alocação estática de um array ou dinâmica de tamanho fixo inicial para evitar crash
    // Alocamos espaço para até 50 equipamentos para começar de forma segura
    equipamento dispositivo[50]; 

    printf("Selecione o seu Sistema Operativo (1- Windows | 0- Linux): ");
    scanf("%d", &so);
    getchar();

    // MENU    
    do {
        printf("\n-------------------------");
        printf("\n-------Menu de testes----");
        printf("\n1-Adicionar Equipamento \n2-Listar equipamentos");
        printf("\n2 Listar todas os equipamentos ");
          printf("\n3 ");
        printf("\n6- Sair                  ");
        printf("\n-------------------------");
        printf("\n Escolha uma opção: ");
        scanf("%d", &op);
        getchar(); // Limpa o buffer

        switch (op) {
            case 1:
                Adicionar(dispositivo, &n, so);
                break;
            case 6:
                printf("Saindo e libertando memória...\n");
                break;
            default:
                printf("\nValor invalido!!\n"); // Corrigido: adicionado ';'
                break;
        }    
    } while (op != 6);

    // LIBERAÇÃO DA MEMÓRIA ALOCADA DINAMICAMENTE ANTES DE FECHAR
    for (k = 0; k < n; k++) {
        free(dispositivo[k].nome);
        free(dispositivo[k].tipo);
        free(dispositivo[k].marca);
        free(dispositivo[k].modelo);
        free(dispositivo[k].IP);
    }

    return 0;
}
