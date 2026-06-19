#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int testip(char *ip, int so) { //usa ping diretamente
	FILE *f= NULL ; // apontador para o ficheiro de resultado de ping 
    char comando[MAXCHAR]; 
    char datahora[MAXCHAR]; //variavel que vai carregar data e hora usando biblioteca time.h
    time_t agora = time(NULL); // Pega tempo atual do sistema e guarda
    f = fopen("pingresultado.txt" ,"a");
    if (f== NULL){
    	printf("\nErro ao abrir o ficheiro \nCriando Ficheiro");
    	f= fopen("pingresultado.txt" ,"w");
    	
   	 	
    	
	}
	strftime(datahora, sizeof(datahora), "%Y-%m-%d %H:%M:%S", localtime(&agora));
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
        if (f != NULL){
        	fprintf(f, "\n=== %s ===\n", datahora); 
        	fprintf(f,"\nIP: %s",ip);
        	fprintf(f,"\nComando: %s ",comando); //so para ter certeza que foi feito o comando certo
        	fprintf(f,"\nRESULTADO DO PING : SUCESSO");
			fprintf( "\n--------------------------------------------\n");
		}
        return 0;
    } else {
        printf("\nFalha ao contactar o host ou erro no comando.\n");
        if (f != NULL){
        	fprintf(f, "\n=== %s ===\n", datahora);
        	fprintf(f,"\nIP: %s",ip);
        	fprintf(f,"\nComando: %s ",comando); //so para ter certeza que foi feito o comando certo
        	fprintf(f,"\nRESULTADO DO PING : FALHA \n\n--------------------------");
		}
        return 1;
    }
fclose(f);
}

/* inutil agora
int main() {
	
	//DO modulo 
        do {
            printf("\n Endereço IP: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = '\0';
            
            E[*i].IP = (char*)malloc((strlen(temp) + 1) * sizeof(char));
            strcpy(E[*i].IP, temp);

            // Chama a função testip passando o IP e o SO selecionado (1 windows 0 linux)
            if (testip(E[*i].IP, so) != 0) {
                printf("\n[ERRO] IP inacessível. Tente novamente.\n");
                free(E[*i].IP);
                E[*i].IP = NULL;
            }
        } while (E[*i].IP == NULL);
   

    return 0;
}

