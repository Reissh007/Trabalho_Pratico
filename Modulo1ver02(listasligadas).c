#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
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


void estado(equipamento *E){
	int op;
	char temp[MAXCHAR];
	printf("\n Qual o estado do equipamentos? \n 1 - Operacional \n 2 - Em Falha \n 3 - Em Manutenção \n 4 -Desativado;");
	getchar();
	scanf("%d",&op);
	switch(op){
	case 1:
		printf("\nEstado Operacional");
		sprintf(temp,"Operacional");
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
		
	case 2:
	printf("\nEstado Em Falha");
		sprintf(temp,"Em Falha");
		E->estado=(char*)malloc(strlen(temp));
		strcpy(E->estado,temp);
		break;
	case 3:	
		printf("\nEstado Em Manutencao");
		sprintf(temp,"Em Manutencao");
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
        E->tipo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->tipo, temp);

        printf("\n Marca: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->marca = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->marca, temp);

        printf("\n Modelo: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->modelo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->modelo, temp);
		// Teste de Ping movido para Testeping_experimento
      
        printf("\n Modelo: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->modelo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->modelo, temp);
        
        printf("\n Endereço MAC: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->mac = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->mac, temp);
        
        printf("\n Endereço IP: "); 
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->IP = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->IP, temp);
        
        printf("\n Data da ultima verificação: /n Dia: ");
      	E->Data = malloc(sizeof(dataverificacao)); //pesquisei para ter certeza
        scanf("%d",E->Data.dia);
        printf("/n Mes: ");
        scanf("%d",E->Data.mes);
    	printf("/n Ano: ");
		scanf("%d",E->Data.ano);
		sprintf(temp,"%d/%d/%d",E->Data.dia);
		E->Data.dataver =(char*)malloc(strlen(temp) * sizeof(char));
		strcpy(E->Data.dataver, temp);
		
		printf("\n Localizacao do Despositivo: "); 
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E->localizacao = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E->localizacao, temp);
        
        printf("\n Estado Operacional do equipamento: "); 
        estado(E)
        
        
        
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
void guardar(equipamento *E, FILE *fb){
	
	//guardar num ficheiro todas as informações dentro da lista ligada
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
			if (E->ant == NULL ){
				printf("\n Anterior vazio");
				printf("\n Verificar proximo ");
				verificarlista(E->prox); break;}
				
			else {printf("Anterior nao vazio, meio de uma lista? ");
			 prinff("Verificar proximo? ainda em progresso...");break;}
	}
		
		case 2:
		printf("\n Iniciar Listagem");
		listar(E);
		break;
		}
	
	
}
// LISTAR 
void listar(equipamento *E){
	int n=2
	printf("\n-----Listagem dos equipamentos------")
		if ( E != NULL){
			
				printf("\nCodigo Interno Unico: %d", E->CIU);
				printf("\nNome do Equipamento: %s", E->nome);
				printf("\nTipo do Equipamento:%s", E->tipo);
				printf("\nEndereço de IP:%s", E->IP);
				printf("\nEndereco MAC: %s",E->mac);
				printf("\nMarca: %s", E->marca);
				printf("\nModelo: %s", E->modelo);
				printf("\nData de verificacao: %s ", E->dataverificacao);
				printf("\nLocalização: %s", E->localizacao);
				verificarlista(E->prox,n); 
				}
	else {
		printf("/nLista chegou ao fim");
	}
}

// MAIN
int main() {
		setlocale(LC_ALL, "Portuguese");
        int op;
        int so;
   	 	int atual;
   	 	int n; //so para fazer uso na libertação de memoria
		equipamento *dispositivo = NULL; 
		FILE *fb= NULL;
	fb=fopen("registros_equipamentos.dat","ab+") //nome do ficheiro?
    fread(dispositivo, sizeof(equipamento),k,f); //leitura do ficheiro (comecando do INICIO)
	printf("")
	printf("Selecione o seu Sistema Operativo (1- Windows | 0- Linux): ");
    scanf("%d", &so);
    getchar();


    // MENU    
    do {
        printf("\n-------------------------");
        printf("\n-------Menu de testes----");
        printf("\n1-Adicionar Equipamento");
        printf("\n2 Listar todos os Equipamentos ");
        printf("\n3 Remover um Equipamento");
        printf("\n4 Alterar dados um Equipamento");
        printf("\n5 Pesquisar Equipamentos");
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
            	listar(dispositivo);
            	break;
            case 3:
            	// remover(dispositivo); //criando prioridade 1 (involve incidentes)
            	break;
            case 4: 
            	// alterar(dispositivo); //criando
            	break;	
            case 5:
            	 //pesquisa(dispositivo); //criando prioridade 2
            	break;
            case 6:
                printf("Saindo e libertando memória...\n");
                guardar(dispositivos);
                break;
            default:
                printf("\nValor invalido!!\n"); 
                break;
        }    
    } while (op != 6);
 
	fseek(fb, 0, SEEK_END); //move para O FIM do ficheiro
    k = ftell(fb) / sizeof(equipamento);//ftell diz a conta o numero de cacteres apartir da posicao atual dentro do ficheiro (neste caso estamos no fim do ficheiro)
    // e dividindo tudo pelo tamanho da estrutura nos da a quantidade de estruturas que vao ser necessarias
    // free (libertação da memoria)
    for (n= 0; n < k; k++) {
        free(dispositivo[k].nome);
        free(dispositivo[k].tipo);
        free(dispositivo[k].marca);
        free(dispositivo[k].modelo);
        free(dispositivo[k].IP);
    }

    return 0;
}
