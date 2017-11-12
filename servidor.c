#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int pid;

void closeHandlerFather(){
    remove("servidor.pid");
    kill(pid, SIGINT);
    printf("Father closing.\n");
    FILE *file = fopen("servidor.log", "a");
    fprintf(file, "Father closing.\n");
    fclose(file);
    exit(0);
}

void closeHandlerChild(){
    printf("Fiscal is closing.\n");
    FILE *file = fopen("servidor.log", "a");
    fprintf(file, "Fiscal is closing.\n");
    fclose(file);
    exit(0);
}

void alarmHandlerChild(){
    printf("Fiscal is verifying the data.\n");
    FILE *file = fopen("servidor.log", "a");
    fprintf(file, "Fiscal is verifying the data.\n");
    fclose(file);
}

int main(){

    printf("PID: %d\n", getpid() );

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
