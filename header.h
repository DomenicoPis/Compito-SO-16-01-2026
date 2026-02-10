#ifndef _HEADER_H_
#define _HEADER_H_

#include "monitor_hoare.h"

#define COND_PROD 0
#define COND_CONS 1

#define VUOTO 0
#define PIENO 1

typedef struct {

    int buffer;

    /* TBD: Completare la struttura con altre variabili per la sincronizzazione */

    int stato_buffer;
    Monitor m;

} MonitorProdCons;

void init_monitor_prodcons(MonitorProdCons *p);
void produzione(MonitorProdCons *p, int elemento);
int consumazione(MonitorProdCons *p);
void remove_monitor_prodcons(MonitorProdCons *p);

typedef struct {

    /* TBD: Completare la struttura del messaggio */
    long mtype;
    int valore;
    
} Messaggio;

#endif
