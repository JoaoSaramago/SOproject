#include "defines.h"

Tcliente* arrCliente;
Tviatura* arrViatura;

int idV;
int idC;
int idM;

int pid;

int semaphores;

void printToScreen(char* string){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	printf("%d-%d-%d  %d:%d:%d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, string);
	FILE *file = fopen("servidor.log", "ab");
	fprintf(file, "%d-%d-%d  %d:%d:%d - %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, string);
	fclose(file);
}

void closeHandlerFather(){
	remove("servidor.pid");
	kill(pid, SIGINT);
	printToScreen("Father closing.");
	exit(0);
}

void closeHandlerChild(){
    printToScreen("Fiscal is closing.");
	exit(0);
}

void alarmHandlerChild(){
	alarm(60);
	printToScreen("Fiscal is verifying the data.");
}

void login(char* nickname, char* password, int clientPid){
	printToScreen("Client with pid %d trying to log in...\n", clientPid);
    MsgServerClient tempMessage;
    tempMessage.type = clientPid;
    tempMessage.data.status = FAIL;
    for(int i = 0; i < 200; i++) { // TODO verificar tamanho
        if (strcmp(arrCliente[i].nick, nickname) == 0) {
            printToScreen("User found.\n");
            if(strcmp(arrCliente[i].pass, password) == 0){
                printToScreen("User connected.\n");
				semop(77981, &CDOWN, 1);
                arrCliente[i].pid = clientPid;
                tempMessage.data.value1 = arrCliente[i].id;
                arrCliente[i].online = clientPid; //Client online
				semop(77981, &CUP, 1);
                tempMessage.data.status = SUCCESS;
            }
        }
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
	printToScreen("Sending vehicle list to client %d.\n", clientID);
    MsgServerClient tempMessage;
    tempMessage.type = clientID;
    int i = 0;
    while (arrViatura[i].mudancas != -1) {
        printf("vehicle\n");
        if (arrViatura[i].status == AVAILABLE) {
            vehicleToString(tempMessage.data.text, i);
            printf("%s", tempMessage.data.text);
            tempMessage.data.status = SENDING;
            msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);
        }
        i++;
    }
    tempMessage.data.status = ENDLIST;
    msgsnd(idM, &tempMessage, sizeof(tempMessage.data), 0);    //Fim da lista
}

void reservar(int clientID, char* vehicleID){
	printToScreen("Reservar - Cliente %d a reservar um veiculo.", clientID);
    for (int i = 0; i<200; i++) { //TODO verificar tamanho
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            if(arrCliente[i].saldo > 0){
                for(int j = 0; j<200; j++) { // TODO verificar tamanho
                    if(strcmp(arrViatura[i].ID, vehicleID) == 0 && arrViatura[i].status == AVAILABLE) {
						semop(77981, &VDOWN, 1);
                        arrViatura[j].status = RESERVED;
                        arrViatura[j].clientID = clientID;
                        arrViatura[j].timeStarted = getTimeSecs();
                        arrViatura[j].clientIndex = i;
						semop(77981, &VUP, 1);
                        tempMessage.data.status = SUCCESS;
                        msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
                        break;
                    }
                }
            } else {
                tempMessage.data.status = FAIL;
                msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
            }
			return;
        }
    }
    printToScreen("Reservar - Client %d not found", clientID);
}

void alugar(int clientID, char* vehicleID){
	printToScreen("Client %d a tentar alugar um veiculo.", clientID);
    for (int i = 0; i<200; i++) { //TODO verificar tamanho
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            if(arrCliente[i].saldo > 0){
                for(int j = 0; j<200; j++) { // TODO verificar tamanho
                    if(strcmp(arrViatura[i].ID, vehicleID) == 0 && (arrViatura[i].status == AVAILABLE || (arrViatura[i].status == RESERVED && arrViatura[i].clientID == clientID))) {    //Explicar no relatório!!!
                        semop(77981, &VDOWN, 1);
						arrViatura[i].timeStarted = getTimeSecs();
						semop(77981, &VUP, 1);
                        tempMessage.data.status = SUCCESS;
                        msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
                        //Destranca rodas da viatura?
                        break;
                    }
                }
            } else {
                tempMessage.data.status = FAIL;
                msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
            }
            return;
        }
    }
    printToScreen("Alugar - Client %d not found", clientID);
}

void finalizar(int clientID){
	printToScreen("Cliente %d a tentar finalizar um aluguer", clientID);
    for(int i = 0; i<200; i++){
        if(arrCliente[i].id == clientID) {
            MsgServerClient tempMessage;
            tempMessage.type = clientID;
            tempMessage.data.status = FAIL;
            for(int j = 0; j<200; j++) { // TODO verificar tamanho
			semop(77981, &VDOWN, 1);
                if((arrViatura[i].status == RESERVED || arrViatura[i].status == RENTED) && arrViatura[i].clientID == clientID)  {
					arrViatura[i].timeStarted = -1;
                    arrViatura[i].status = AVAILABLE;
					semop(77981, &VUP, 1);
                    tempMessage.data.status = SUCCESS;
                    break;
                } else {
					semop(77981, &VUP, 1);
				}
            }
            msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
            break;
        }
    }
	printToScreen("Finalizar - Client %d not found", clientID);
}

void saldo(int clientID){
	printToScreen("Saldo - Cliente %d a pedir o seu saldo.", clientID);
	MsgServerClient tempMessage;
	tempMessage.type = clientID;
    tempMessage.data.status = FAIL;
    for(int i = 0; i<200; i++){	//TODO calcular tamanho array
		semop(77981, &CDOWN, 1);
		if(arrCliente[i].id == clientID) {
			for(int j = 0; j<200; j++){	//TODO calcular size array - Percorre viaturas
				semop(77981, &VDOWN, 1);
				if(arrViatura[j].clientID == clientID){
					long timeElapsed = getTimeSecs() - arrViatura[j].timeStarted);
					int timeElapsedSecs = ((int)timeElapsed) / 60;
					int saldoCliente = ((int)arrCliente[i]-timeElapsedSecs);
					printToScreen("Saldo cliente = %d", saldoCliente);
					tempMessage.data.status = SUCCESS;
					tempMessage.data.value1 = saldoCliente;
				}
				semop(77981, &VUP, 1);
			}
			semop(77981, &CUP, 1);
			msgsnd(77561, &tempMessage, sizeof(tempMessage.data), clientID);
			return;
		} else {
			semop(77981, &CUP, 1);
		}
		
	}
	printToScreen("Saldo - User not found!");
}

void carregarSaldo(int clientID, char* moneyToAddChar){
	printToScreen("Carregar - Cliente %d a tentar carregar saldo.", clientID);
    int moneyToAdd = atoi(moneyToAddChar);
    MsgServerClient tempMessage;
    tempMessage.type = clientID;
    tempMessage.data.status = FAIL;
    for(int i = 0; i<200; i++){
        if(arrCliente[i].id == clientID) {
            semop(77981, &CDOWN, 1);
            tempMessage.data.value1 = arrCliente[i].saldo;
            arrCliente[i].saldo += moneyToAdd;
            tempMessage.data.value2 = arrCliente[i].saldo;
            semop(77981, &CUP, 1);
            tempMessage.data.status = SUCCESS;
            break;
        }
    }
    msgsnd(77561, &tempMessage, sizeof(tempMessage.data), 0);
}

void logout(int clientID){
    MsgServerClient tempMessage;
    tempMessage.data.status = FAIL;
    for(int i = 0; i<200; i++){
        if(arrCliente[i].id == clientID) {
            semop(77981, &CDOWN, 1);
            arrCliente[i].online = -1;
			semop(77981, &CUP, 1);
            tempMessage.data.status = SUCCESS;
            break;
        }
    }
    msgsnd(77561, &tempMessage, sizeof(tempMessage.data), 0);
}



int main(){ //TODO registar nos logs
    
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
    
	char temp[20];
	sprintf(temp, "PID: %d", getpid());
	printToScreen(temp);
	
	FILE *pidFile = fopen("servidor.pid", "w");
	FILE *logFile = fopen("servidor.log", "a");

	if(pidFile == NULL){
		printToScreen("PID File - Error opening file!\n");
		exit(1);
	}
	
	fprintf(pidFile,"PID: %d\n", getpid() );
	printToScreen("PID: %d\n", getpid() );
	
	fclose(pidFile);
    fclose(logFile);
	
	pid = fork();
	
	if(pid==0){ //Child (Fiscal)
		signal(SIGINT, closeHandlerChild);
		signal(SIGALRM, alarmHandlerChild);
		alarm(60);
        while(1){
            for(int i = 0; i<200; i++){ //TODO verificar tamanho
				semop(77981, &VDOWN, 1);
                if(arrViatura[i].status == RESERVED && ((arrViatura[i].timeStarted - getTimeSecs())/60 >= 5)){
					arrViatura[i].status = AVAILABLE;
                    arrViatura[i].timeStarted = -1;
					semop(77981, &CDOWN, 1);
                    if(arrCliente[arrViatura[i].clientIndex].online) {		//Check to see if the client is online before sending a signal. The client shouldn't ever be offline when he has a reservation, but it's nice to check anyway
                        kill(arrCliente[arrViatura[i].clientIndex].pid, SIGUSR1);
						printToScreen("Cliente %d ultrapassou os 5 minutos!", arrCliente[arrViatura[i].clientIndex);
					}
					semop(77981, &CUP, 1);
					semop(77981, &VUP, 1);
                }
				
				semop(77981, &VDOWN, 1);
				semop(77981, &CDOWN, 1);
				if(arrCliente[arrViatura[i].clientIndex].saldo - 1 < 0){
					printToScreen("Cliente %d excedeu o seu saldo.", arrCliente[arrViatura[i].clientIndex].online);
					if(arrCliente[arrViatura[i].clientIndex].online != -1)
						kill(arrCliente[arrViatura[i].clientIndex].pid, SIGUSR2);
				} else {
					arrCliente[arrViatura[i].clientIndex].saldo--;
				}
				semop(77981, &CUP, 1);
				semop(77981, &VUP, 1);
            }
            sleep(1);
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
                    printf("listvehicles\n");
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
