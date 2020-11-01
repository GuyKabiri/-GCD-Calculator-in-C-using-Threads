#include <stdio.h>
#include "general.h"

int read_Numbers_From_StdIn(int* num1, int* num2)
{
    char temp[MAX_CHARS_IN_LINE];
    char line[MAX_CHARS_IN_LINE];
	char* check = fgets(line, MAX_CHARS_IN_LINE - 1, stdin);

    if (check == NULL)  //  read entire line
	{
		if(feof(stdin))
        	return EOF;
		else
			return 0;
	}
    if (sscanf(line, "%d %d %s\n", num1, num2, temp) != 2)
        return ILLEGAL_INPUT;
    return 1;
}

//  calculate a gcd
int calc_gcd(int a, int b)
{
    while(a != b)
    {
        if(a > b)
            a -= b;
        else
            b -= a;
    }
    return a;
}