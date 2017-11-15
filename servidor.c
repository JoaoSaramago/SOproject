#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

int pid;

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

int main(){
	char temp[20];
	sprintf(temp, "PID: %d", getpid());
	printToScreen(temp);
	
	FILE *pidFile = fopen("servidor.pid", "w");
	FILE *logFile = fopen("servidor.log", "a");

	if(pidFile == NULL){
		printf("Error opening file!\n");
		fprintf(logFile, "Error opening file!\n");
		exit(1);
	}
	
	fprintf(pidFile,"PID: %d\n", getpid() );
	fprintf(logFile, "PID: %d\n", getpid() );
	
	fclose(pidFile);
    fclose(logFile);
	
	pid = fork();
	
	if(pid==0){ //Child (Fiscal)
		signal(SIGINT, closeHandlerChild);
		signal(SIGALRM, alarmHandlerChild);
		alarm(60);
		while(1) sleep(1);
	}
	
	if(pid>0){ //Father
		signal(SIGINT, closeHandlerFather);
		while(1) sleep(1);
	}
}
