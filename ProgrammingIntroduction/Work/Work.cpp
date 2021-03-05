#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::max;

const int mod = 1e9 + 7;

void Enter(vector<vector<int>> &cost, int count, int time)
{
    for(int i = 0; i < time; ++i)
    {
        for(int j = 0; j < count; ++j)
        {
            cin >> cost[i][j];
        }
    }
}

void FillTempArray(vector<int> &temp, vector<vector<int>> &dp, vector<vector<int>> &cost, int year, int count)
{
    for(int idx = 0; idx < count; ++idx)
    {
        temp[idx] = 0;
    }

    for(int idx = 0; idx < count; ++idx)
    {
        int left = idx - cost[year][idx];
        int right = idx + cost[year][idx];

        if (left < 0) { 
            left = 0;
        }

        temp[left] = (temp[left] + dp[year][idx]) % mod;

        if(right + 1 < count)
        {
            temp[right + 1] = (temp[right + 1] + mod - dp[year][idx]) % mod;
        }
    }
}

void GetDynamic(vector<vector<int>> &cost, vector<vector<int>> &dp, int count, int time)
{
    for(int i = 0; i < count; ++i)
    {
        dp[0][i] = 1;
    }

    vector<int> temp(count);

    for(int i = 0; i < time; ++i)
    {
        FillTempArray(temp, dp, cost, i, count);

        dp[i + 1][0] = temp[0] % mod;

        for(int j = 1; j < count; ++j)
        {
            dp[i + 1][j] = (dp[i + 1][j - 1]+ temp[j]) % mod;
        }
    }
}

void Output(vector<vector<int>> &dp, int count, int time)
{
    for (int j = 0; j < count; ++j)
    {
        cout << (dp[time][j] % mod) << " ";
    }
}

int main()
{
    int time = 0;
    int count = 0;

    cin >> time >> count;

    vector<vector<int>> cost(time, vector<int> (count));

    Enter(cost, count, time);

    vector<vector<int>> dp(time + 1, vector<int> (count));

    GetDynamic(cost, dp, count, time);

    Output(dp, count, time);

    return 0;
}