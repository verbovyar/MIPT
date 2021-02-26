#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::min;
using std::vector;

void Enter(vector<int> &nails, int count)
{
    for (int idx = 0; idx < count; ++idx)
    {
        cin >> nails[idx];
    }
}

void MakeBase(vector<int> &dp, vector<int> &nails)
{
    dp[0] = 0;
    dp[1] = nails[1] - nails[0];
    dp[2] = nails[2] - nails[0];
    dp[3] = nails[1] - nails[0] + nails[3] - nails[2];
}

void GetDynamic(vector<int>& dp, vector<int>& nails, int count)
{
    for (int idx = 4; idx < count; ++idx)
    {
        dp[idx] = min(dp[idx - 1], dp[idx - 2]) + nails[idx] - nails[idx - 1];
    }
}

int main()
{
    int count = 0;
    cin >> count;

    vector<int> nails(count);

    Enter(nails, count);

    std::sort(nails.begin(), nails.end());

    vector<int> dp(count);

    MakeBase(dp, nails);

    GetDynamic(dp, nails, count);

    cout << dp[count - 1];

    return 0;
}

