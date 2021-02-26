#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void EnterSequance(vector<int> &sequence, int count)    
{
    for (int idx = 0; idx < count; ++idx)
    {
        cin >> sequence[idx];
    }
}

void GetDynamic(vector<int> &dp, vector<int> &position, vector<int> &sequence, int count)
{
    for (int first_idx = 0; first_idx < count; ++first_idx)
    {
        dp[first_idx] = 1;
        position[first_idx] = -1;

        for (int second_idx = 0; second_idx < first_idx; ++second_idx)
        {
            if (sequence[second_idx] < sequence[first_idx])
            {
                if (dp[first_idx] < dp[second_idx] + 1)
                {
                    dp[first_idx] = dp[second_idx] + 1;
                    position[first_idx] = second_idx;
                }
            }
        }
    }
}

int GetPosition(vector<int> &dp, int count)
{
    int max = 0;
    int pos = 0;
    max = dp[0];

    for (int idx = 0; idx < count; ++idx)
    {
        if (max < dp[idx])
        {
            max = dp[idx];
            pos = idx;
        }
    }

    return pos;
}

void OutputSequance(vector<int>& position, vector<int>& sequence, vector<int> way, int pos)
{
    while (pos != -1)
    {
        way.push_back(pos);
        pos = position[pos];
    }

    std::reverse(way.begin(), way.end());

    for (int idx = 0; idx < way.size(); ++idx)
    {
        cout << sequence[way[idx]] << " ";
    }
}

int main()
{
    int count = 0;
    cin >> count;

    vector<int> sequence(count);

    EnterSequance(sequence, count);

    vector<int> dp(count);
    vector<int> position(count);   

    GetDynamic(dp, position, sequence, count);

    int pos = GetPosition(dp, count);

    vector<int> way;

    OutputSequance(position, sequence, way, pos);

    return 0;
}

