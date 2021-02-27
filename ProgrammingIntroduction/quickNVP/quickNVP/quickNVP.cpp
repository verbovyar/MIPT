#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::max;
using std::upper_bound;
using std::vector;

const int INF = 1e5 + 2;

void EnterSequance(vector<int>& sequence, int count)
{
    for (int idx = 0; idx < count; ++idx)
    {
        cin >> sequence[idx];
    }
}

int Lis(vector<int> &dp, vector<int>& sequence, int count)
{
    int len = 0;
    dp[0] = -INF;

    for (int idx = 1; idx < count; ++idx)
    {
        dp[idx] = INF;
    }

    for (int idx = 0; idx < count; ++idx)
    {
        int pos = upper_bound(dp.begin(), dp.end(), sequence[idx]) - dp.begin();

        if (dp[pos - 1] < sequence[idx] && sequence[idx] < dp[pos])
        {
            dp[pos] = sequence[idx];
            len = max(len, pos);
        }
    }

    return len;
}

int main()
{   
    int count = 0;
    cin >> count;

    vector<int> sequence(count);

    EnterSequance(sequence, count);

    vector<int> dp(count);

    int answer = Lis(dp, sequence, count);

    cout << answer;

    return 0;
}
