#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int ALPHA_COUNT = 26;

using std::cin;
using std::cout;
using std::string;
using std::vector;
     
//-------------
vector<int> Input     ();
void        GetString (string &text, vector<int> &pi);
void        GetLetter (string &text, vector<int> &pi, vector<bool> &used, int position);
void        GetAnswer (string &text);
//-------------

const char alpha[ALPHA_COUNT] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main()
{
    vector<int> pi = Input();

    string text;

    GetString(text, pi);

    GetAnswer(text);

    return 0;
}

vector<int> Input()
{
    int count = 0;
    cin >> count;

    if(count == 0) {exit(0);}

    vector<int> pi(count);

    for(int i = 0; i < count; ++i)
    {
        cin >> pi[i];
    }

    return pi;
}

void GetString(string &text, vector<int> &pi)
{
    int count = pi.size();

    text.push_back('a');

    for(int i = 1; i < count; ++i)
    {
        vector<bool> used(ALPHA_COUNT, false);
        used[0] = true;

        int position = i;

        if(pi[position] != 0)
        {
            text.push_back(text[pi[position] - 1]);
        }
        else
        {
            GetLetter(text, pi, used, position);
        }
    }
}

void GetLetter(string &text, vector<int> &pi, vector<bool> &used, int position)
{
    while(pi[position - 1] != 0)
    {
        used[text[pi[position - 1]] - 'a'] = true;
        position = pi[position - 1];
    }

    int idx = 0;

    while(used[idx])
    {
        ++idx;
    }

    text.push_back(alpha[idx]);
}

void GetAnswer(string &text)
{
    for(int i = 0; i < text.size(); ++i)
    {
        cout << text[i];
    }
}
