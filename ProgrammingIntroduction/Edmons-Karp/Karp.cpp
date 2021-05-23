#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::min;
using std::vector;
using std::queue;

const int inf = 1e9;
const int no_parent = -1;

struct Graph
{
    int first_part;
    int second_part;
    int vertex_count;
    vector<vector<int>> graph;
    vector<vector<int>> residual;
};

void Input(Graph* info)
{
    cin >> info->first_part >> info->second_part;

    info->vertex_count = info->first_part + info->second_part + 2;

    info->graph.resize(info->vertex_count);
    info->residual.resize(info->vertex_count);
    for (int i = 0; i < info->vertex_count; ++i) {
        info->graph[i].resize(info->vertex_count);
        info->residual[i].resize(info->vertex_count);
    }

    for(int i = 0; i < info->first_part; ++i)
    {
        int vertex = 0;
        while(true)
        {
            cin >> vertex;
            if (vertex == 0) {
                break;
            }
            --vertex;
            
            info->graph[i][info->first_part + vertex] = 1;
            info->residual[i][info->first_part + vertex] = 1;
        }
    }

    for (int i = 0; i < info->first_part; ++i)
    {
        info->graph[info->vertex_count - 2][i] = 1;
        info->residual[info->vertex_count - 2][i] = 1;
    }

    for (int i = 0; i < info->second_part; ++i)
    {
        info->graph[info->first_part + i][info->vertex_count - 1] = 1;
        info->residual[info->first_part + i][info->vertex_count - 1] = 1;
    }
}

int EdmondsCarp(Graph* info, int start, int finish)
{
    int flow = 0;

    std::vector<bool> visited(info->vertex_count);
    std::vector<int> parent(info->vertex_count);
    std::queue<int> queue;
    int current = 0;
    int add = 0;
    while (true)
    {
        for (int i = 0; i < info->vertex_count; ++i)
        {
            visited[i] = false;
            parent[i] = no_parent;
        }

        queue.push(start);
        while (!queue.empty())
        {
            int vertex = queue.front();
            queue.pop();

            for (int i = 0; i < info->vertex_count; ++i)
            {
                if (!visited[i] && info->residual[vertex][i] > 0)
                {
                    visited[i] = true;
                    queue.push(i);
                    parent[i] = vertex;
                }
            }
        }

        if (!visited[finish])
        {
            break;
        }

        add = inf;
        current = finish;
        while (current != start)
        {
            add = min(add, info->residual[parent[current]][current]);
            current = parent[current];
        }

        flow += add;
        current = finish;
        while (current != start)
        {
            info->residual[parent[current]][current] -= add;
            info->residual[current][parent[current]] += add;
            current = parent[current];
        }
    }

    return flow;
}

void GetAnswer(Graph* info, std::vector<std::pair<int, int>>& matching)
{
    EdmondsCarp(info, info->vertex_count - 2, info->vertex_count - 1);

    for (int i = 0; i < info->first_part; ++i)
    {
        for (int j = 0; j < info->second_part; ++j)
        {
            if (info->graph[i][info->first_part + j] == 1 &&
                info->residual[i][info->first_part + j] == 0)
            {
                matching.emplace_back(i + 1, j + 1);
            }
        }
    }
}

void PrintAnswer(const std::vector<std::pair<int, int>>& matching)
{
    cout << matching.size() << "\n";
    for (int i = 0; i < matching.size(); ++i) {
        cout << matching[i].first << " " << matching[i].second << "\n";
    }    
}

int main()
{
    Graph info  = {};
    Input(&info);

    std::vector<std::pair<int, int>> matching;
    GetAnswer(&info, matching);
    PrintAnswer(matching);

    return 0;
}
