#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sem.h>

#define RESET   "\033[0m"
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

struct sembuf DOWN = { 0, -1, 0};
struct sembuf UP = { 0, 1, 0};

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
    int available;
    long timeStarted;
} Tviatura;

int sizeCliente = 0;
Tcliente* arrCliente;

int sizeViatura = 0;
Tviatura* arrViatura;

int semaforos;

void lerDadosParaMemoriaCliente(){
    
    FILE *file;
    if ((file = fopen("utilizadores.dat", "r")) == NULL) { //usar "utilizadores.dat" se o ficheiro compilado estiver no mesmo sitio do .dat
        if ((file = fopen("utilizadores.txt", "r")) == NULL) { //usar "utilizadores.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
            printf("Nao foi possivel abrir o ficheiro com os clientes.\n");
            exit(1);
        }
        printf("A ler clientes de clientes.txt");
        char s[100];
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
            arrCliente[sizeCliente] = cliente;
            sizeCliente++;
            printf(".");
        }
    } else {
        printf("A ler clientes de clientes.dat");
        while (fread(&arrCliente[sizeCliente], sizeof(Tcliente), 1, file) > 0) {
            sizeCliente++;
            printf(".");
        }
    }
    fclose(file);
}

void lerDadosParaMemoriaViatura() {
    
    FILE *file;
    if ((file = fopen("viaturas.dat", "r")) == NULL) { //usar "viaturas.dat" se o ficheiro compilado estiver no mesmo sitio do .dat
        if ((file = fopen("viaturas.txt", "r")) == NULL) { //usar "viaturas.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
            printf("Nao foi possivel abrir o ficheiro com as viaturas.\n");
            //free(arrViatura);
            exit(1);
        }
        printf("A ler viaturas de viaturas.txt");
        char s[100];
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
            viatura.available = 1;
            viatura.timeStarted = -1;
            arrViatura[sizeViatura] = viatura;
            sizeViatura++;
            printf(".");
        }
    } else {
        printf("A ler viaturas de viaturas.dat");
        while (fread(&arrViatura[sizeViatura], sizeof(Tviatura), 1, file) > 0) {
            sizeViatura++;
            printf(".");
        }
    }
    fclose(file);
}

void lerDadosParaMemoria() {
    lerDadosParaMemoriaCliente();
    printf("\n");
    lerDadosParaMemoriaViatura();
    printf("\nDados guardados em memória com sucesso.\n");
}

void imprimirMemoria() {
    if (arrCliente!= NULL) {
        printf(BOLDCYAN"\nClientes (Quantidade - %d):\n\n"RESET, sizeCliente);
        for(int i = 0; i < sizeCliente; ++i) {
            Tcliente cliente = arrCliente[i];
            printf (BOLDCYAN" Nickname:"RESET" %s\n "BOLDCYAN"Password:"RESET" %s\n "BOLDCYAN"ID:"RESET" %d\n "BOLDCYAN"Nome:"RESET" %s\n "BOLDCYAN"Email:"RESET" %s\n "BOLDCYAN"Turma:"RESET" %s\n "BOLDCYAN"Saldo:"RESET" %d\n\n", cliente.nick, cliente.pass, cliente.id, cliente.nome, cliente.email, cliente.turma, cliente.saldo);
        }
    } else
        printf("Memória de clientes vazia.\n");
    if (arrViatura!= NULL) {
        printf(BOLDCYAN"\nViaturas (Quantidade - %d):\n\n"RESET, sizeViatura);
        for(int i = 0; i < sizeViatura; ++i) {
            Tviatura viatura = arrViatura[i];
            printf(BOLDCYAN" ID:"RESET" %s\n "BOLDCYAN"Cor:"RESET" %s\n "BOLDCYAN"Marca:"RESET" %s\n "BOLDCYAN"Modelo:"RESET" %s\n "BOLDCYAN"Tipo:"RESET" %s\n "BOLDCYAN"Mudanças:"RESET" %d\n "BOLDCYAN"Matricula:"RESET" %s\n", viatura.ID, viatura.cor, viatura.marca, viatura.modelo, viatura.tipo, viatura.mudancas, viatura.matricula);
        }
    } else
        printf("Memória de viaturas vazia.\n");
}

void alterarUtilizador() {
    char s[50];
    char pass[20];
    char saldo[20], *end;
    printf(BOLDBLACK"\nIntroduza o nickname do utilizador a alterar: "RESET);
    fgets( s, 50, stdin);
    s[ strlen(s)-1 ] = 0;
    for(int i = 0; i < sizeCliente-1; ++i) {
        if (strcmp(arrCliente[i].nick,s)==0) {
            printf("Utilizador encontrado.\n");
            char j[20];
            while (1) {
                printf(BOLDBLACK"Deseja alterar a password? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    printf(BOLDBLACK"Digite a nova password: "RESET);
                    fgets(pass, 20, stdin);
                    pass[strlen(pass) - 1] = 0;
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar o saldo? (s/n): "RESET);
                fgets( j, 20, stdin);
                j[ strlen(j)-1 ] = 0;
                if (strcmp(j, "n")==0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    printf(BOLDBLACK"Digite o novo saldo: "RESET);
                    fgets(saldo, 20, stdin);
                    saldo[ strlen(saldo)-1 ] = 0;
                    break;
                }
            }
            semop(77981, &DOWN, 1);
            strcpy(arrCliente[i].pass, pass);
            arrCliente[i].saldo = (int) strtol(saldo, &end, 10);
            semop(77981, &UP, 1);
            printf("Alterações guardadas na memória com sucesso.\n");
            return;
        }
    }
    printf("Utilizador não encontrado.\n");
}

void alterarViatura() {
    char s[50];
    printf(BOLDBLACK"\nIntroduza o ID da viatura a alterar: "RESET);
    fgets( s, 50, stdin);
    s[ strlen(s)-1 ] = 0;
    for(int i = 0; i < sizeViatura-1; ++i) {
        if (strcmp(arrViatura[i].ID,s)==0) {
            printf("Viatura encontrada.\n");
            char j[20];
            while (1) {
                printf(BOLDBLACK"Deseja alterar a cor? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char cor[20];
                    printf(BOLDBLACK"Digite a nova cor: "RESET);
                    fgets(cor, 20, stdin);
                    cor[strlen(cor) - 1] = 0;
                    strcpy(arrViatura[i].cor, cor);
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar a marca? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char marca[30];
                    printf(BOLDBLACK"Digite a nova marca: "RESET);
                    fgets(marca, 30, stdin);
                    marca[strlen(marca) - 1] = 0;
                    strcpy(arrViatura[i].marca, marca);
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar o modelo? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char modelo[30];
                    printf(BOLDBLACK"Digite o novo modelo: "RESET);
                    fgets(modelo, 30, stdin);
                    modelo[strlen(modelo) - 1] = 0;
                    strcpy(arrViatura[i].modelo, modelo);
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar o tipo? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char tipo[30];
                    printf(BOLDBLACK"Digite o novo tipo: "RESET);
                    fgets(tipo, 30, stdin);
                    tipo[strlen(tipo) - 1] = 0;
                    strcpy(arrViatura[i].tipo, tipo);
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar o nº de mudanças? (s/n): "RESET);
                fgets( j, 20, stdin);
                j[ strlen(j)-1 ] = 0;
                if (strcmp(j, "n")==0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char mudancas[20], *end;
                    printf(BOLDBLACK"Digite o novo nº de mudanças: "RESET);
                    fgets(mudancas, 20, stdin);
                    mudancas[ strlen(mudancas)-1 ] = 0;
                    arrViatura[i].mudancas = (int) strtol(mudancas, &end, 10);
                    break;
                }
            }
            while (1) {
                printf(BOLDBLACK"Deseja alterar a matricula? (s/n): "RESET);
                fgets(j, 20, stdin);
                j[strlen(j) - 1] = 0;
                if (strcmp(j, "n") == 0)
                    break;
                else if (strcmp(j, "s") == 0) {
                    char matricula[30];
                    printf(BOLDBLACK"Digite a nova matricula: "RESET);
                    fgets(matricula, 30, stdin);
                    matricula[strlen(matricula) - 1] = 0;
                    strcpy(arrViatura[i].matricula, matricula);
                    break;
                }
            }
            printf("Alterações guardadas na memória com sucesso.\n");
            return;
        }
    }
    printf("Viatura não encontrada.\n");
}

int guardarDadosCliente() {
    if (arrCliente != NULL) {
        FILE *filebin = fopen("utilizadores.dat", "wb"); //usar "utilizadores.dat" se o ficheiro compilado estiver no mesmo sitio do .dat
        FILE *filetxt = fopen("utilizadores.txt", "wb"); //usar "utilizadores.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
        printf("A guardar %d clientes.\n", sizeCliente);
        for (int i = 0; i < sizeCliente; ++i) {
            Tcliente cliente = arrCliente[i];
            fprintf(filetxt, "%s;%s;%d;%s;%s;%s;%d\n", cliente.nick, cliente.pass, cliente.id, cliente.nome,
                    cliente.email, cliente.turma, cliente.saldo);
            fwrite(&cliente, sizeof(Tcliente), 1, filebin);
        }
        fclose(filebin);
        fclose(filetxt);
        return 1;
    } else
        printf("Memória de clientes vazia.\n");
    return 0;
}

int guardarDadosViatura() {
    if (arrViatura != NULL) {
        FILE *filebin = fopen("viaturas.dat", "wb"); //usar "viaturas.dat" se o ficheiro compilado estiver no mesmo sitio do .dat
        FILE *filetxt = fopen("viaturas.txt", "wb"); //usar "viaturas.txt" se o ficheiro compilado estiver no mesmo sitio do .txt
        printf("A guardar %d viaturas.\n", sizeViatura);
        for (int i = 0; i < sizeViatura; ++i) {
            Tviatura viatura = arrViatura[i];
            fprintf(filetxt, "%s;%s;%s;%s;%s;%d;%s", viatura.ID, viatura.cor, viatura.marca, viatura.modelo, viatura.tipo, viatura.mudancas, viatura.matricula);
            fwrite( &viatura, sizeof(Tviatura), 1, filebin);
        }
        fclose(filebin);
        fclose(filetxt);
        return 1;
    } else
        printf("Memória de viaturas vazia.\n");
    return 0;
}

void guardarDados() {
    if ( guardarDadosCliente() && guardarDadosViatura() )
        printf("Dados guardados com sucesso.\n");
}

void viaturasDisponiveis() {
    for (int i=0; i<sizeViatura; i++) {
        if (arrViatura[i].available) {
            printf("%s\n", arrViatura[i].marca);
            //TODO calcular tempo alugado
        }
    }
}

void viaturasNaoDisponiveis() {
    for (int i=0; i<sizeViatura; i++) {
        if (!arrViatura[i].available) {
            printf("%s\n", arrViatura[i].marca);
            //TODO calcular tempo alugado
        }
    }
}

long getTimeSecs() {
    long            s;  // Seconds
    struct timespec spec;
    
    clock_gettime(CLOCK_MONOTONIC, &spec);
    
    s  = spec.tv_sec;
    
    return s;
}

int main() {
    semaforos = semget(77981, 5, 0);
    
    int idV = shmget( 77981, sizeof(Tviatura)*200, IPC_CREAT | 0666 );
    arrViatura = (Tviatura *)shmat(idV, 0, 0);
    
    int idC = shmget( 77561, sizeof(Tcliente)*200, IPC_CREAT | 0666 );
    arrCliente = (Tcliente *)shmat(idC, 0, 0);
    
    printf("time: %ld seconds\n", getTimeSecs());
    
    printf(BOLDCYAN"\nPrograma administrador\n\n");
    printf("1"RESET" - Ler dados para memória\n");
    printf(BOLDCYAN"2"RESET" - Imprimir memória\n");
    printf(BOLDCYAN"3"RESET" - Alterar utilizador\n");
    printf(BOLDCYAN"4"RESET" - Alterar viatura\n");
    printf(BOLDCYAN"5"RESET" - Guardar dados\n");
    printf(BOLDCYAN"6"RESET" - Viaturas disponiveis\n");
    printf(BOLDCYAN"7"RESET" - Viaturas ocupadas\n");
    printf(BOLDCYAN"sair"RESET" - Sair\n");

    char s[10], *end;
    while (1) {
        printf(BOLDBLACK"\nEscolha um menu: "RESET);
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
                alterarViatura();
                break;

            case 5 :
                guardarDados();
                break;
                
            case 6 :
                viaturasDisponiveis();
                break;
                
            case 7 :
                viaturasNaoDisponiveis();
                break;

            default :
                if (strcmp(end, "sair")==0){
                    exit(0);
                }
                printf("Escolha uma opção válida.\n");
        }
    }
}
