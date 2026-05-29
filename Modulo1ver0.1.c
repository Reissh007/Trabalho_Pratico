#include <stdio.h>
#include <string.h>
#include <stl>

// MODULO 1 
// Simplificações do sistema
//CIU - codigo interno unico

typedef struct EQUIPAMENTOS{
	int CIU;
	char *nome;
	char *tipo;
	char *marca;
	char *modelo;
	char *IP;
	char *mac;
	char *localizacao;
	char *dataverificacao;
	
}equipamento;
//FUNÇAO TESTE DE PING se =1 deu erro, caso 0 executado com sucesso
int testip(char ip){
	
}

//..Adiciona equipamentos (tentando poupar memoria)
void Adicionar(equipamento *E,int i){
char temp[60];
	do{
		printf("----------------------------------------------")
		printf("\n Introduza: ");
		printf("\n Nome Do equipamento: ")	
		fgets(temp, sizeof(temp),stdin);
		temp[strcspn(temp, "\n")]= '\0';
		E[i]->nome =(char*)malloc(strlen(temp)*sizeof(char));
		strcpy (E[i]->nome , temp);
		printf("\n Tipo de equipamento: ")
		fgets(temp,sizeof(temp),stdin);
		temp[strcspn(temp,"\n")]= '\0';
		E[i]->tipo = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(E[i]->tipo , temp);
		printf("\n Marca: ")
		fgets(temp,sizeof(temp),stdin);
		temp[strcspn(temp,"\n")]= '\0';
		E[i]->marca = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(E[i]->marca , temp);
		printf("\n Modelo: ")
		fgets(temp,sizeof(temp),stdin);
		temp[strcspn(temp,"\n")]= '\0';
		E[i]->modelo = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(E[i]->modelo , temp);
		printf("\n IP: ")
		fgets(temp,sizeof(temp),stdin);
		temp[strcspn(temp,"\n")]= '\0';
		E[i]->IP = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(E[i]->IP , temp);
		//FAZER O TESTE DE IP
		if(testeip==1){ //1 - erro No ping do IP
			printf("IP")
		}
		printf("\n T: ")
		fgets(temp,sizeof(temp),stdin);
		temp[strcspn(temp,"\n")]= '\0';
		E[i]->tipo = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(E[i]->tipo , temp);
		printf("");
	}while(resp==Y || resp==y);	
	 	
free(E);}
int main(){
	#Menu
	do{
	
	printf("\n-------------------------");
	printf("\n-------Menu de testes----");
	printf("\n1-Adicionar Equipamento");
	printf("\n2-");
	printf("\n3- ");
	printf("\n4-");
	printf("\n5- ");
	printf("\n6- Sair                  ");
	printf("\n-------------------------");
	printf ("\n:");
	scanf("%d",&op);
	switch (op){
	
	case 1:
		 
		break;
	case 3:
		break;
	case 4:
		break;
	
	case 5:
		break;
		
	case 6:
		break;}	
	default:
		printf("\nValor invalido!!")
	
while (op!=6);};
printf("Saindo...");
return 0;}
return0;}
