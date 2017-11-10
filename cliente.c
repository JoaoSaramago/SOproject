#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

void closeHandler(int signal){
        printf(" %s Time is up, exiting!", time(NULL) );
        exit(0);
}

void balanceHandler(int signal){
    printf(" %s Insuficient funds, return soon!", time(NULL) );
}

int main(){

    printf("PID: %d\n", getpid());

    signal(SIGUSR1, closeHandler);
    signal(SIGUSR2, balanceHandler);

    while(1) sleep(1);

}