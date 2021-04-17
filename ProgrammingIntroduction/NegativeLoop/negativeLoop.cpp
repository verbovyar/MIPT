#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
 
struct Edge
{
    int from = 0;
    int to = 0;
    int edge_weight = 0;
};

struct Loop
{
    vector<int> dist;
    vector<int> parents;
    int answer = 0;
};

const int inf = 1e9;

void Input(int count, vector<Edge> &graph)
{
    for(int i = 0; i < count; ++i)
    {
        for(int j = 0; j < count; ++j)
        {
            int edge;
            cin >> edge;
            if(edge != 0 && edge != 100000)
            {
                graph.push_back({i, j, edge});
            }
        }
    }
}

Loop* NewLoop(int count)
{   
    Loop* loop_info = (Loop*)calloc(1, sizeof(Loop));

    loop_info->answer = 0;
    loop_info->dist.resize(count, inf);
    loop_info->parents.resize(count, -1);

    return loop_info;
}

void NegativeLoop(int count, Loop* loop_info, vector <Edge> &graph)
{
    loop_info->dist[0] = 0;
    for(int i = 0; i < count; ++i)
    {
        loop_info->answer = -1;
        for(int j = 0; j < graph.size(); ++j)
        {
            int from = graph[j].from;
            int to = graph[j].to;
            int edge_weight = graph[j].edge_weight;
            if(loop_info->dist[to] > loop_info->dist[from] + edge_weight )
            {

                loop_info->dist[to] = std::max(loop_info->dist[from] + edge_weight, -inf);
                loop_info->parents[to] = from;
                loop_info->answer = to;
            }
        }
    }
}

void GetAnswer(Loop* loop_info, int count)
{
    if(loop_info->answer == -1)
    {
        cout << "NO" << "\n";
    }
    else
    {
        int y = loop_info->answer;
        for(int i = 0; i < count; i++)
        {
            y = loop_info->parents[y];
        }
        vector <int> path;
        for(int cur = y; ; cur = loop_info->parents[cur])
        {
            path.push_back(cur);
            if(cur == y && path.size() > 1)
            {
                break;
            }
        }
        reverse(path.begin(), path.end());
        cout << "YES" << "\n";
        cout << path.size() << "\n";
        for(int i = 0; i < path.size(); i++)
        {
            cout << path[i] + 1;
            if(i != path.size() - 1)
            {
                cout << ' ';
            }
        }
    }
}
 
int main()
{
    int count;
    cin >> count;

    vector <Edge> graph;
    Input(count, graph); 

    Loop* loop_info = NewLoop(count);
    NegativeLoop(count, loop_info, graph);

    GetAnswer(loop_info, count);
    
    return 0;
}