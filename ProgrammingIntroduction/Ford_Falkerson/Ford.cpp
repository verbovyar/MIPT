#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int inf = 1e9;

struct Graph
{
    int start  = 0;
    int finish = 0;
    vector<vector<int>> graph;
};


void Input         (Graph* info, int int_count);
int  FindMax       (Graph* info, vector<bool> &visited, int vertex_count);
int  SearchMatching(Graph* info, vector<bool> &visited, int vertex_count, int from, int min);

int main()
{
    int vertex_count = 0;
    int edge_count   = 0;

    cin >> vertex_count >> edge_count;

    Graph info  = {};

    info.graph.resize(vertex_count, vector<int>(vertex_count));
    info.start  = 0;
    info.finish = vertex_count - 1;

    Input(&info, edge_count);

    vector<bool> visited(vertex_count, false);

    cout << FindMax(&info, visited, vertex_count);

    return 0;
}

void Input(Graph* info, int edge_count)
{
    int to     = 0;
    int from   = 0;
    int weight = 0;

    for (int i = 0; i < edge_count; ++i)
    {
        cin >> to >> from >> weight;

        info->graph[to - 1][from - 1] = weight;
    }
}

int FindMax(Graph* info, vector<bool> &visited, int vertex_count)
{
    int total = 0;
    int delta = SearchMatching(info, visited, vertex_count, 0, inf);

    while (delta != 0)
    {
        total += delta;
        visited.assign(vertex_count, false);
        delta  = SearchMatching(info, visited, vertex_count, 0, inf);
    }
    
    return total;
}

int SearchMatching(Graph* info, vector<bool> &visited, int vertex_count, int from, int min)
{
    if (from == info->finish)
    { 
        return min; 
    }

    visited[from] = true;

    for (int to = 0; to < vertex_count; ++to)
    {
        if (visited[to])
        {
            continue;
        }

        int capacity = info->graph[from][to];

        if (capacity > 0)
        {
            min = std::min(min, capacity);

            int delta = SearchMatching(info, visited, vertex_count, to, min);
            if (delta > 0)
            {
                info->graph[from][to] -= delta;
                info->graph[to][from] += delta;

                return delta;
            }
        }
    }

    return 0;
}