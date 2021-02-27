#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int delim = 1e6;

void Enter(vector<int>& sequance, int count)
{
    for (int idx = 0; idx < count; ++idx)
    {
        cin >> sequance[idx];
    }
}

void GetDynamic(vector<int>& sequance, vector<int>& dp, int count)
{
    for (int first_idx = 0; first_idx < count; ++first_idx)
    {
        dp[first_idx] = 1;

        for (int second_idx = 0; second_idx < first_idx; ++second_idx)
        {
            if (sequance[second_idx] < sequance[first_idx])
            {
                dp[first_idx] = (dp[first_idx] + dp[second_idx]) % delim;
            }
        }
    }
}

int Output(vector<int>& dp, int count)
{
    int answer = 0;

    for (int idx = 0; idx < count; ++idx)
    {
        answer += dp[idx];
    }

    return answer;
}

int main()
{
    int count = 0; 
    cin >> count;

    vector<int> sequance(count);

    Enter(sequance, count);

    vector<int> dp(count);

    GetDynamic(sequance, dp, count);

    int answer = Output(dp, count);

    cout << answer % delim;

    return 0;
}
