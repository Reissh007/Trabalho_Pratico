#include <stdio.h>
#include <stdlib.h>

int main() {
	
	//DO modulo 1
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
    // Alvo que queremos testar (pode ser um IP ou domínio)
    char ip[] = "192.168.56.1"; 
    char comando[100];
    
    //

    // Monta o comando. 
    // No Linux/macOS usamos "-c 4" para enviar apenas 4 pacotes.
    // No Windows, mudaria para "ping -n 4 %s"
    sprintf(comando, "ping -n 4 %s", ip);

    printf("A iniciar o ping para %s...\n\n", ip);
    
    // Executa o comando no terminal
    int status = system(comando);

    // Verifica se o comando foi executado com sucesso
    if (status == 0) {
        printf("\nO host respondeu com sucesso!\n");
    } else {
        printf("\nFalha ao contactar o host ou erro no comando.\n");
    }

    return 0;
}

