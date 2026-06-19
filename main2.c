#include <stdio.h>
#include <stdlib.h>

// funcoes dos outros modulos
void menuModulo3();
void menuModulo4();
void menuModulo5();
void guardarBinario();
void carregarBinario();
void guardarIncidentes();
void carregarIncidentes();
void guardarConfigs();
void carregarConfigs();

int main() {
    printf("A carregar dados...\n");
    carregarBinario();
    carregarIncidentes();
    carregarConfigs();

    int op;
    do {
        printf("\n=============================\n");
        printf("   Mini NOC - Menu Principal\n");
        printf("=============================\n");
        printf("3 - Modulo 3 (Sensores)\n");
        printf("4 - Modulo 4 (Incidentes)\n");
        printf("5 - Modulo 5 (Configuracoes)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 3: menuModulo3(); break;
            case 4: menuModulo4(); break;
            case 5: menuModulo5(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while(op != 0);

    printf("A guardar dados...\n");
    guardarBinario();
    guardarIncidentes();
    guardarConfigs();
    printf("Ate logo!\n");
    return 0;
}
