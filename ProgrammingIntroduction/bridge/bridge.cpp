#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

struct Bridge {
    vector<int> answer;
    bool* used = nullptr;
    int* intime = nullptr;
    int* fup = nullptr;
    int timer = 0;
    int count = 0;
};

struct Edge {
    int to = 0;
    int numb_of_edge = 0;
};

void Input(int vertex, int edges_count, vector<vector<Edge>> &graph)
{
    int vertex1 = 0;
    int vertex2 = 0;

    for (int i = 0; i < edges_count; ++i)
    {
        Edge first_struct = {};
        first_struct.numb_of_edge = i;
        Edge second_struct = {};
        second_struct.numb_of_edge = i;

        cin >> vertex1 >> vertex2;

        first_struct.to = vertex2 - 1;
        second_struct.to = vertex1 - 1;

        graph[vertex1 - 1].push_back(first_struct);
        graph[vertex2 - 1].push_back(second_struct);
    }
}

Bridge* NewBridge(int vertex)
{
    Bridge* bridge = (Bridge*)calloc(1, sizeof(Bridge));

    bridge->timer = 0;
    bridge->count = 0;
    bridge->answer.resize(0);
    bridge->fup = (int*)calloc(vertex, sizeof(int));
    bridge->intime = (int*)calloc(vertex, sizeof(int));
    bridge->used = (bool*)calloc(vertex, sizeof(bool));

    return bridge;
}

void GetBridge(int edge, Bridge* bridge)
{
    bridge->answer.push_back(edge + 1);
}

void Dfs(int vertex, Bridge* bridge, vector<vector<Edge>> &graph, int parent = -1)
{
    bridge->used[vertex] = true;
    ++bridge->timer;
	bridge->intime[vertex] = bridge->fup[vertex] = bridge->timer;

	for (int i = 0; i < graph[vertex].size(); ++i) 
    {
		int to = graph[vertex][i].to;
		if (to == parent)
        {
            continue;
        }
		if (bridge->used[to])
        {
			bridge->fup[vertex] = std::min(bridge->fup[vertex], bridge->intime[to]);
        }
		else 
        {
			Dfs (to, bridge, graph, vertex);
			bridge->fup[vertex] = std::min(bridge->fup[vertex], bridge->fup[to]);
			if (bridge->fup[to] > bridge->intime[vertex])
            {
                ++bridge->count;
				GetBridge(graph[vertex][i].numb_of_edge, bridge);
            }
		}
	}
}

void FindBridge(int vertex, Bridge* bridge, vector<vector<Edge>> &graph)
{
    for (int i = 0; i < vertex; ++i)
    {
		bridge->used[i] = false;
    }

	for (int i = 0; i < vertex; ++i)
    {
		if (!bridge->used[i])
        {
			Dfs (i, bridge, graph);
        }
    }
}

void GetAnswer(Bridge* bridge)
{
    cout << bridge->count << std::endl;

    sort(bridge->answer.begin(), bridge->answer.end());

    for (int i = 0; i < bridge->answer.size(); ++i)
    {
        cout << bridge->answer[i] << " ";
    }
}

void FreeBridge(Bridge* bridge)
{
    bridge->timer = 0;
    bridge->count = 0;
    free(bridge->used);
    free(bridge->fup);
    free(bridge->intime);
}

int main()
{
    int vertex = 0;
    int edges_count = 0;

    cin >> vertex >> edges_count;

    vector<vector<Edge>> graph(vertex, vector<Edge>());

    Input(vertex, edges_count, graph);

    Bridge* bridge = NewBridge(vertex);

    FindBridge(vertex, bridge, graph);

    GetAnswer(bridge);
    
    FreeBridge(bridge);

    return 0;
}