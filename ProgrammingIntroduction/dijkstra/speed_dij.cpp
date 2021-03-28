#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::make_pair;
using std::pair;
using std::priority_queue;
using std::vector;

using Heap = std::priority_queue<pair<int, int>,
                                 vector<pair<int, int>>,
                                 std::less<>>;

const int inf = 2009000999;

struct Table
{
    int to = 0;
    int edge = 0;
};

struct Graph
{
    vector<int> dist;
    Heap q;
    int start = 0;
};

void Enter(int count, vector<vector<Table>> &table)
{   
    int vertex1 = 0;
    int vertex2 = 0;
    int edge = 0;

    for (int i = 0; i < count; ++i)
    {
        cin >> vertex1 >> vertex2 >> edge;

        Table first = {};
        Table second = {};

        first.to = vertex2;
        second.to = vertex1;
        
        first.edge = edge;
        second.edge = edge;

        table[vertex1].push_back(first);
        table[vertex2].push_back(second);
    }
}

Graph* NewGraph(int count)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));

    graph->dist.resize(count, inf);
    graph->start = 0;

    return graph;
}

void Dijkstra(int count, vector<vector<Table>> &table, Graph* graph)
{   
    graph->dist[graph->start] = 0;
	graph->q.push(make_pair(0, graph->start));
	while (!graph->q.empty()) 
    {
		int vertex = graph->q.top().second;
        int cur_dist = -graph->q.top().first;
		graph->q.pop();
		if (cur_dist > graph->dist[vertex])
        {
            continue;
        } 
		for (int j = 0; j < table[vertex].size(); ++j) 
        {
			int to = table[vertex][j].to;
		    int len = table[vertex][j].edge;
			if (graph->dist[vertex] + len < graph->dist[to]) 
            {
				graph->dist[to] = graph->dist[vertex] + len;
				graph->q.push(make_pair(-graph->dist[to], to));
			}
		}
	}
}

void GetAnswer(int count, Graph* graph)
{
    for (int i = 0; i < count; ++i)
    {
        cout << graph->dist[i] << " ";
    }
}

int main()
{
    int num = 0;
    int edge = 0;
    int vertex = 0;

    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> num;

    for (int i = 0; i < num; ++i)
    {
        cin >> vertex >> edge;

        vector<vector<Table>> table(vertex, vector<Table>(0));

        Enter(edge, table);

        Graph* graph = NewGraph(vertex);
        cin >> graph->start;

        Dijkstra(vertex, table, graph);

        GetAnswer(vertex, graph);
    }

    return 0;
}