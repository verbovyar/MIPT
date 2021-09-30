#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

vector<int> PreficsFunction(string &text, string &pattern);
void        GetAnswer      (vector<int> &pi, int pattern_length, int text_lenght, int temp_length);

int main()
{
    string text;
    string pattern;

    cin >> text >> pattern;

    vector<int> pi = PreficsFunction(text, pattern);

    int temp_length = pattern.size() + text.size() + 1;

    GetAnswer(pi, pattern.size(), text.size(), temp_length);

    return 0;
}

vector<int> PreficsFunction(string &text, string &pattern)
{
    string temp = pattern + "#" + text;

    int temp_length = temp.size();

    vector<int> pi(temp_length);

    for(int i = 1; i < temp_length; ++i)
    {
        int position = pi[i - 1];

        while(position > 0 && temp[i] != temp[position])
        {
            position = pi[position - 1];
        }

        if(temp[i] == temp[position])
        {
            ++position;
        }

        pi[i] = position;
    }

    return pi;
}

void GetAnswer(vector<int> &pi, int pattern_length, int text_lenght, int temp_length)
{
    for(int idx = pattern_length; idx < temp_length; ++idx)
    {
        if(pi[idx] == pattern_length)
        {
            printf("%d\n", idx - 2 * pattern_length);
        }
    }
}