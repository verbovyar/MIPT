#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int inf = 30000;

struct Table
{
    int to = 0;
    int start = 0;
    int edge = 0;
};

void Input(int count, vector<vector<Table>> &table)
{   
    int vertex1 = 0;
    int vertex2 = 0;
    int edge = 0;

    for (int i = 0; i < count; ++i)
    {
        cin >> vertex1 >> vertex2 >> edge;
        --vertex1;
        --vertex2;

        Table first = {};

        first.start = vertex1;
        first.to = vertex2;
        first.edge = edge;

        table[i].push_back(first);
    }
}

void BellmanFord(vector<vector<Table>> &graph, vector<int> &dist, int edge_count, int vertex_count)
{
    dist[0] = 0;

    for (int i = 0; i < vertex_count; ++i)
		for (int j = 0; j < edge_count; ++j)
			if (dist[graph[j][0].start] < inf)
				dist[graph[j][0].to] = std::min(dist[graph[j][0].to], dist[graph[j][0].start] + graph[j][0].edge);
}

void GetAnswer(vector<int> &dist, int vertex_count)
{   
    for (int i = 0; i < vertex_count; ++i)
    {
        cout << dist[i] << " ";
    }
}

int main()
{
    int vertex_count = 0;
    int edge_count = 0;

    cin >> vertex_count >> edge_count;

    vector<vector<Table>> graph(edge_count, vector<Table>(0));
    Input(edge_count, graph);

    vector<int> dist(vertex_count, inf);
    BellmanFord(graph, dist, edge_count, vertex_count);

    GetAnswer(dist, vertex_count);

    return 0;
}