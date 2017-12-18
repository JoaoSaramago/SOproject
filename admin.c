#include "defines.h"

int sizeCliente = 0;
Tcliente* arrCliente;

int sizeViatura = 0;
Tviatura* arrViatura;

int semaforos;

//coloca os clientes em memoria a partir da leitura de um ficheiro, se os clientes lidos já existirem na memoria estes não são modificados, apenas apaga da memoria os que já nao existem no ficheiro ou acrescenta os que forem novos
void lerDadosParaMemoriaCliente(){
    int sizeArrNew = 0;
    int sizeArrFinal = 0;
    Tcliente tempArrNew[200];
    Tcliente tempArrFinal[200];
    
    FILE *file;
    if ((file = fopen("utilizadores.dat", "r")) == NULL) {
        if ((file = fopen("utilizadores.txt", "r")) == NULL) {
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
            cliente.hasVehicle = 0;
            tempArrNew[sizeArrNew] = cliente; // mete todos os clientes lidos do .txt num array temporario
            sizeArrNew++;
        }
    } else {
        printf("A ler clientes de clientes.dat");
        while (fread(&tempArrNew[sizeArrNew], sizeof(Tcliente), 1, file) > 0) { // mete todos os clientes lidos do .dat num array temporario
            sizeArrNew++;
        }
    }
    fclose(file);
    for (int i = 0; i<sizeArrNew; i++) {
        int stop = 0;
        for (int j = 0; j<sizeCliente; j++) {
            if (tempArrNew[i].id == arrCliente[j].id) { // se no novo array temporario houver um cliente igual a um já existente entao copia se o antigo para um outro array temporario final
                tempArrFinal[sizeArrFinal] = arrCliente[j];
                stop = 1;
                break;
            }
        }
        if (stop == 0) { // se o cliente no novo array ainda nao existe no array antigo, entao copia se este para o array temporario final
            tempArrFinal[sizeArrFinal] = tempArrNew[i];
            printf(".");
        }
        sizeArrFinal++;
    }
    for (int k = 0; k<200; k++) {
        if (k<sizeArrFinal) {
            arrCliente[k] = tempArrFinal[k]; //copia se o array temporario final para a nossa memoria partilhada
        } else
            arrCliente[k].id = -1; //mete se os restantes espaços como "apagados"
    }
    sizeCliente = sizeArrFinal;
}

//coloca as viaturas em memoria a partir da leitura de um ficheiro, se as viaturas lidos já existirem na memoria estes não são modificados, apenas apaga da memoria as que já nao existem no ficheiro ou acrescenta as que forem novas
void lerDadosParaMemoriaViatura() {
    int sizeArrNew = 0;
    int sizeArrFinal = 0;
    Tviatura tempArrNew[200];
    Tviatura tempArrFinal[200];
    
    FILE *file;
    if ((file = fopen("viaturas.dat", "r")) == NULL) {
        if ((file = fopen("viaturas.txt", "r")) == NULL) {
            printf("Nao foi possivel abrir o ficheiro com as viaturas.\n");
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
            viatura.status = AVAILABLE;
            viatura.timeStarted = -1;
            tempArrNew[sizeArrNew] = viatura; // mete todas as viaturas lidas do .txt num array temporario
            sizeArrNew++;
        }
    } else {
        printf("A ler viaturas de viaturas.dat");
        while (fread(&tempArrNew[sizeArrNew], sizeof(Tviatura), 1, file) > 0) { // mete todas as viaturas lidas do .dat num array temporario
            sizeArrNew++;
        }
    }
    fclose(file);
    for (int i = 0; i<sizeArrNew; i++) {
        int stop = 0;
        for (int j = 0; j<sizeViatura; j++) {
            if (strcmp(tempArrNew[i].ID, arrViatura[j].ID) == 0) { // se no novo array temporario houver uma viatura igual a uma já existente entao copia se a antiga para um outro array temporario final
                tempArrFinal[sizeArrFinal] = arrViatura[j];
                stop = 1;
                break;
            }
        }
        if (stop == 0) { // se a viatura no novo array ainda nao existe no array antigo, entao copia se esta para o array temporario final
            tempArrFinal[sizeArrFinal] = tempArrNew[i];
            printf(".");
        }
        sizeArrFinal++;
    }
    for (int k = 0; k<200; k++) {
        if (k<sizeArrFinal) {
            arrViatura[k] = tempArrFinal[k]; //copia se o array temporario final para a nossa memoria partilhada
        } else
            arrViatura[k].mudancas = -1; //mete se os restantes espaços como "apagados"
    }
    sizeViatura = sizeArrFinal;
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
    printf(BOLDBLACK"\nIntroduza o nickname do utilizador a alterar: "RESET);
    fgets( s, 50, stdin);
    s[ strlen(s)-1 ] = 0;
    for(int i = 0; i < sizeCliente; ++i) {
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
                    char pass[20];
                    printf(BOLDBLACK"Digite a nova password: "RESET);
                    fgets(pass, 20, stdin);
                    pass[strlen(pass) - 1] = 0;
                    semop(77981, &CDOWN, 1);
                    strcpy(arrCliente[i].pass, pass);
                    semop(77981, &CUP, 1);
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
                    char saldo[20], *end;
                    printf(BOLDBLACK"Digite o novo saldo: "RESET);
                    fgets(saldo, 20, stdin);
                    saldo[ strlen(saldo)-1 ] = 0;
                    semop(77981, &VDOWN, 1);
                    arrCliente[i].saldo = (int) strtol(saldo, &end, 10);
                    semop(77981, &VUP, 1);
                    break;
                }
            }
            printf("Alterações guardadas na memória com sucesso.\n");
            return;
        }
    }
    printf("Utilizador não encontrado.\n");
}

void alterarViatura() {
    
    struct sembuf DOWN = { 1, -1, 0};
    struct sembuf UP = { 1, 1, 0};
    
    char s[50];
    printf(BOLDBLACK"\nIntroduza o ID da viatura a alterar: "RESET);
    fgets( s, 50, stdin);
    s[ strlen(s)-1 ] = 0;
    for(int i = 0; i < sizeViatura; ++i) {
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
                    semop(77981, &DOWN, 1);
                    strcpy(arrViatura[i].cor, cor);
                    semop(77981, &UP, 1);
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
                    semop(77981, &DOWN, 1);
                    strcpy(arrViatura[i].marca, marca);
                    semop(77981, &UP, 1);
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
                    semop(77981, &DOWN, 1);
                    strcpy(arrViatura[i].modelo, modelo);
                    semop(77981, &UP, 1);
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
                    semop(77981, &DOWN, 1);
                    strcpy(arrViatura[i].tipo, tipo);
                    semop(77981, &UP, 1);
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
                    semop(77981, &DOWN, 1);
                    arrViatura[i].mudancas = (int) strtol(mudancas, &end, 10);
                    semop(77981, &UP, 1);
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
                    semop(77981, &DOWN, 1);
                    strcpy(arrViatura[i].matricula, matricula);
                    semop(77981, &UP, 1);
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
        FILE *filebin = fopen("utilizadores.dat", "wb");
        FILE *filetxt = fopen("utilizadores.txt", "wb");
        printf("A guardar %d clientes", sizeCliente);
        for (int i = 0; i < sizeCliente; ++i) {
            Tcliente cliente = arrCliente[i];
            fprintf(filetxt, "%s;%s;%d;%s;%s;%s;%d\n", cliente.nick, cliente.pass, cliente.id, cliente.nome,
                    cliente.email, cliente.turma, cliente.saldo);
            fwrite(&cliente, sizeof(Tcliente), 1, filebin);
            printf(".");
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
        FILE *filebin = fopen("viaturas.dat", "wb");
        FILE *filetxt = fopen("viaturas.txt", "wb");
        printf("\nA guardar %d viaturas", sizeViatura);
        for (int i = 0; i < sizeViatura; ++i) {
            Tviatura viatura = arrViatura[i];
            fprintf(filetxt, "%s;%s;%s;%s;%s;%d;%s", viatura.ID, viatura.cor, viatura.marca, viatura.modelo, viatura.tipo, viatura.mudancas, viatura.matricula);
            fwrite( &viatura, sizeof(Tviatura), 1, filebin);
            printf(".");
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
        printf("\nDados guardados com sucesso.\n");
}

void viaturasDisponiveis() {
    printf("Viaturas disponiveis:\n");
    for (int i=0; i<sizeViatura; i++) {
        if (arrViatura[i].status == AVAILABLE) {
            printf("%s\n", arrViatura[i].ID);
        }
    }
}

void viaturasNaoDisponiveis() {
    printf("Viaturas ocupadas:\n");
    for (int i=0; i<sizeViatura; i++) {
        if (!(arrViatura[i].status == AVAILABLE)) {
            long min = (getTimeSecs()-arrViatura[i].timeStarted)/60;
            long s = (getTimeSecs()-arrViatura[i].timeStarted) - (min*60);
            printf("ID viatura: %s, ocupada à %ld min e %ld s.\n", arrViatura[i].ID, min, s);
        }
    }
}

int main() {
    
    printf(BOLDCYAN"\nPrograma administrador\n"RESET);
    
    semaforos = semget(77981, 5, 0);
    
    int idV = shmget( 77981, sizeof(Tviatura)*200, 0);
    if (idV < 0) { // se a memoria partilhada ainda nao existe
        printf("A criar memoria partilhada para as viaturas...\n");
        idV = shmget( 77981, sizeof(Tviatura)*200, IPC_CREAT | 0666 );
        arrViatura = (Tviatura *)shmat(idV, 0, 0);
        // "reset" as viaturas
        for (int i=0; i<200; i++) {
            arrViatura[i].mudancas = -1;
        }
    } else { // se a memoria partilhada já existe, saber o tamanho dela
        arrViatura = (Tviatura *)shmat(idV, 0, 0);
        int i = 0;
        while (arrViatura[i].mudancas != -1) {
            i++;
        }
        sizeViatura = i;
    }
    
    int idC = shmget( 77561, sizeof(Tcliente)*200, 0);
    if (idC < 0) { // se a memoria partilhada ainda nao existe
        printf("A criar memoria partilhada para os clientes...\n");
        idC = shmget( 77561, sizeof(Tcliente)*200, IPC_CREAT | 0666 );
        arrCliente = (Tcliente *)shmat(idC, 0, 0);
        // "reset" aos clientes
        for (int i=0; i<200; i++) {
            arrCliente[i].id = -1;
        }
    } else { // se a memoria partilhada já existe, saber o tamanho dela
        arrCliente = (Tcliente *)shmat(idC, 0, 0);
        int i = 0;
        while (arrCliente[i].id != -1) {
            i++;
        }
        sizeCliente = i;
    }

    char s[10], *end;
    while (1) {
        // as opcoes do menu aparecem sempre que se escolhe um novo menu para nao ter de voltar ao topo a procura de quais sao os menus existentes
        printf(BOLDCYAN"\nMenu:");
        printf("\n--------------------------");
        printf("\n1"RESET" - Ler dados para memória\n");
        printf(BOLDCYAN"2"RESET" - Imprimir memória\n");
        printf(BOLDCYAN"3"RESET" - Alterar utilizador\n");
        printf(BOLDCYAN"4"RESET" - Alterar viatura\n");
        printf(BOLDCYAN"5"RESET" - Guardar dados\n");
        printf(BOLDCYAN"6"RESET" - Viaturas disponiveis\n");
        printf(BOLDCYAN"7"RESET" - Viaturas ocupadas\n");
        printf(BOLDCYAN"sair"RESET" - Sair\n");
        printf(BOLDCYAN"--------------------------\n"RESET);
        
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
