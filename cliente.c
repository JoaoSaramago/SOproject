#include "defines.h"

void printToScreen(char* string){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    printf("%d-%02d-%02d  %02d:%02d:%02d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, string);
}

void closeHandler(int signal){
    printToScreen("O tempo de reserva terminou.");
    exit(0);
}

void balanceHandler(int signal){
    printToScreen("Saldo insuficiente. Regresse assim que possível.");
    exit(0);
}

void listVehicles(){
    MsgClientServer tempMessage;
    tempMessage.type = 1;
}

int main(){

    printf("PID: %d\n", getpid());

    signal(SIGUSR1, closeHandler);
    signal(SIGUSR2, balanceHandler);
    
    //Connect to message queue
    int idM = msgget(77561, 0);
    exit_on_error(idM, "msgget");
    
    printf(BOLDCYAN"\nPrograma cliente\n\n");
    
    char nick[20];
    printf(BOLDBLACK"\nNickname: "RESET);
    fgets( nick, 20, stdin);
    nick[ strlen(nick)-1 ] = 0;
    
    char pass[20];
    printf(BOLDBLACK"\nPassword: "RESET);
    fgets( pass, 20, stdin);
    pass[ strlen(pass)-1 ] = 0;
    
    MsgClientServer loginMessage;
    loginMessage.type = 1;
    loginMessage.data.msgType = LOGIN;
    strcpy(loginMessage.data.info1, nick);
    strcpy(loginMessage.data.info2, pass);
    loginMessage.data.myid = getpid();
    msgsnd(idM, &loginMessage, sizeof(loginMessage.data), 0);
    MsgServerClient tempMessage;
    msgrcv(idM, &tempMessage, sizeof(tempMessage.data), getpid(), 0);
    
    printf("1"RESET" - Listar viaturas disponíveis\n");
    printf(BOLDCYAN"2"RESET" - Iniciar reserva\n");
    printf(BOLDCYAN"3"RESET" - Iniciar aluguer\n");
    printf(BOLDCYAN"4"RESET" - Terminar pedido\n");
    printf(BOLDCYAN"5"RESET" - Adicionar saldo\n");
    printf(BOLDCYAN"6"RESET" - Ver saldo\n");
    printf(BOLDCYAN"sair"RESET" - Sair\n");
    
    char s[10], *end;
    while (1) {
        printf(BOLDBLACK"\nEscolha um menu: "RESET);
        fgets( s, 10, stdin);
        s[ strlen(s)-1 ] = 0;
        long menuOption = strtol(s, &end, 10);
        switch (menuOption) {
                
            case 1 :
                listVehicles();
                break;
                
            case 2 :
                
                break;
                
            case 3 :
                
                break;
                
            case 4 :
                
                break;
                
            case 5 :
                
                break;
                
            case 6 :
                
                break;
                
            default :
                if (strcmp(end, "sair")==0){
                    exit(0);
                }
                printf("Escolha uma opção válida.\n");
        }
    }
}
