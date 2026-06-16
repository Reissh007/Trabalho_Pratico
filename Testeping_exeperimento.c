#include <stdio.h>
#include <stdlib.h>
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

