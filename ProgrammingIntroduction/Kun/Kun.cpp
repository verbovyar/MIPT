#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

void Input    (vector<vector<int>> &graph, int first_part);
bool Khun     (vector<vector<int>> &graph, vector<int> &matching, vector<bool> &used, int vertex);
void GetAnswer(vector<int> &matching, int second_part);

int main()
{
    int first_part  = 0;
    int second_part = 0;

    cin >> first_part >> second_part;

    vector<vector<int>> graph(first_part);

    Input(graph, first_part);

    vector<int> matching(second_part, -1);
    vector<bool> used;

    for(int i = 0; i < first_part; ++i)
    {
        used.assign(first_part, false);
        Khun(graph, matching, used, i);
    }

    GetAnswer(matching, second_part);

    return 0;
}

void Input(vector<vector<int>> &graph, int first_part)
{
    for(int i = 0; i < first_part; ++i)
    {
        int vertex = 0;
        while(true)
        {
            cin >> vertex;
            if (vertex == 0) {
                break;
            }
            --vertex;
            graph[i].push_back(vertex);
        }
    }
}

bool Khun(vector<vector<int>> &graph, vector<int> &matching, vector<bool> &used, int vertex)
{
    if (used[vertex])
    {
        return false;
    }  
        
	used[vertex] = true;
	for (int64_t to : graph[vertex]) 
    {
		if (matching[to] == -1 || Khun(graph, matching, used, matching[to])) 
        {
			matching[to] = vertex;
			return true;
		}
	}

	return false;
}

void GetAnswer(vector<int> &matching, int second_part)
{
    int count = 0;
    for (int i = 0; i < second_part; ++i)
    {
        if (matching[i] != -1)
        {
			++count;
        }
    
    }
    cout << count << "\n";

    for (int i = 0; i < second_part; ++i)
    {
        if (matching[i] != -1)
        {
			printf ("%d %d\n", matching[i]+1, i+1);
        }
    }
}