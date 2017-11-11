#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nick[50];
    char pass[50];
    int id;
    char nome[50];
    char email[40];
    char turma[10];
    int saldo;
} Tcliente;

typedef struct {
    char ID[20];
    char cor[20];
    char marca[50];
    char modelo[30];
    char tipo[20];
    int mudancas;
    char matricula[15];
} Tviatura;

int sizeCliente = 1;
Tcliente *arrCliente;

int sizeViatura = 1;
Tviatura *arrViatura;

void lerDadosParaMemoriaCliente(){
    FILE *file;
    file = fopen("../utilizadores.txt", "r"); //usar "utilizadores.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
    if ( file == NULL ){
        perror("Nao foi possivel abrir o ficheiro\n");
        exit(1);
    }
    printf("A ler utilizadores.");
    char s[100];
    arrCliente = (Tcliente*) malloc(sizeCliente * sizeof(Tcliente));
    if(arrCliente == NULL)
    {
        printf("Erro! Memoria não foi alocada.");
        exit(0);
    }
    while ( fgets (s, 100, file ) != NULL ) {
        char *end;
        Tcliente cliente;
        strcpy(cliente.nick, strtok (s,";"));
        strcpy(cliente.pass, strtok (NULL,";"));
        cliente.id =  (int) strtol(strtok(NULL, ";"), &end, 10);
        strcpy(cliente.nome, strtok (NULL,";"));
        strcpy(cliente.email, strtok (NULL,";"));
        strcpy(cliente.turma, strtok (NULL,";"));
        cliente.saldo = (int) strtol(strtok(NULL, ";"), &end, 10);
        arrCliente[sizeCliente-1] = cliente;
        sizeCliente++;
        arrCliente = realloc(arrCliente, sizeCliente * sizeof(Tcliente));
        printf(".");
    }
}

void lerDadosParaMemoriaViatura() {
    FILE *file;
    file = fopen("../viaturas.txt", "r"); //usar "viaturas.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
    if ( file == NULL ){
        perror("Nao foi possivel abrir o ficheiro\n");
        exit(1);
    }
    printf("A ler viaturas.");
    char s[100];
    arrViatura = (Tviatura*) malloc(sizeViatura * sizeof(Tviatura));
    if(arrViatura == NULL)
    {
        printf("Erro! Memoria não foi alocada.");
        exit(0);
    }
    while ( fgets (s, 100, file ) != NULL ) {
        char *end;
        Tviatura viatura;
        strcpy(viatura.ID, strtok (s,";"));
        strcpy(viatura.cor, strtok (NULL,";"));
        strcpy(viatura.marca, strtok (NULL,";"));
        strcpy(viatura.modelo, strtok (NULL,";"));
        strcpy(viatura.tipo, strtok (NULL,";"));
        viatura.mudancas = (int) strtol(strtok(NULL, ";"), &end, 10);
        strcpy(viatura.matricula, strtok (NULL,";"));
        arrViatura[sizeViatura-1] = viatura;
        sizeViatura++;
        arrViatura = realloc(arrViatura, sizeViatura * sizeof(Tviatura));
        printf(".");
    }
}

void lerDadosParaMemoria() {
    lerDadosParaMemoriaCliente();
    printf("\n");
    lerDadosParaMemoriaViatura();
    printf("\nDados guardados em memória com sucesso.\n");
}

void imprimirMemoria() {
    if (arrCliente!= NULL) {
        printf("Clientes (Quantidade - %d):\n\n", sizeCliente-1);
        for(int i = 0; i < sizeCliente-1; ++i) {
            Tcliente cliente = arrCliente[i];
            printf (" Nickname: %s\n Password: %s\n ID: %d\n Nome: %s\n Email: %s\n Turma: %s\n Saldo: %d\n\n", cliente.nick, cliente.pass, cliente.id, cliente.nome, cliente.email, cliente.turma, cliente.saldo);
        }
    } else
        printf("Memória de clientes vazia.\n");
    if (arrViatura!= NULL) {
        printf("Viaturas (Quantidade - %d):\n\n", sizeViatura-1);
        for(int i = 0; i < sizeViatura-1; ++i) {
            Tviatura viatura = arrViatura[i];
            printf(" ID: %s\n Cor: %s\n Marca: %s\n Modelo: %s\n Tipo: %s\n Mudanças: %d\n Matricula: %s\n", viatura.ID, viatura.cor, viatura.marca, viatura.modelo, viatura.tipo, viatura.mudancas, viatura.matricula);
        }
    } else
        printf("Memória de viaturas vazia.\n");
}

void alterarUtilizador() {
    char s[50];
    printf("\nIntroduza o nickname do utilizador a alterar: ");
    fgets( s, 50, stdin);
    s[ strlen(s)-1 ] = 0;
    for(int i = 0; i < sizeCliente-1; ++i) {
        if (strcmp(arrCliente[i].nick,s)==0) {
            printf("Utilizador encontrado.\n");
            char j[5];
            while (1) {
                printf("Deseja alterar a password? (s/n): ");
                fgets(j, 5, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char pass[20];
                    printf("Digite a nova password: ");
                    fgets(pass, 20, stdin);
                    pass[strlen(pass) - 1] = 0;
                    strcpy(arrCliente[i].pass, pass);
                    break;
                }
            }
            while (1) {
                printf("Deseja alterar o saldo? (s/n): ");
                fgets( j, 5, stdin);
                j[ strlen(j)-1 ] = 0;
                if (strcmp(j, "n")==0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char saldo[20], *end;
                    printf("Digite o novo saldo: ");
                    fgets(saldo, 20, stdin);
                    saldo[ strlen(saldo)-1 ] = 0;
                    arrCliente[i].saldo = (int) strtol(saldo, &end, 10);
                    break;
                }
            }
            printf("Alterações guardadas na memória com sucesso.\n");
            return;
        }
    }
    printf("Utilizador não encontrado.\n");
}


int main() {
    printf("Programa administrador\n\n");

    printf("1 - Ler dados para memória\n");
    printf("2 - Imprimir memória\n");
    printf("3 - Alterar utilizador\n");
    printf("4 - Alterar viatura\n");
    printf("5 - Guardar dados\n");
    printf("0 - Sair\n");

    char s[10], *end;
    while (1) {
        printf("\nEscolha um menu: ");
        fgets( s, 10, stdin);
        s[ strlen(s)-1 ] = 0;
        long menuOption = strtol(s, &end, 10);
        switch (menuOption) {

            case 1 :
                lerDadosParaMemoria();
                break;

            case 2 :
                imprimirMemoria();
                break;

            case 3 :
                alterarUtilizador();
                break;

            case 4 :
                printf("4");
                break;

            case 5 :
                printf("5");
                break;

            default :
                if (strcmp(end, "sair")==0){
                    free(arrCliente);
                    free(arrViatura);
                    exit(0);
                }
                printf("Escolha uma opção válida.\n");
        }
    }
}