#include <stdio.h>
#include <stdlib.h>
#include "gcd_struct.h"

//  get a gcd structure and two numbers and initiate the struct with the numbers
void init_gcd(GCD_Component* g, int num1, int num2)
{
    if (!g)
        return;
    
    g->res = 0;
    g->num1 = num1;
    g->num2 = num2;
}

void update_gcd(GCD_Component* gcd, int value)
{
    gcd->res = value;
}

void freeGCDarray(GCD_Array* gcds)
{
    if (!gcds)
        return;
    free(gcds->arr);
}

void printGCDarray(GCD_Array* gcds)
{
    if (!gcds)
        return;
    for (int i = 0;i < gcds->len; i++)
        printf("%5d %5d: %5d\n", gcds->arr[i].num1, gcds->arr[i].num2, gcds->arr[i].res);
}