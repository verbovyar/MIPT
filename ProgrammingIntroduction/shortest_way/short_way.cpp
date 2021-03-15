#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::vector;

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

void Bfs(int start, int vertexs,vector<vector<int>> &graph, vector<bool> &used, vector<int> &parents)
{
    queue<int> v_queue;
    vector<int> ways(vertexs);

    v_queue.push(start);
    used[start] = true;
    parents[start] = -1;
    ways[start] = 0;

    while (!v_queue.empty())
    {
        int v = v_queue.front();
        v_queue.pop();
        for (int i = 0; i < graph[v].size(); ++i)
        {
            int to = graph[v][i];

		    if (!used[to]) 
            {
                used[to] = true;
                v_queue.push (to);
                ways[to] = ways[v] + 1;
                parents[to] = v;
		    }
        }
    }
}

void GetAnswer(int end, vector<bool> &used, vector<int> &parents)
{
    vector<int> path;

    if (!used[end])
    {
	    cout << "-1";
    }
    else 
    {
        for (int v = end; v != -1; v = parents[v])
        {
            path.push_back (v);
        }

	    reverse (path.begin(), path.end());

	    for (int i = 0; i < path.size(); ++i)
        {
		    cout << path[i] + 1 << " ";
        }
    }
}

int main()
{
    int vertexs = 0;
    int ribs = 0;

    cin >> vertexs >> ribs;

    int start = 0;
    int end = 0;

    cin >> start >> end;

    --start;
    --end;

    vector<vector<int>> graph(ribs ,vector<int>());

    Enter(ribs, graph);

    vector<bool> used(vertexs);
    vector<int> parents(vertexs);
    
    Bfs(start, vertexs, graph, used, parents);

    GetAnswer(end, used, parents);

    return 0;
}