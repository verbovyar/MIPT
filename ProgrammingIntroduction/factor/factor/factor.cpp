#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int numberCount = 0;
    scanf("%d", &numberCount);

    int number = 0;
    for (int index = 0; index < numberCount; ++index)
    {
        scanf("%d", &number);

        for (int divider = 2; divider <= sqrt(number); ++divider)
        {
            while (number % divider == 0)
            {
                printf("%d ", divider);

                number /= divider;
            }
        }

        if (number != 1)
        {
            printf("%d", number);
        }

        printf("\n");
    }

    return 0;
}

