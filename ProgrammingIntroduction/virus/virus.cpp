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

using Heap = std::priority_queue<pair<long long int, long long int>,
                                 vector<pair<long long int, long long int>>,
                                 std::less<>>;

const long long int inf = 1e10;

struct Virus
{
    long long int country = 0;
    long long int road = 0;
    long long int virus_count = 0;
};

struct Table
{
    long long int to = 0;
    long long int edge = 0;
};

struct Graph
{
    vector<long long int> dist;
    Heap q;
    long long int start = 0;
    long long int finish = 0;
};

void InputVirus(vector<long long int> &virus_countries, long long int count)
{
    for (long long int i = 0; i < count; ++i)
    {
        cin >> virus_countries[i];
        --virus_countries[i];
    }
}

void Enter(long long int count, vector<vector<Table>> &table)
{   
    long long int vertex1 = 0;
    long long int vertex2 = 0;
    long long int edge = 0;

    for (long long int i = 0; i < count; ++i)
    {
        cin >> vertex1 >> vertex2 >> edge;
        --vertex1;
        --vertex2;

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

Graph* NewGraph(long long int count)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));

    graph->dist.resize(count, inf);
    graph->start = 0;

    return graph;
}

void Dijkstra(long long int count, vector<vector<Table>> &table, Graph* graph)
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
		for (long long int j = 0; j < table[vertex].size(); ++j) 
        {
			long long int to = table[vertex][j].to;
		    long long int len = table[vertex][j].edge;
			if (graph->dist[vertex] + len < graph->dist[to]) 
            {
				graph->dist[to] = graph->dist[vertex] + len;
				graph->q.push(make_pair(-graph->dist[to], to));
			}
		}
	}
}

void GetAnswer(Graph* graph, vector<long long int> &virus_countries)
{
    for (int i = 0; i < virus_countries.size(); ++i)
    {
        if (graph->dist[graph->finish] >= graph->dist[virus_countries[i]])
        {
            cout << "-1";
            return;
        }
    }

    cout << graph->dist[graph->finish];
}

int main()
{
    Virus* virus_info = (Virus*)calloc(1, sizeof(Virus));
    cin >> virus_info->country >> virus_info->road >> virus_info->virus_count;

    vector<long long int> virus_countries(virus_info->virus_count);
    InputVirus(virus_countries, virus_info->virus_count);

    vector<vector<Table>> table(virus_info->country, vector<Table>(0));
    Enter(virus_info->road, table);

    Graph* graph = NewGraph(virus_info->country);
    cin >> graph->finish >> graph->start;
    --graph->start;
    --graph->finish;

    Dijkstra(virus_info->country, table, graph);

    GetAnswer(graph, virus_countries);

    return 0;
}