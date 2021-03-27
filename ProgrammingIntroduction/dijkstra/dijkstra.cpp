#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int inf = 1e8;

struct Graph
{
    vector<int> dist;
    vector<bool> used;
    int start = 0;
    int finish = 0;
};

Graph* NewGraph(int count)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));

    graph->dist.resize(count, inf);
    graph->used.resize(count);

    return graph;
}

void Enter(int count, vector<vector<int>> &table)
{
    int edge = 0;

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            cin >> edge;

            table[i][j] = edge;
        }
    }
}

void Dijkstra(int count, vector<vector<int>> &table, Graph* graph)
{
    graph->dist[graph->start] = 0;
	for (int i = 0; i < count; ++i) 
    {
		int vertex = -1;
		for (int j = 0; j < count; ++j)
        {
			if (!graph->used[j] && (vertex == -1 || graph->dist[j] < graph->dist[vertex]))
            {
                vertex = j;
            }
        }
		if (graph->dist[vertex] == inf)
        {
			break;
        }
		graph->used[vertex] = true;
		for (int j = 0; j < count; ++j) 
        {
            if (table[vertex][j] != -1)
            {
                int to = j;
				int len = table[vertex][j];
                if (graph->dist[vertex] + len < graph->dist[to]) 
                {
                    graph->dist[to] = graph->dist[vertex] + len;
                }
            }
		}
	}
}

void GetAnswer(Graph* graph)
{
    if (graph->dist[graph->finish] != inf)
    {
        cout << graph->dist[graph->finish];
    }
    else
    {
        cout << "-1";
    }
}

int main()
{
    int count = 0;

    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> count;

    Graph* graph = NewGraph(count);

    cin >> graph->start >> graph->finish;
    --graph->start;
    --graph->finish;

    vector<vector<int>> table(count, vector<int>(count));

    Enter(count, table);

    Dijkstra(count, table, graph);

    GetAnswer(graph);

    return 0;
}
