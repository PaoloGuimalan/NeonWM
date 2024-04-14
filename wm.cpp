#include "core/neoncore.hpp"
#include <pthread.h>

int main(){

    neoncore neoncr;

    neoncr.init_neon();
    pthread_t ui_thread;
    
    pthread_create(&ui_thread, NULL, &neoncr.update_ui_thread, NULL);

    neoncr.neonwm_run();
    neoncr.neonwm_terminate();
    return 0;
}

