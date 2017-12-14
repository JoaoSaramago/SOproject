#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define RESET   "\033[0m"
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

#define exit_on_error(s,m) if (s<0) { perror(m); exit(1);}
#define exit_on_null(s,m) if(s==NULL) {perror(m); exit(1);}

struct sembuf CDOWN = { 0, -1, 0};
struct sembuf CUP = { 0, 1, 0};

struct sembuf VDOWN = { 0, -1, 0};  //MUDAR!!
struct sembuf VUP = { 0, 1, 0};

typedef enum {
    LOGIN,
    VIATURAS,
    RESERVAR,
    ALUGAR,
    FINALIZAR,
    SALDO,
    CARREGAR,
    LOGOUT
} MsgType;

typedef enum {
    FAIL,
    SUCCESS,
    ENDLIST
} Status;

typedef enum {
    AVAILABLE,
    RESERVED,
    RENTED
} Availability;

typedef struct {
    char nick[50];
    char pass[50];
    int id;
    char nome[50];
    char email[40];
    char turma[10];
    int saldo;
    int online; //-1 Offline, ClientPID ou ClientID se online
    int pid;
} Tcliente;

typedef struct {
    char ID[20];
    char cor[20];
    char marca[50];
    char modelo[30];
    char tipo[20];
    int mudancas;
    char matricula[15];
    int clientID;
    Availability status;
    long timeStarted; //Quando começou a usar o veiculo ou quando fez a reserva, -1 disponivel
    int clientIndex;
} Tviatura;

typedef struct {
    long type;
    struct {
        MsgType msgType;
        char info1[20];
        char info2[20];
        int myid;
    } data;
} MsgClientServer;

typedef struct {
    long type;
    struct {
        char text[100];
        int value1;
        int value2;
        Status status;
    } data;
} MsgServerClient;

long getTimeSecs() {
    long            s;  // Seconds
    struct timespec spec;
    
    clock_gettime(CLOCK_MONOTONIC, &spec);
    
    s  = spec.tv_sec;
    
    return s;
}
