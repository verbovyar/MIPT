#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

struct Cutpoint {
    bool* used = nullptr;
    int* intime = nullptr;
    int* fup = nullptr;
    vector<bool> answer;
    int timer = 0;
    int count = 0;
};

void Enter(int ribs, vector<vector<int>> &graph)
{
    for (int i = 0; i < ribs; ++i)
    {
        int vert1 = 0;
        int vert2 = 0;

        cin >> vert1 >> vert2;

        graph[vert1 - 1].push_back(vert2 - 1);
        graph[vert2 - 1].push_back(vert1 - 1);
    }
}

Cutpoint* NewCutpoint(int vertex)
{
    Cutpoint* cutpoint = (Cutpoint*)calloc(1, sizeof(Cutpoint));

    cutpoint->timer = 0;
    cutpoint->count = 0;
    cutpoint->fup = (int*)calloc(vertex, sizeof(int));
    cutpoint->intime = (int*)calloc(vertex, sizeof(int));
    cutpoint->used = (bool*)calloc(vertex, sizeof(bool));
    cutpoint->answer.resize(vertex, false);

    return cutpoint;
}

void Dfs (int vertex, Cutpoint* cutpoint, vector<vector<int>> &graph, int parent = -1) 
{
	cutpoint->used[vertex] = true;
	cutpoint->intime[vertex] = cutpoint->fup[vertex] = cutpoint->timer;
    ++cutpoint->timer;
	int children = 0;
	for (int i = 0; i < graph[vertex].size(); ++i) 
    {
		int to = graph[vertex][i];
		if (to == parent)
        {
            continue;
        }  
            
		if (cutpoint->used[to])
        {
			cutpoint->fup[vertex] = std::min(cutpoint->fup[vertex], cutpoint->intime[to]);
        }
		else 
        {
			Dfs (to, cutpoint, graph, vertex);
			cutpoint->fup[vertex] = std::min(cutpoint->fup[vertex], cutpoint->fup[to]);
			if (cutpoint->fup[to] >= cutpoint->intime[vertex] && parent != -1)
            {
                ++cutpoint->count;
				cutpoint->answer[vertex] = true;
            }
			++children;
		}
	}
	if (parent == -1 && children > 1)
    {
        ++cutpoint->count;
		cutpoint->answer[vertex] = true;
    }
}

void FindCutpoint(int vertex, Cutpoint* cutpoint, vector<vector<int>> &graph)
{
    for (int i = 0; i < vertex; ++i)
    {
		cutpoint->used[i] = false;
    }

	for (int i = 0; i < vertex; ++i)
    {
		if (!cutpoint->used[i])
        {
			Dfs (i, cutpoint, graph);
        }
    }
}

void GetAnswer (Cutpoint* cutpoint, int vertex)
{
    cout << cutpoint->count << "\n";

    for (int i = 0; i < vertex; ++i)
    {
        if (cutpoint->answer[i])
        {
            cout << i + 1 << "\n";
        }
    }
}

void FreeCutpoint(Cutpoint* cutpoint)
{
    cutpoint->timer = 0;
    cutpoint->count = 0;
    free(cutpoint->used);
    free(cutpoint->fup);
    free(cutpoint->intime);
}

int main()
{
    int vertex = 0;
    int edges_count = 0;

    cin >> vertex >> edges_count;

    vector<vector<int>> graph(vertex ,vector<int>());

    Enter(edges_count, graph);

    Cutpoint* cutpoint = NewCutpoint(vertex);

    cutpoint->timer = 0;
    for (int i = 0; i < vertex; ++i)
    {
        cutpoint->used[i] = false;
    }

    FindCutpoint(vertex, cutpoint, graph);

    GetAnswer(cutpoint, vertex);

    FreeCutpoint(cutpoint);
    free(cutpoint);

    return 0;
}