#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

void EnterSequance(vector<int>& sequence, int count)
{
    for (int idx = 0; idx < count; ++idx)
    {
        cin >> sequence[idx];
    }
}

void Filling(vector<int> &first, vector<int> &second, vector<vector<int>>&lcs, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        if (first[i] == second[0])
        {
            lcs[i][0] = 1;
        }
    }

    for (int i = 0; i < M; i++)
    {
        if (second[i] == first[0])
        {
            lcs[0][i] = 1;
        }
    }
}

void Lcs(vector<int>& first, vector<int>& second, vector<vector<int>>& lcs)
{
    for (int i = first.size() - 1; i >= 0; --i)
    {
        for (int j = second.size() - 1; j >= 0; --j)
        {

            if (first[i] == second[j])
            {
                lcs[i][j] = lcs[i + 1][j + 1] + 1;
            }
            else
            {
                lcs[i][j] = std::max(lcs[i + 1][j], lcs[i][j + 1]);
            }
        }
    }
}

void Answer(vector<int>& first, vector<int>& second, vector<vector<int>>& lcs, vector<int>& result)
{
    int i = 0;
    int j = 0;

    for (; i < first.size() && j < second.size();)
    {
        if (first[i] == second[j])
        {
            result.push_back(first[i]);
            ++i;
            ++j;
        }
        else
        {
            if (lcs[i][j] == lcs[i + 1][j])
            {
                ++i;
            }
            else
            { 
                ++j;
            }
        }
    }

    for (int i = 0; i < result.size(); ++i)
    {
        cout << result[i] << " ";
    }
}

int main()
{
    int N = 0;
    cin >> N;

    vector<int> first(N);
    EnterSequance(first, N);

    int M = 0;
    cin >> M;

    vector<int> second(M);
    EnterSequance(second, M);

    vector<vector<int>> lcs(N + 1, vector<int>(M + 1));

    Filling(first, second, lcs, N, M);

    Lcs(first, second, lcs);

    vector<int> result;

    Answer(first, second, lcs, result);

    return 0;
}
