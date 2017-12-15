#include "defines.h"

int thisid;
int idM;

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
    printf("Showing available vehicles...\n\n");
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = VIATURAS;
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    do {
        msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
        if (receivedMsg.data.status == SENDING) {
            char *end;
            
            printf(BOLDCYAN" ID:"RESET" %s\n "BOLDCYAN"Cor:"RESET" %s\n "BOLDCYAN"Marca:"RESET" %s\n "BOLDCYAN"Modelo:"RESET" %s\n "BOLDCYAN"Tipo:"RESET" %s\n "BOLDCYAN"Mudanças:"RESET" %d\n "BOLDCYAN"Matricula:"RESET" %s\n", strtok (receivedMsg.data.text,";"), strtok (NULL,";"), strtok (NULL,";"), strtok (NULL,";"), strtok (NULL,";"), (int) strtol(strtok(NULL, ";"), &end, 10), strtok (NULL,";"));
            
        }
    } while (!(receivedMsg.data.status == ENDLIST));
}

void reserve() {
    char vehicle[20];
    printf(BOLDBLACK"\nVehicle ID to reserve: "RESET);
    fgets( vehicle, 20, stdin);
    vehicle[ strlen(vehicle)-1 ] = 0;
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = RESERVAR;
    strcpy(sendMsg.data.info1, vehicle);
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
    if (receivedMsg.data.status == SUCCESS)
        printf("Vehicle reserved successefully.\n");
    else
        printf("It was not possible to reserve the vehicle.\n");
}

void rent() {
    char vehicle[20];
    printf(BOLDBLACK"\nVehicle ID to rent: "RESET);
    fgets( vehicle, 20, stdin);
    vehicle[ strlen(vehicle)-1 ] = 0;
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = ALUGAR;
    strcpy(sendMsg.data.info1, vehicle);
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
    if (receivedMsg.data.status == SUCCESS)
        printf("Vehicle rented successefully.\n");
    else
        printf("It was not possible to rent the vehicle.\n");
}

void finalize() {
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = FINALIZAR;
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
    if (receivedMsg.data.status == SUCCESS)
        printf("Renting or reserve finalized.\n");
    else
        printf("Nothing to finalize.\n");
}

void addMoney() {
    char money[20];
    printf(BOLDBLACK"\nMoney to add: "RESET);
    fgets( money, 20, stdin);
    money[ strlen(money)-1 ] = 0;
    // TODO verificar se é int
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = CARREGAR;
    strcpy(sendMsg.data.info1, money);
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
    if (receivedMsg.data.status == SUCCESS)
        printf("Money add to balance successefully.\nBefore: %d\nAfter: %d\n", receivedMsg.data.value1, receivedMsg.data.value2);
    else
        printf("It was not possible to add the money to your balance.\n");
}

void getBalance() {
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = SALDO;
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    
    MsgServerClient receivedMsg;
    msgrcv(idM, &receivedMsg, sizeof(receivedMsg.data), thisid, 0);
    if (receivedMsg.data.status == SUCCESS)
        printf("Your balance: %d.\n", receivedMsg.data.value1);
    else
        printf("It was not possible to get your balance.\n");
}

void logout() {
    MsgClientServer sendMsg;
    sendMsg.type = 1;
    sendMsg.data.msgType = LOGOUT;
    sendMsg.data.myid = thisid;
    msgsnd(idM, &sendMsg, sizeof(sendMsg.data), 0);
    exit(0);
}

int main(){

    printf("PID: %d\n", getpid());

    signal(SIGUSR1, closeHandler);
    signal(SIGUSR2, balanceHandler);
    
    //Connect to message queue
    idM = msgget(77561, 0);
    exit_on_error(idM, "msgget");
    
    printf(BOLDCYAN"\nPrograma cliente\n");
    
    MsgServerClient tempMessage;
    do {
        char nick[20];
        printf(BOLDBLACK"\nNickname: "RESET);
        fgets( nick, 20, stdin);
        nick[ strlen(nick)-1 ] = 0;
        
        char pass[20];
        printf(BOLDBLACK"Password: "RESET);
        fgets( pass, 20, stdin);
        pass[ strlen(pass)-1 ] = 0;
        
        MsgClientServer loginMessage;
        loginMessage.type = 1;
        loginMessage.data.msgType = LOGIN;
        strcpy(loginMessage.data.info1, nick);
        strcpy(loginMessage.data.info2, pass);
        loginMessage.data.myid = getpid();
        msgsnd(idM, &loginMessage, sizeof(loginMessage.data), 0);
        
        msgrcv(idM, &tempMessage, sizeof(tempMessage.data), getpid(), 0);
        if (tempMessage.data.status == FAIL) {
            printf("Nickname or password incorrect.\n");
        }
    } while (tempMessage.data.status == FAIL);
    
    thisid = tempMessage.data.value1;
    printf("\nClientID: %d\n", thisid);
    
    
    printf(BOLDCYAN"\nPrograma administrador");
    
    char s[10], *end;
    while (1) {
        printf(BOLDCYAN"\nMenu:");
        printf(BOLDCYAN"\n-------------------------------");
        printf(BOLDCYAN"\n1"RESET" - Listar viaturas disponíveis\n");
        printf(BOLDCYAN"2"RESET" - Iniciar reserva\n");
        printf(BOLDCYAN"3"RESET" - Iniciar aluguer\n");
        printf(BOLDCYAN"4"RESET" - Terminar pedido\n");
        printf(BOLDCYAN"5"RESET" - Adicionar saldo\n");
        printf(BOLDCYAN"6"RESET" - Ver saldo\n");
        printf(BOLDCYAN"7"RESET" - Sair\n");
        printf(BOLDCYAN"-------------------------------\n"RESET);
        printf(BOLDBLACK"\nEscolha um menu: "RESET);
        fgets( s, 10, stdin);
        s[ strlen(s)-1 ] = 0;
        long menuOption = strtol(s, &end, 10);
        switch (menuOption) {
                
            case 1 :
                listVehicles();
                break;
                
            case 2 :
                reserve();
                break;
                
            case 3 :
                rent();
                break;
                
            case 4 :
                finalize();
                break;
                
            case 5 :
                addMoney();
                break;
                
            case 6 :
                getBalance();
                break;
                
            case 7 :
                logout();
                break;
                
            default :
                printf("Escolha uma opção válida.\n");
        }
    }
}
