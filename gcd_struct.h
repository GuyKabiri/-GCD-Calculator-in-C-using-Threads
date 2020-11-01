#ifndef __GCD__
#define __GCD__

typedef struct gcd_struct
{
    int num1;
    int num2;
    int res;
} GCD_Component;

typedef struct gcd_arr
{
    struct gcd_struct* arr;
    int len;    
}GCD_Array;


void init_gcd(GCD_Component* g, int num1, int num2);
void update_gcd(GCD_Component* gcd, int value);
void freeGCDarray(GCD_Array* gcds);
void printGCDarray(GCD_Array* gcds);

#endif