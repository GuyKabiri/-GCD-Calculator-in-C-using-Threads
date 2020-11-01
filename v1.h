#ifndef __V1__
#define __V1__

#include "gcd_struct.h"

typedef struct
{
    int id;
    int *indexes;
    int arr_len;
    GCD_Array* all_gcds;
} Thread_Data; 

int main(int, char**);
void* thread_action(void *);
void calc_gcd_sequences(Thread_Data* td);
void thread_print(Thread_Data *data);

#endif