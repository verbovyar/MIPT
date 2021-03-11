#include<iostream>
#include<vector>

using namespace std;

bool GetBit (int mask, int pos)
{
    return (mask >> pos) & 1;
}

bool IsCorrect(int mask, int mask1, int height)
{
    for (int i = 0; i < height - 1; ++i)
    {
        if ((GetBit(mask, i) == GetBit(mask, i + 1)) && (GetBit(mask1, i) == GetBit(mask1, i + 1)) && (GetBit(mask, i) == GetBit(mask1, i)))
        {
            return false;
        }
    }

    return true;
}

void GetDynamic(vector<vector<int>> &dp, int height, int width)
{
    for (int mask = 0; mask < (1 << height); ++mask)
    {
        dp[0][mask] = 1;
    }
    
    for (int i = 0; i < width - 1; ++i)
    {
        for (int mask = 0; mask < (1 << height); ++mask)
        {
            for (int mask1 = 0; mask1 < (1 << height); ++mask1)
            {
                if (IsCorrect(mask, mask1, height))
                {
                    dp[i + 1][mask1] += dp[i][mask];
                }
            }
        }
    }
}

int GetAnswer(vector<vector<int>> &dp, int height, int width)
{
    int count = 0;

    for (int i = 0; i < (1 << height); ++i)
    {
        count += dp[width - 1][i];
    }

    return count;
}

int main()
{
    int height = 0;
    int width = 0;

    cin >> height >> width;

    if (height > width)
    {
        swap(height, width);   
    }

    vector<vector<int>> dp (width, vector<int> (1 << height));

    GetDynamic(dp, height, width);

    cout << GetAnswer(dp, height, width);

    return 0;
}