#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

long long maxSubsegmentSum(long long* begin, long long* end)
{
    long long* left = end - 1;
    long long max_summ = *end - *left;

    long long max = *end;

    while (begin != left + 1)
    {
        if (*(left + 1) > max)
        {
            max = *(left + 1);
        }

        if ((max - *left) > max_summ)
        {
            max_summ = max - *left;
        }
        left--;
    }

    return max_summ;
}

int main()
{
    int len = 0;
    scanf("%d", &len);

    long long* pref_summ = (long long*)calloc(len + 1, sizeof(long long));
    pref_summ[0] = 0;
    for (int i = 1; i <= len; ++i)
    {
        scanf("%lld", pref_summ + i);
        pref_summ[i] += pref_summ[i - 1];
    }

    int count_of_requests = 0;
    scanf("%d", &count_of_requests);
    for (int k = 0; k < count_of_requests; k++)
    {
        int begin = 0;
        int end = 0;
        scanf("%d %d", &begin, &end);
        printf("%lld\n", maxSubsegmentSum(pref_summ + begin, pref_summ + end));
    }

    return 0;
}