#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::min;
using std::vector;

const int inf = 1e9;

struct Edge
{
    int from;
    int to;
    int capacity;
};

struct Graph
{
    int edge_count;
    int vertex_count;
    std::vector<Edge> graph;
    vector<vector<int>> residual;
};

void Input(Graph* info)
{
    cin >> info->vertex_count >> info->edge_count;

    info->residual.resize(info->vertex_count);
    for (int i = 0; i < info->vertex_count; ++i)
    {
        info->residual[i].resize(info->vertex_count);
    }

    int from = 0;
    int to = 0;
    int capacity = 0;
    for (int i = 0; i < info->edge_count; ++i)
    {
        cin >> from >> to >> capacity;

        info->graph.push_back({from - 1, to - 1, capacity});
        info->residual[to - 1][from - 1] = capacity;
        info->residual[from - 1][to - 1] = capacity;
    }
}

int FordFullkerson(Graph* info, int vertex, int finish, std::vector<bool>& visited, int capacity)
{
    visited[vertex] = true;

    if (vertex == finish)
    {
        return capacity;
    }

    for (int i = 0; i < info->vertex_count; ++i)
    {
        if (!visited[i] && info->residual[vertex][i] > 0)
        {
            int flow = FordFullkerson(info, i, finish, visited, min(capacity, info->residual[vertex][i]));
            if (flow > 0)
            {
                info->residual[vertex][i] -= flow;
                info->residual[i][vertex] += flow;
                return flow;
            }
        }
    }

    return 0;
}

void DFS(Graph* info, size_t vertex, std::vector<bool>& visited)
{
    visited[vertex] = true;

    for (int i = 0; i < info->vertex_count; ++i)
    {
        if (!visited[i] && info->residual[vertex][i] > 0)
        {
            DFS(info, i, visited);
        }
    }
}

int FindMinCut(Graph* info, std::vector<int>& cut)
{
    std::vector<bool> visited(info->vertex_count);

    int flow = 0;
    while (true) {
        for (int i = 0; i < info->vertex_count; ++i)
        {
            visited[i] = false;
        }

        int add = FordFullkerson(info, 0, info->vertex_count - 1, visited, inf);
        if (add == 0)
        {
            break;
        }
        flow += add;
    }

    for (int i = 0; i < info->vertex_count; ++i)
    {
        visited[i] = false;
    }
    DFS(info, 0, visited);
    for (int i = 0; i < info->graph.size(); ++i)
    {
        if (visited[info->graph[i].from] != visited[info->graph[i].to])
        {
            cut.push_back(i + 1);
        }
    }

    return flow;
}

int main()
{
    Graph info = {};
    Input(&info);

    std::vector<int> cut;
    int capacity = FindMinCut(&info, cut);

    cout << cut.size() << " " << capacity << "\n";
    for (int i = 0; i < cut.size(); ++i)
    {
        cout << cut[i] << " ";
    }
    
    return 0;
}