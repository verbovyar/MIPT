#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool BinarySearch(int* begin, int* end, int target)
{
    while (begin != end - 1)
    {
        int* mid = (end - begin) / 2 + begin;

        if (*mid <= target)
        {
            begin = mid;
        }
        else
        {
            end = mid;
        }
    }

    return (*begin == target);
}

int main()
{
    int count = NULL;
    scanf("%d", &count);

    int* segment = (int*)calloc(count, sizeof(int));
    for (int index = 0; index < count; ++index)
    {
        scanf("%d", &segment[index]);
    }

    int countOfRepeats = NULL;
    scanf("%d", &countOfRepeats);


    int begin = NULL;
    int end = NULL;
    int target = NULL;
    for (int index = 0; index < countOfRepeats; ++index)
    {
        scanf("%d %d %d", &begin, &end, &target);

        if (BinarySearch(segment + begin, segment + end, target))
        {
            printf("YES\n");
        }
        else
        {
            printf("NO\n");
        }
    }

    return 0;
}
