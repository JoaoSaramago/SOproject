#include "defines.h"

Tcliente* arrCliente;
Tviatura* arrViatura;

int idV;
int idC;
int idM;

int pid;

int semaphores;

void printToScreen(char* string, int clientID){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
    char buffer[100];
    sprintf(buffer, string, clientID);
	printf("%d-%d-%d  %d:%d:%d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, buffer);
	FILE *file = fopen("servidor.log", "ab");
	fprintf(file, "%d-%d-%d  %d:%d:%d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, buffer);
	fclose(file);
}

void closeHandlerFather(){
	remove("servidor.pid");
	kill(pid, SIGINT);
	printToScreen("Father closing.",0);
	exit(0);
}

void closeHandlerChild(){
    printToScreen("Fiscal is closing.",0);
	exit(0);
}

void alarmHandlerChild(){
	alarm(60);
	printToScreen("Fiscal is verifying the data.",0);
}

void login(char* nickname, char* password, int clientPid){
	printToScreen("Client with pid %d trying to log in...", clientPid);
    MsgServerClient tempMessage;
    tempMessage.type = clientPid;
    tempMessage.data.status = FAIL;
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
        if (strcmp(arrCliente[i].nick, nickname) == 0) {
            if(strcmp(arrCliente[i].pass, password) == 0){
                printToScreen("Client with pid %d logged in.", clientPid);
				semop(77981, &CDOWN, 1);
                arrCliente[i].pid = clientPid;
                tempMessage.data.value1 = arrCliente[i].id;
                arrCliente[i].online = clientPid; //Client online
				semop(77981, &CUP, 1);
                tempMessage.data.status = SUCCESS;
            }
        }
        i++;
    }
    msgsnd(idM, &tempMessage, sizeof(tempMessage.data), clientPid);
}

void vehicleToString(char* string, int i){
    strcpy(string, arrViatura[i].ID);
    strcat(string, ";");
    strcat(string, arrViatura[i].cor);
    strcat(string, ";");
    strcat(string, arrViatura[i].marca);
    strcat(string, ";");
    strcat(string, arrViatura[i].modelo);
    strcat(string, ";");
    strcat(string, arrViatura[i].tipo);
    strcat(string, ";");
    char buffer[33];
    sprintf(buffer, "%d", arrViatura[i].mudancas);
    strcat(string, buffer);
    strcat(string, ";");
    strcat(string, arrViatura[i].matricula);
}

void listVehicles(int clientID){
	printToScreen("Sending vehicle list to client %d.", clientID);
    MsgServerClient tempMessage;
    tempMessage.type = clientID;
    int i = 0;
    while (arrViatura[i].mudancas != -1) {
        if (arrViatura[i].status == AVAILABLE) {
            vehicleToString(tempMessage.data.text, i);
            tempMessage.data.status = SENDING;
            msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
        }
        i++;
    }
    tempMessage.data.status = ENDLIST;
    msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
}

void reservar(int clientID, char* vehicleID){
    printToScreen("Reservar - Cliente %d a reservar um veiculo.", clientID);
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            if(arrCliente[i].saldo > 0 && !arrCliente[i].hasVehicle){
                int j = 0;
                while (arrViatura[j].mudancas != -1) { //percorrer viaturas
                    if(strcmp(arrViatura[j].ID, vehicleID) == 0 && arrViatura[j].status == AVAILABLE) {
						semop(77981, &VDOWN, 1);
                        arrViatura[j].status = RESERVED;
                        arrViatura[j].clientID = clientID;
                        arrViatura[j].timeStarted = getTimeSecs();
                        arrViatura[j].clientIndex = i;
						semop(77981, &VUP, 1);
                        arrCliente[i].hasVehicle = 1;
                        tempMessage.data.status = SUCCESS;
                        msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
                        return;
                    }
                    j++;
                }
                printToScreen("Vehicle not found",0);
            }
            tempMessage.data.status = FAIL;
            msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
			return;
        }
        i++;
    }
    printToScreen("Reservar - Client %d not found", clientID);
}

void alugar(int clientID, char* vehicleID){
    printToScreen("Client %d a tentar alugar um veiculo.", clientID);
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            if(arrCliente[i].saldo > 0){
                int j = 0;
                while (arrViatura[j].mudancas != -1) { //percorrer viaturas
                    if(strcmp(arrViatura[j].ID, vehicleID) == 0 && ((arrViatura[j].status == AVAILABLE && !arrCliente[i].hasVehicle) || (arrViatura[j].status == RESERVED && arrViatura[j].clientID == clientID))) { //verifica se o ID recebido é igual ao da viatura atual e se a viatura esta disponivel ou se está reservada pelo cliente que a pretende alugar
                        semop(77981, &VDOWN, 1);
                        arrViatura[j].status = RENTED;
                        arrViatura[j].clientID = clientID;
                        arrViatura[j].timeStarted = getTimeSecs();
                        arrViatura[j].clientIndex = i;
                        semop(77981, &VUP, 1);
                        arrCliente[i].hasVehicle = 1;
                        tempMessage.data.status = SUCCESS;
                        msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
                        return;
                    }
                    j++;
                }
                printToScreen("Vehicle not found",0);
            }
            tempMessage.data.status = FAIL;
            msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
            return;
        }
        i++;
    }
    printToScreen("Alugar - Client %d not found", clientID);
}

void finalizar(int clientID){
    printToScreen("Cliente %d a tentar finalizar um aluguer", clientID);
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            tempMessage.data.status = FAIL;
            int j = 0;
            while (arrViatura[j].mudancas != -1) { //percorrer viaturas
                semop(77981, &VDOWN, 1);
                if((arrViatura[j].status == RESERVED || arrViatura[j].status == RENTED) && arrViatura[j].clientID == clientID)  {
					arrViatura[j].timeStarted = -1;
                    arrViatura[j].status = AVAILABLE;
					semop(77981, &VUP, 1);
                    tempMessage.data.status = SUCCESS;
                    break;
                } else {
                    semop(77981, &VUP, 1);
                }
                j++;
            }
            arrCliente[i].hasVehicle = 0;
            msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
            return;
        }
        i++;
    }
	printToScreen("Finalizar - Client %d not found", clientID);
}

void saldo(int clientID){
	printToScreen("Saldo - Cliente %d a pedir o seu saldo.", clientID);
	MsgServerClient tempMessage;
	tempMessage.type = clientID;
    tempMessage.data.status = FAIL;
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
		semop(77981, &CDOWN, 1);
		if(arrCliente[i].id == clientID) {
            tempMessage.data.status = SUCCESS;
            tempMessage.data.value1 = arrCliente[i].saldo;
			semop(77981, &CUP, 1);
			msgsnd(idM, &tempMessage, sizeof(tempMessage.data), clientID);
			return;
		} else {
			semop(77981, &CUP, 1);
		}
        i++;
	}
	printToScreen("Saldo - User not found!",0);
    msgsnd(idM, &tempMessage, sizeof(tempMessage.data), clientID);
}

void carregarSaldo(int clientID, char* moneyToAddChar){
	printToScreen("Carregar - Cliente %d a tentar carregar saldo.", clientID);
    int moneyToAdd = atoi(moneyToAddChar);
    MsgServerClient tempMessage;
    tempMessage.type = clientID;
    tempMessage.data.status = FAIL;
    if (moneyToAdd > 0) {
        int i = 0;
        while (arrCliente[i].id != -1) { //percorrer clientes
            if(arrCliente[i].id == clientID) {
                semop(77981, &CDOWN, 1);
                tempMessage.data.value1 = arrCliente[i].saldo;
                arrCliente[i].saldo += moneyToAdd;
                tempMessage.data.value2 = arrCliente[i].saldo;
                semop(77981, &CUP, 1);
                tempMessage.data.status = SUCCESS;
                break;
            }
            i++;
        }
    }
    msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
}

void logout(int clientID){
    printToScreen("Logout - Cliente %d terminou a sessão.", clientID);
    int i = 0;
    while (arrCliente[i].id != -1) { //percorrer clientes
        if(arrCliente[i].id == clientID) {
            arrCliente[i].online = -1;
        }
        i++;
    }
}



int main(){
    
    //Connect to semaphores
    semaphores = semget(77981, 5, 0);
    //Connect to Vehicle shared memory
    idV = shmget(77981, sizeof(Tviatura)*200, 0);
    exit_on_error(idV, "Vehicle shmget");
    arrViatura = (Tviatura*) shmat(idV, 0, 0);
    //Connect to Client shared memory
    idC = shmget(77561, sizeof(Tcliente)*200, 0);
    exit_on_error(idC, "Client shmget");
    arrCliente = (Tcliente*) shmat(idC, 0, 0);
    //Connect to message queue
    idM = msgget(77561, 0666 | IPC_CREAT);
    exit_on_error(idM, "msgget");
    
	printToScreen("PID: %d", getpid());
	
	FILE *pidFile = fopen("servidor.pid", "w");
	FILE *logFile = fopen("servidor.log", "a");

	if(pidFile == NULL){
		printToScreen("PID File - Error opening file!\n",0);
		exit(1);
	}
    
	fprintf(pidFile,"PID: %d\n", getpid() );
	
	fclose(pidFile);
    fclose(logFile);
	
	pid = fork();
	
	if(pid==0){ //Child (Fiscal)
		signal(SIGINT, closeHandlerChild);
		signal(SIGALRM, alarmHandlerChild);
		alarm(60);
        while(1){
            int i = 0;
            while (arrViatura[i].mudancas != -1) { //percorrer viaturas
				semop(77981, &VDOWN, 1);
                if(arrViatura[i].status == RESERVED && ((getTimeSecs() - arrViatura[i].timeStarted)/60 >= 5)){
					arrViatura[i].status = AVAILABLE;
                    arrViatura[i].timeStarted = -1;
					semop(77981, &CDOWN, 1);
                    if(arrCliente[arrViatura[i].clientIndex].online) {		//Check to see if the client is online before sending a signal. The client shouldn't ever be offline when he has a reservation, but it's nice to check anyway
                        arrCliente[arrViatura[i].clientIndex].hasVehicle = 0;
                        kill(arrCliente[arrViatura[i].clientIndex].pid, SIGUSR1);
						printToScreen("Cliente %d ultrapassou os 5 minutos!", arrCliente[arrViatura[i].clientIndex].id);
					}
					semop(77981, &CUP, 1);
					semop(77981, &VUP, 1);
                }
				
				semop(77981, &VDOWN, 1);
				semop(77981, &CDOWN, 1);
                if ( arrViatura[i].status == RESERVED || arrViatura[i].status == RENTED ) {
                    if(arrCliente[arrViatura[i].clientIndex].saldo - 1 < 0){
                        printToScreen("Cliente %d excedeu o seu saldo.", arrCliente[arrViatura[i].clientIndex].online);
                        if(arrCliente[arrViatura[i].clientIndex].online != -1)
                            kill(arrCliente[arrViatura[i].clientIndex].pid, SIGUSR2);
                    } else {
                        arrCliente[arrViatura[i].clientIndex].saldo--;
                    }
                }
				semop(77981, &CUP, 1);
				semop(77981, &VUP, 1);
                i++;
            }
            pause();
        }
	}
	
	if(pid>0){ //Father
        
		signal(SIGINT, closeHandlerFather);
        
        MsgClientServer tempMessage;
        
        while (1) {
            int status = msgrcv(idM, &tempMessage, sizeof(tempMessage.data), 1, 0);
            switch (tempMessage.data.msgType) {
                case LOGIN:
                    login(tempMessage.data.info1, tempMessage.data.info2, tempMessage.data.myid);
                    break;
                    
                case VIATURAS:
                    listVehicles(tempMessage.data.myid);
                    break;
                    
                case RESERVAR:
                    reservar(tempMessage.data.myid, tempMessage.data.info1);
                    break;
                    
                case ALUGAR:
                    alugar(tempMessage.data.myid, tempMessage.data.info1);
                    break;
                    
                case FINALIZAR:
                    finalizar(tempMessage.data.myid);
                    break;
                    
                case SALDO:
                    saldo(tempMessage.data.myid);
                    break;
                    
                case CARREGAR:
                    carregarSaldo(tempMessage.data.myid, tempMessage.data.info1);
                    break;
                    
                case LOGOUT:
                    logout(tempMessage.data.myid);
                    break;
                    
                default:
                    break;
            }
        }
	}
}
                                                                                         
