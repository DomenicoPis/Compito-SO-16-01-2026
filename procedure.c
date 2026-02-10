#include "header.h"

void init_monitor_prodcons(MonitorProdCons *p) {

    /* TBD: Inizializzare il monitor */
    init_monitor(&p->m, 2);

    p->buffer = 0;
    p->stato_buffer = VUOTO;
}


void produzione(MonitorProdCons *p, int elemento) {

    /* TBD: Effettuare la produzione */

    enter_monitor(&p->m);

    if(p->stato_buffer == PIENO){

        wait_condition(&p->m, COND_PROD);

    }

    p->buffer = elemento;
    p->stato_buffer = PIENO;

    signal_condition(&p->m, COND_CONS);
    leave_monitor(&p->m);

}


int consumazione(MonitorProdCons *p) {

    /* TBD: Effettuare la consumazione */
    int valore;

    enter_monitor(&p->m);

    if(p->stato_buffer == VUOTO){

        wait_condition(&p->m, COND_CONS);

    }

    valore = p->buffer;
    p->stato_buffer = VUOTO;

    signal_condition(&p->m, COND_PROD);
    leave_monitor(&p->m);

    return valore;
}


void remove_monitor_prodcons(MonitorProdCons *p) {

    /* TBD: De-inizializzare il monitor */

    remove_monitor(&p->m);

}
