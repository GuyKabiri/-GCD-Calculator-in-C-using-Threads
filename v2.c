#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "v2.h"
#include "general.h"

#define THREAD_NUM 3
// #define PRING_THREAD_ID

int main(int argc, char** argv)
{
    pthread_t threads[THREAD_NUM];
    Thread_Data threads_data[THREAD_NUM];
    pthread_mutex_t gcd_arr_mutex;
    pthread_cond_t cond;
    GCD_Component_Mutex gcds;
    gcds.next = -1;
    gcds.count_done = 0;
    gcds.arr_struct.arr = NULL;
    gcds.arr_struct.len = 0;

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

    // create mutex lock
	if (pthread_mutex_init(&gcd_arr_mutex, NULL))
    {
        printf("Error creating mutex\n");
        exit(-1);
    }

    //  create condition variable
    if (pthread_cond_init(&cond, NULL))
    {
        printf("Error creating condition variable\n");
        exit(-1);
    }

    //  read all numbers
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
        gcds.arr_struct.len++;
        gcds.arr_struct.arr = realloc(gcds.arr_struct.arr, sizeof(GCD_Component) * gcds.arr_struct.len);
        if (!gcds.arr_struct.arr)
        {
            printf("Error reallocate\n");
            exit(-1);
        }
        init_gcd(&gcds.arr_struct.arr[gcds.arr_struct.len - 1], num1, num2);
    }

    if (gcds.arr_struct.len == 0)
    {
        printf("Input file is empty\n");
        exit(-1);
    }


    //  initiate all threads
    for (int i = 0; i < THREAD_NUM; i++)
    {
        threads_data[i].id = i;
        threads_data[i].gcds = &gcds;
        threads_data[i].mutex = &gcd_arr_mutex;
        threads_data[i].cond = &cond;
        if (pthread_create(&threads[i], NULL, thread_action, (void*)&threads_data[i]))
        {
            printf("Error creating thread\n");
            exit(-1);
        }
    }


    // lock before wait for condition variable
    pthread_mutex_lock(&gcd_arr_mutex);
    while (gcds.count_done < gcds.arr_struct.len)  // if not all gcd have been calculated
    {       
        pthread_cond_wait(&cond, &gcd_arr_mutex);   //  wait for siganl
    }
    pthread_mutex_unlock(&gcd_arr_mutex);   //  unlock after wait


    //  print all gcds
    printGCDarray(&gcds.arr_struct);

    //  join all thread, unnecessary, but requested in exercise
    for (int i = 0; i < THREAD_NUM; i++)
        pthread_join(threads[i], NULL);

    //  free gcds array
    freeGCDarray(&gcds.arr_struct);

    //  release lock and condition variable
    pthread_mutex_destroy(&gcd_arr_mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}


//  get a thread data structure and start calculate gcds, until all done, then exit thread
void* thread_action(void* params)
{
    Thread_Data* data = (Thread_Data*)params;
    int gcd_index;
    #ifdef PRINT_THREAD_ID
    printf("thread %d is working\n", data->id);
    #endif

    while(1)
    {
        //  lock before getting next gcd index to calculate
        pthread_mutex_lock(data->mutex);
        gcd_index = next_gcd(data->gcds);   //  get the next gcd index
        pthread_mutex_unlock(data->mutex);

        // if index == -1, all gcds have been calculated, exit thread
        if (gcd_index == -1)
            break;

        // get the gcd from array
        GCD_Component* g = &(data->gcds->arr_struct.arr)[gcd_index];
        // calculate the gcd value of two numbers
        int gcd_value = calc_gcd(g->num1, g->num2);
        update_gcd(g, gcd_value);   //  writing to different addresses, therefore no lock needed

        //  lock before update the done counter
        pthread_mutex_lock(data->mutex);
        data->gcds->count_done++;
        //  if work is done, signal to main thread
        if (data->gcds->count_done == data->gcds->arr_struct.len)
            pthread_cond_signal(data->cond);
        pthread_mutex_unlock(data->mutex);

        #ifdef PRINT_THREAD_ID
        printf("thread %d, %5d %5d: %5d\n", data->id, data->gcds->arr_struct.arr[gcd_index].num1, data->gcds->arr_struct.arr[gcd_index].num2, data->gcds->arr_struct.arr[gcd_index].res);
        #endif
    }

    #ifdef PRINT_THREAD_ID
    printf("thread %d is done\n", data->id);
    #endif

    pthread_exit(NULL);
    return NULL;
}


//  get the next gcd index to calculate, must lock before call to this function
int next_gcd(GCD_Component_Mutex* gcds)
{
    if (!gcds)
        return -1;
    
    // if there is no next gcd (all done), return -1
    if (gcds->next >= gcds->arr_struct.len - 1)
        return -1;

    // count up and return index
    gcds->next++;
    return gcds->next;
}