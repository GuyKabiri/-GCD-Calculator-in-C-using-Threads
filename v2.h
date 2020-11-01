#ifndef __V2__
#define __V2__

#include <pthread.h>
#include "gcd_struct.h"

typedef struct
{
    int id;
    struct gcd_with_mutex* gcds;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} Thread_Data;

typedef struct gcd_with_mutex 
{
    GCD_Array arr_struct;
    int next;
    int count_done;
} GCD_Component_Mutex;



int main(int argc, char** argv);
void* thread_action(void* params);
int next_gcd(GCD_Component_Mutex* gcds);

#endif