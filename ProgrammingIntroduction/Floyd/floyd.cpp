#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

void Input(vector<vector<bool>> &graph, int vertex_count)
{
    int temp = 0;

    for (int i = 0; i < vertex_count; ++i)
    {
        for (int j = 0; j < vertex_count; ++j)
        {
            cin >> temp;

            if (temp == 1)
            {
                graph[i][j] = true;
            }
            else
            {
                graph[i][j] = false;
            }
        }
    }
}

void Floyd(vector<vector<bool>> &graph, int vertex_count)
{
    for (int k = 0; k < vertex_count; ++k)
    {
        for (int i = 0; i < vertex_count; ++i)
        {
            for (int j = 0; j < vertex_count; ++j)
            {
                graph[i][j] = graph[i][j] || (graph[i][k] && graph[k][j]);
            }
        }
    }
}

void GetAnswer(vector<vector<bool>> &graph, int vertex_count)
{
    for (int i = 0; i < vertex_count; ++i)
    {
        for (int j = 0; j < vertex_count; ++j)
        {
            cout << graph[i][j] << " ";
        }

        cout << "\n";
    }
}

int main()
{
    int vertex_count = 0;
    cin >> vertex_count;

    vector<vector<bool>> graph(vertex_count, vector<bool>(vertex_count));
    Input(graph, vertex_count);

    Floyd(graph, vertex_count);

    GetAnswer(graph, vertex_count);

    return 0;
}