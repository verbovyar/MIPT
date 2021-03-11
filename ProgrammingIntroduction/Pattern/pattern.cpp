#include<iostream>
#include<vector>

using namespace std;

const int inf = 13e6 + 1;

void Enter(int count, vector<vector<int>> &way)
{
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            cin >> way[i][j];
        }
    }
}

int GetBit(int i)
{
    return 1 << i;
}

bool Bit (int mask, int pos)
{
    return (mask >> pos) & 1;
}

void FillBase(int count, vector<vector<int>> &dp)
{
    //cout << "here";

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < (1 << count); ++j)
        {
            dp[i][j] = inf;
        }

        dp[i][GetBit(i)] = 0;
    }
}

void GetDynamic(int count, vector<vector<int>> &way, vector<vector<int>> &dp, vector<vector<int>> &prev)
{
    //cout << "Here!";

    for (int mask = 1; mask < (1 << count); ++mask)
    {
        for (int i = 0; i < count; ++i)
        {
            for (int j = 0; j < count; ++j)
            {
                if (!Bit (mask, i) || Bit (mask, j)) 
                {
                    continue;
                }

                int new_mask = mask | GetBit(j);

                if (dp[j][new_mask] > (dp[i][mask] + way[i][j]))
                {
                    dp[j][new_mask] = (dp[i][mask] + way[i][j]);
                    prev[j][new_mask] = i;
                }
            }
        }
    }
}

void GetAnswer(int count, vector<vector<int>> &dp, vector<int> &answer, vector<vector<int>> &prev)
{
    //cout << "HERE";

    int min = inf;
    int start_point = 0;

    for (int i = 0; i < count; ++i)
    {
        if(dp[i][(1 << count) - 1] < min)
        {
            min = dp[i][(1 << count) - 1];
            start_point = i;
        }
    }

    cout << min << endl;

    int mask = (1 << count) - 1; //^ GetBit(answer[count - 1]);

    answer[count - 1] = start_point;

    for(int i = count - 2; i >= 0; --i)
    {
        answer[i] = prev[answer[i + 1]][mask];

        mask = mask ^ GetBit(answer[i + 1]);
    }

    for (int i = 0; i < count; ++i)
    {
        cout << answer[i] + 1 << " "; 
    }
}

int main()
{
    int count = 0;

    cin >> count;

    vector<vector<int>> way(count, vector<int>(count));

    Enter(count, way);
    
    vector<vector<int>> dp(count, vector<int>(1 << count));

    FillBase(count, dp);

    vector<vector<int>> prev(count, vector<int>(1 << count));

    GetDynamic(count, way, dp, prev);

    vector<int> answer(count);

    GetAnswer(count, dp, answer, prev);

    return 0;
} 
