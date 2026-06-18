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
    char *estado;
    dataverificacao *Data;
    EQUIPAMENTOS *ant;
    EQUIPAMENTOS *prox;
} equipamento;

// FUNÇAO TESTE DE PING,coloquei para uso do modulo 2
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

void estado(equipamento *E){
	int op;
	char temp[MAXCHAR];
	printf("\n Qual o estado do equipamentos? \n 1 - Operacional \n 2 - Em Falha \n 3 - Em Manutenção \n 4 -Desativado;")
	scanf("%d",&op);
	switch(op){
	case 1:
		printf("\nEstado Operacional");
		sprintf(temp,"Operacional")
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
		
	case 2:
	printf("\nEstado Em Falha");
		sprintf(temp,"Em Falha")
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
	case 3:	
		printf("\nEstado Em Manutencao");
		sprintf(temp,"Em Manutencao")
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
	case 4:
	printf("\nEstado Desativado");
		sprintf(temp,"Desativado");
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
		default:
            printf("\nValor invalido!!\n"); 
            break;}
		
}

// Adiciona equipamentos 
void Adicionar(equipamento *E) {
//verificação do inicio
	int n=1
    char temp[MAXCHAR];
    int resp;

    
        printf("----------------------------------------------");
        printf("\n Codigo Interno Unico (CIU): %d ", *i); // Usa *i para ver o valor
        
        E.CIU = ; // Atribui o CIU automaticamente TRABALHAR NISSO!
        

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
        
        printf("\n Data da ultima verificação: /n Dia: ");
        scanf("%d",E.Data.dia);
        printf("/n Mes: ");
        scanf("%d",E.Data.mes);
    	printf("/n Ano: ");
		scanf("%d",E.Data.ano);
		sprintf(temp,"%d/%d/%d",E.Data.dia);
		E.Data.dataver =(char*)malloc(strlen(temp) * sizeof(char));
		strcpy(E.Data.dataver, temp);
		
		printf("\n Localizacao do Despositivo: "); 
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E.localizacao = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E.localizacao, temp);
        
        printf("\n Estado Operacional do equipamento: "); 
        // FAZER FUNÇÂO PARA ISSO
        estado
        
        
        
        // Validação da Resposta de Continuidade
        do {     
            printf("\nDeseja Introduzir mais Equipamentos? (Sim - 1 ; Não - 0): ");
            
            scanf("%d", &resp); 
            getchar(); 
            
			if (resp == 1){
				Adicionar(E.prox ); break; //forma recursiva 
			}
            if (resp == 0) {
                printf("\n A terminar a inserção... \n"); break;} 
			if (resp != 1) {
                printf("\n Resposta invalida \n");
            }
        } while (resp != 0 && resp != 1); 
} 
void verificarlista(equipamento *E,int n){
	if (E == NULL){
		switch(n){
			case 1:
			printf("\n Lista vazia, Ininiciando inserção");
			Adicionar(E); break;
	
		case 2:
		printf("\n Lista vazia");
		break;}
	}
	if (E != NULL){
		switch(n){
		case 1:
				printf("\n Inicio cheio? \nVerificar anterior");
			if (E.ant == NULL ){
				printf("\n Anterior vazio");
				printf("\n Verificar proximo ");
				verificarlista(E.prox); break;}
				
			else {printf("Anterior nao vazio, meio de uma lista? ");
			 prinff("Verificar proximo? ainda em progresso...");break;}
	}
		
		case 2:
		printf("\n Iniciar Listagem");
		listar(E);
		break;}
	
	
}
// LISTAR 
void listar(equipamento *E){
	int n=2
	printf("\n-----Listagem dos equipamentos------")
		if ( E != NULL){
			
				printf("\nCodigo Interno Unico: %d", E.CIU);
				printf("\nNome do Equipamento: %s", E.nome);
				printf("\nTipo do Equipamento:%s", E.tipo);
				printf("\nEndereço de IP:%s", E.IP);
				printf("\nEndereco MAC: %s",E.mac);
				printf("\nMarca: %s", E.marca);
				printf("\nModelo: %s", E.modelo);
				printf("\nData de verificacao: %s ", E.dataverificacao);
				printf("\nLocalização:%s", E.localizacao);
				verificarlista(E.prox,n) 
				}
	else {
		printf("/nLista chegou ao fim");
	}
}

// MAIN
int main() {
		setlocale(LC_ALL, "Portuguese");
    int so = 1; 
    int op;
    int k;
    
   
    equipamento dispositivo; 

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
                Adicionar(dispositivo)
                break;
            case 2:
            	listar
            case 6:
                printf("Saindo e libertando memória...\n");
                break;
            default:
                printf("\nValor invalido!!\n"); 
                break;
        }    
    } while (op != 6);

    // free (libertação da memoria)
    for (k = 0; k < n; k++) {
        free(dispositivo[k].nome);
        free(dispositivo[k].tipo);
        free(dispositivo[k].marca);
        free(dispositivo[k].modelo);
        free(dispositivo[k].IP);
    }

    return 0;
}
