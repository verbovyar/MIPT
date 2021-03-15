#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
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

inline char invert(int color) 
{
    return color == 1 ? 2 : 1;
}

void Dfs(int vert, char color, vector<vector<int>> &graph, char* color_array, bool* flag) 
{   
    color_array[vert] = color;

    for (int u: graph[vert]) 
    {
        if (color_array[u] == 0) 
        {    
            Dfs(u, invert(color), graph, color_array, flag);
        } 
        else if (color_array[u] == color) 
        {
            *flag = false;
        }
    }
}

int main()
{   
    int vertex = 0;
    int pairs = 0;

    cin >> vertex >> pairs;

    vector<vector<int>> graph(vertex ,vector<int>());

    Enter(pairs, graph);

    char* color_array = (char*)calloc(vertex, sizeof(char));
    bool flag = true;

    for (size_t i = 0; i < vertex; ++i) 
    {
        if (color_array[i] == 0) 
        {
            Dfs(i, 1, graph, color_array, &flag);
        } 
    }

    if (flag)
    {
        cout << "YES";
    }
    else
    {
        cout << "NO";
    }

    free(color_array);

    return 0;
}