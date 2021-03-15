#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;

const int inf = 1e7;

void Enter(int height, int width, vector<vector<bool>> &is_sub)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int temp = 0;
            cin >> temp;

            if (temp == 1)
            {
                is_sub[i][j] = true;
            }
            else
            {
                is_sub[i][j] = false;
            }
        }
    }
}

void Filling(int height, int width, vector<int> &way)
{
    for (int i = 0; i < height * width; ++i)
    {
        way[i] = inf;
    }
}

void MakeGraph(int height, int width, vector<vector<int>> &graph)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (j - 1 >= 0)
            {
                graph[i * width + j].push_back(i * width + j - 1);
            }

            if (j + 1 < width)
            {
                graph[i * width + j].push_back(i * width + j + 1);
            }

            if (i - 1 >= 0)
            {
                graph[i * width + j].push_back((i - 1) * width + j);
            }

            if (i + 1 < height)
            {
                graph[i * width + j].push_back((i + 1) * width + j);
            }
        }
    }
}

void Bfs(int start, vector<vector<int>> &graph, vector<int> &ways)
{
    queue<int> v_queue;

    v_queue.push(start);
    ways[start] = 0;

    while (!v_queue.empty())
    {
        int v = v_queue.front();
        v_queue.pop();
        for (int i = 0; i < graph[v].size(); ++i)
        {
            int to = graph[v][i];
        
            if (ways[v] + 1 < ways[to])
            {
                v_queue.push (to);
                ways[to] = ways[v] + 1;
            }
        }
    }
}

void CheckOnSub(int height, int width, vector<vector<bool>> &is_sub, vector<int> &way, vector<vector<int>> &graph)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if  (is_sub[i][j])
            {
                Bfs(i * width + j, graph, way);
            }
        }
    }
}

void GetAnswer(int height, int width, vector<int> &way)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cout << way[i * width + j] << " ";
        }

        cout << endl;
    }
}

int main()
{
    int height = 0;
    int width = 0;

    cin >> height >> width;

    vector<vector<bool>> is_sub(height, vector<bool> (width));

    Enter(height, width, is_sub);

    vector<vector<int>> graph(height * width, vector<int> ());
    vector<int> way (height * width);

    Filling(height, width, way);

    MakeGraph(height, width, graph);

    CheckOnSub(height, width, is_sub, way, graph);

    GetAnswer(height, width, way);

    return 0;
}