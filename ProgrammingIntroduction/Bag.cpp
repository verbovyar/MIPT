#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::max;

void Enter(vector<int> &weight, vector<int> &price, int count)
{
    for(int idx = 0; idx < count; ++idx)
    {
        cin >> weight[idx];
    }

    for(int idx = 0; idx < count; ++idx)
    {
        cin >> price[idx];
    }
}

void GetDynamic(vector<vector<int>> &dp, vector<int> &weight, vector<int> &price, int count, int bag_weight)
{
    for(int idx = 0; idx <= count; ++idx)
    {
        dp[idx][0] = 0;
    }

    for(int idx = 0; idx <= bag_weight; ++idx)
    {
        dp[0][idx] = 0;
    }

    for(int i = 0; i < count; ++i)
    {
        for(int j = 1; j <= bag_weight; ++j)
        {
            dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);

            if (weight[i] <= j)
            {
                dp[i + 1][j] = max(dp[i + 1][j], dp[i][j - weight[i]] + price[i]);
            }
        }
    }
}

void GetAnswer(vector<vector<int>> &dp, vector<int> &weight, vector<int> &answer, int bag_weight, int count)
{    
    int i = dp.size() - 1;
    int j = 0;
    
    for (size_t w = 0; w < dp[0].size(); ++w)
    {
        if (dp[i][j] < dp[i][w])
        {
            j = w;
        }
    }

	while (i != 0)
	{
		if (dp[i][j] > dp[i - 1][j])
		{
			j -= weight[i - 1];
            answer.push_back(i);
		}

		--i;
	}

    for(int i = 0; i < answer.size(); ++i)
    {
        cout << answer[i]<< endl;
    }
}


int main()
{
    std::ios_base::sync_with_stdio(false);

    int count = 0;
    int bag_weight = 0;

    cin >> count >> bag_weight;

    vector<int> weight(count);
    vector<int> price(count);

    Enter(weight, price, count);

    vector<vector<int>> dp (count + 1, vector<int> (bag_weight + 1));

    GetDynamic(dp, weight, price, count, bag_weight);

    vector<int> answer;

    GetAnswer(dp, weight, answer, bag_weight, count);

    //Output(answer);

    return 0;
}