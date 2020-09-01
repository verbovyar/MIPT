#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>

int main()
{
    int c;
    int countofdigit = 0;
    int count = 0;
    int digit[10];

    while ((c = getchar()) != EOF) 
    {
        if (c == '\n')
        {
            count++;
        }
    }

    printf("%d", count);

    return 0;
}


