#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::vector;

enum Color{
    white,
    grey,
    black
};

struct Cycle {
    int vertex = 0;
    int parent = 0;
    int cycle_st = -1;
    int cycle_end = -1;
    bool flag = true;
};

void Enter(int ribs, vector<vector<int>> &graph)
{
    for (int i = 0; i < ribs; ++i)
    {
        int vert1 = 0;
        int vert2 = 0;

        cin >> vert1 >> vert2;

        graph[vert1 - 1].push_back(vert2 - 1);
    }
}

void Dfs(Cycle* cycle, vector<vector<int>> &graph, vector<int> &parents, vector<Color> &colors)
{
    colors[cycle->vertex] = grey;

    for (size_t i = 0; i < graph[cycle->vertex].size(); ++i)
    {
        int u = graph[cycle->vertex][i];
        if (colors[u] == white)
        {
            parents[u] = cycle->vertex;
            if (cycle->flag)
            {
                Dfs(cycle, graph, parents, colors);
                cycle->flag = true;
                return;
            }
        }
        else if (colors[u] == grey)
        {
            cycle->cycle_end = cycle->vertex;
            cycle->cycle_st = u;

            cycle->flag = true;
            return;
        }
    }
    colors[cycle->vertex] = black;
    cycle->flag = false;
}

void GetAnswer(Cycle* cycle, vector<int> &parents)
{
    if (cycle->cycle_st == -1)
    {
		cout << "NO";
    }
	else 
    {
        cout << "YES\n";
        stack<int> answ;
        answ.push (cycle->cycle_st);
        for (int v = cycle->cycle_end; v != cycle->cycle_st; v = parents[v])
        {
            answ.push (v);   
        }
        while (!answ.empty())
        {
            cout << answ.top() + 1 << " ";
            answ.pop();
        }
	}
}

int main()
{
    int vertex = 0;
    int ribs = 0;

    cin >> vertex >> ribs;

    vector<vector<int>> graph(vertex, vector<int>());

    Enter(ribs, graph);

    vector<Color> colors(vertex, white);
    vector<int> parents(vertex, -1);

    Cycle* cycle = (Cycle*)calloc(1, sizeof(Cycle));

    for (int i = 0; i < vertex; i++) 
    {   
        cycle->vertex = i;
        cycle->parent = -1;

        if (cycle->flag) 
        {
            break;
        }
    }

    GetAnswer(cycle, parents);

    return 0;
}