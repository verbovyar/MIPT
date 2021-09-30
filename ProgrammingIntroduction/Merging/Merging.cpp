#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::min;
using std::string;
using std::vector;

vector<int> PreficsFunction(string &text, string &pattern);
void        GetNewString   (string &result, string &word, vector<int> &pi);
void        OutPut         (string &result);

int main()
{
    int count = 0;
    cin >> count;

    string result;
    cin >> result;

    string word;

    for(int i = 1; i < count; ++i)
    {
        cin >> word;

        int size = min(result.size(), word.size());

        string temp_word = result.substr(result.size() - size, size);

        vector<int> pi = PreficsFunction(temp_word, word);

        GetNewString(result, word, pi);
    }

    OutPut(result);

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

void GetNewString(string &result, string &word, vector<int> &pi)
{
    result += word.substr(pi[pi.size() - 1], word.size() - pi[pi.size() - 1]);
}

void OutPut(string &result)
{
    for(int i = 0; i < result.size(); ++i)
    {
        cout << result[i];
    }
}
