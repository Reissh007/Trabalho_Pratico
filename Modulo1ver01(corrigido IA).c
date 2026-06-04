#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define MAXCHAR 100

// MODULO 1 
// Simplificações do sistema
//CIU - codigo interno unico


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
    char *dataverificacao;
} equipamento;

// FUNÇAO TESTE DE PING - Corrigido: alterado o parâmetro de 'char' para 'char*'
int testip(char *ip, int so) {
    char comando[MAXCHAR + 50]; // Tamanho aumentado para evitar buffer overflow
    
    if (so == 1) {
        printf(" -MS-WINDOWS- \n"); // Corrigido: adicionado ';'
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

// Adiciona equipamentos - Corrigido: Ajuste na sintaxe de ponteiros
void Adicionar(equipamento *E, int *i, int so) {
    char temp[MAXCHAR];
    int resp;

    do {
        printf("----------------------------------------------");
        printf("\n Codigo Interno Unico (CIU): %d ", *i); // Usa *i para ver o valor
        
        E[*i].CIU = *i; // Atribui o CIU automaticamente

        printf("\n Nome Do equipamento: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E[*i].nome = (char*)malloc((strlen(temp) + 1) * sizeof(char)); // +1 para o '\0'
        strcpy(E[*i].nome, temp);

        printf("\n Tipo de equipamento: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E[*i].tipo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E[*i].tipo, temp);

        printf("\n Marca: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E[*i].marca = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E[*i].marca, temp);

        printf("\n Modelo: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
        E[*i].modelo = (char*)malloc((strlen(temp) + 1) * sizeof(char));
        strcpy(E[*i].modelo, temp);

        // LÓGICA DO PING CORRIGIDA
        do {
            printf("\n Endereço IP: "); // Corrigido: adicionado ';'
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            
            E[*i].IP = (char*)malloc((strlen(temp) + 1) * sizeof(char));
            strcpy(E[*i].IP, temp);

            // Chama a função testip passando o IP e o SO selecionado
            if (testip(E[*i].IP, so) != 0) {
                printf("\n[ERRO] IP inacessível. Tente novamente.\n");
                free(E[*i].IP); // Liberta a memória do IP errado antes de repetir
                E[*i].IP = NULL;
            }
        } while (E[*i].IP == NULL);
        
        // Validação da Resposta de Continuidade
        do {     
            printf("\nDeseja Introduzir mais Equipamentos? (Sim - 1 ; Não - 0): ");
            // Inverti a lógica (1 para Sim, 0 para Não) para fazer mais sentido com o fluxo do while
            scanf("%d", &resp); // Corrigido: %d e &resp
            getchar(); // Limpa o buffer do teclado para o próximo fgets

            if (resp == 0) {
                printf("\n A terminar a inserção... \n");
            } else if (resp != 1) {
                printf("\n Resposta invalida \n");
            }
        } while (resp != 0 && resp != 1); 
        
        (*i)++; // Avança para a próxima posição do array de estruturas
        
    } while (resp == 1); 
}
// LISTAR 
void listar(equipamento *E,int i, int so){
	int n;
	printf("\n-----Listagem dos equipamentos------")
		if (i != 0){
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
