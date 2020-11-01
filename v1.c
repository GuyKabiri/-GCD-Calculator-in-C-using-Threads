#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "v1.h"
#include "general.h"

#define NUM_THREAD 3
// #define PRING_THREAD_ID

int main(int argc, char ** argv)
{
    GCD_Array gcds_array;
    gcds_array.arr = NULL;
    gcds_array.len = 0;

    //  too much parameters
    if (argc > 2)
    {
        printf("Too many arguments\n");
        exit(-1);
    }

    //  try to open input file
    else if (argc == 2)
    {
        FILE* file = fopen(argv[1], "r");
        if (!file)
        {
            printf("Can not open file %s\n", argv[1]);
            exit(-1);
        }
        dup2(fileno(file), STDIN_FILENO);
        close(fileno(file));
    }


    int num1, num2, check;
    while(1)
    {   
        check = read_Numbers_From_StdIn(&num1, &num2);
        if (check == EOF)
            break;
        else if (check == ILLEGAL_INPUT)
        {
            printf("Illegal input\n");
            continue;
        }
        
        //  increase arr length
        gcds_array.len++;
        gcds_array.arr = realloc(gcds_array.arr, sizeof(GCD_Component) * gcds_array.len);
        if (!gcds_array.arr)
        {
            printf("Error reallocate\n");
            exit(-1);
        }
        init_gcd(&gcds_array.arr[gcds_array.len - 1], num1, num2);
    }

    if (gcds_array.len == 0)
    {
        printf("Input file is empty\n");
        exit(-1);
    }

    pthread_t threads[NUM_THREAD];  //  number of threads (exclude the main thread)

    //  struct to save each thread parameters, last index for the main thread
    Thread_Data threads_data[NUM_THREAD];

    //  initiate each thread with it's parameters
    for (int i = 0; i < NUM_THREAD; i++)
    {
        threads_data[i].id = i;
        threads_data[i].arr_len = 0;
        threads_data[i].indexes = NULL;
        threads_data[i].all_gcds = &gcds_array; // set pointer for main array of gcds to all the threads
    }

    //  initiate main thread
    Thread_Data main_thread;
    main_thread.id = -1;
    main_thread.arr_len = 0;
    main_thread.indexes = NULL;
    main_thread.all_gcds = &gcds_array;

    // all gcd devide total number of threads (include main thread)
    int num_gcd_per_thread = gcds_array.len / (NUM_THREAD + 1);

    //  if there are more threads than numbers
    if (gcds_array.len < NUM_THREAD + 1)
        num_gcd_per_thread = 1;

    int count_gcds = 0;
    Thread_Data* pData;
    for (int i = 0; i < gcds_array.len; i++)
    {
        // calculate index of thread in the array
        int thread_index = i % (NUM_THREAD + 1);
        pData = &threads_data[thread_index];
        if (thread_index == 3)
            pData = &main_thread;

        //  reallocate thread's gcds array
        pData->arr_len++;
        pData->indexes = realloc(pData->indexes, sizeof(int) * pData->arr_len);
        if (!threads_data[thread_index].indexes)
        {
            printf("Error reallocate\n");
            exit(-1);
        }
        //  add a gcd to the thread's array in the last index of array
        ((int*)pData->indexes)[pData->arr_len - 1] = i;
    }

    //  iterate over all the other threads, create them execute their functions
    for (int i = 0; i < NUM_THREAD; i++)
    {
        //  if error creating thread
        if (pthread_create(&threads[i], NULL, thread_action, (void*)&threads_data[i]))
        {
            printf("\nerror creating thread\n");
            exit(-1);
        }
    }

    //  send main thread to calculate it's gcds
    calc_gcd_sequences(&main_thread);

    //  join all other threads
    for (int i = 0 ; i < NUM_THREAD; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //  print all gcds
    printf("\n");
    printGCDarray(&gcds_array);

    //  free all resources
    for (int i = 0; i < NUM_THREAD; i++)
        free(threads_data[i].indexes);
    free(main_thread.indexes);
    freeGCDarray(&gcds_array);
    pthread_exit(NULL);
}


//  get a thread data structure and send it to calculate all it's gcds, then exit thread
void* thread_action(void * params)
{
    calc_gcd_sequences((Thread_Data*)params);
        
    pthread_exit(NULL);
    return NULL;
}

// get a thread data structure and calculate all of it's gcds
void calc_gcd_sequences(Thread_Data* td)
{  
    //  iterate over array of gcds and calculate each one
    for (int i = 0; i < td->arr_len; i++)
    {
        //  get the right structure from array
        GCD_Component *g = &td->all_gcds->arr[td->indexes[i]];

        int val = calc_gcd(g->num1, g->num2);

        update_gcd(g, val);
    }
}