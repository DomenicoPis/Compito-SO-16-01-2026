#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include "header.h"

void Produttore(MonitorProdCons * m) {

    srand(getpid());

    for(int i = 0; i < 5; i++) {

        int valore = rand() % 10;

        printf("Produzione [%d]: %d\n", getpid(), valore);

        produzione(m, valore);

    }
}

void Consumatore(MonitorProdCons * m1, MonitorProdCons * m2, int id_coda) {

    for(int i = 0; i < 5; i++) {

        int valore1 = consumazione(m1);

        printf("[Consumatore] Consumazione n.1: %d\n", valore1);

        int valore2 = consumazione(m2);

        printf("[Consumatore] Consumazione n.2: %d\n", valore2);

        int valore_tot = valore1 + valore2;



        /* TBD: Effettuare una SEND SINCRONA verso il processo Gestore, per inviargli "valore_tot" */

        Messaggio mess; 
        mess.mtype = 1; 
        mess.valore = valore_tot;

        msgsnd(id_coda, &mess, sizeof(Messaggio)-sizeof(long), 0);
        printf("[Consumatore] Inviato: %d. Attendo ACK...\n", valore_tot);

        // 3. Mi blocco in attesa della CONFERMA (ACK) dal Gestore (TIPO_ACK = 2)
        // Questo rende la send "sincrona": non proseguo finché lui non risponde
        msgrcv(id_coda, &mess, sizeof(Messaggio)-sizeof(long), 2, 0);

    }
}


void Gestore(int id_coda) {

    for(int i = 0; i < 5; i++) {

        /* TBD: Effettuare la ricezione dal processo Consumatore */
        
        Messaggio msg;

        msgrcv(id_coda, &msg, sizeof(Messaggio)-sizeof(long), 1, 0);

        int valore_totale = msg.valore;

        printf("[Gestore] Valore totale ricevuto: %d\n", valore_totale);

        msg.mtype = 2;
        msgsnd(id_coda, &msg, sizeof(Messaggio)-sizeof(long), 0);

    }

}


int main() {

    /* TBD: Inizializzare le risorse IPC */

    int id_shm = shmget(IPC_PRIVATE, 2 * sizeof(MonitorProdCons), IPC_CREAT | 0664);

    if(id_shm < 0){

        perror("shmget");
        exit(1);
    }

    MonitorProdCons * monitor = (MonitorProdCons *) shmat(id_shm, NULL, 0);

    if(monitor == (void*)-1){

        perror("shmat");
        exit(1);
    }

    init_monitor_prodcons(&monitors[0]); // Buffer 1
    init_monitor_prodcons(&monitors[1]); // Buffer 2

    //alloco la coda di messaggi

    int id_coda = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
    
    if(id_coda < 0){

        perror("msgget");
        exit(1);
    }


    /* TBD: Creare i processi figli Produttore, e fargli eseguire la funzione Produttore() */


    for(int i=0; i<2; i++){

        pid_t pid_prod = fork();

        if(pid_prod == 0){

            Produttore(&monitors[i]);
            exit(0);
        }else if(pid_prod < 0){

            perror("errore fork");
            exit(1);
        }

    }

    /* TBD: Creare il processo figlio Consumatore, e fargli eseguire la funzione Consumatore() */

    pid_t pid_cons = fork();

    if(pid_cons == 0){

        Consumatore(&monitors[0], &monitors[1], id_coda);
        exit(0);
    }else if(pid_cons < 0){

        perror("errore fork");
        exit(1);
    }

    /* TBD: Creare il processo figlio Gestore, e fargli eseguire la funzione Gestore() */

    pid_t pid_gest = fork();

    if(pid_gest == 0){

        Gestore(id_coda);

    }else if(pid_gest < 0){

        perror("errore fork");
        exit(1);
    }

    /* TBD: Attendere la terminazione dei processi figli */

    int status;

    for(int i=0; i<4; i++){

        wait(&status);

    }

    /* TBD: De-inizializzare le risorse IPC */

    remove_monitor_prodcons(&monitors[0]);
    remove_monitor_prodcons(&monitors[1]);

    shmctl(id_shm, IPC_RMID, NULL);

    msgctl(id_coda, IPC_RMID, NULL);

    return 0;
}