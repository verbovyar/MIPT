#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>

#define MAXLINE 1000
#define IFELSE(a, b) if (a > b) \
                        {\
                            printf("NO");\
                        }

// const
const int MAX = 1001;
const char* privet = "Hello wordl";
const int array[5] = { 1, 2, 3, 4, 5 };
static int my = 1; // dont view in any files

int power(int x, int n)
{
    int p = 1;

    for ( ;n > 0; --n)
    {
        p *= x;
    }

    return p;
}   

int mystrlen(char* line) // strlen
{
    int i = 0;

    while (line[i] != '\0')
    {
        ++i;
    }

    return i;
}

int atoi(char s[])
{
    int n = 0;

    for (int i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0'); // s[i] - '0': take number, no simbol!
    }
        
    return n;
}

char* myget(char *line)
{
    int i = 0;
    int c;

    while ((c = getchar()) != EOF && c != '\n' && i < MAXLINE)
    {
        line[i] = c;
        ++i;
    }

    return line;
}

int  bin(int x, int v[], int n)
{
    int low = 0;
    int high = n - 1;
    int mid;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (x < v[mid])
        {
            high = mid - 1;
        }
        else if (x > v[mid])
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }

    return -1; // not found
}

int trim(char s[])
{
    int len = mystrlen(s);
    int i;

    for (i = 0; i < len; ++i)
    {
        if (s[i] == ' ')
        {
            break;
        }
    }

    return i;
}

int main()
{
    char str[MAX] = {0};
    int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    IFELSE(2, 1); // macro

    myget(str);

    printf("%d\n",atoi(str));

    int x = atoi(str);

    double y = 0;
    (int)y; // new type

    int ind = bin(7, array, 10);

    switch (ind) // cases
    {
    case(7):
    {
        printf("YES\n");

        break;
    }
    case(6):
    {
        printf("NO\n");

        break;
    }
    }

    printf("%d", trim(str));

    return 0;
}


