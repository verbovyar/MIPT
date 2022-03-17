#include <iostream>
#include <string>

const int START_IDX      = 5;
const int NUMERALS_COUNT = 7; 

using std::cin;
using std::cout;
using std::string;

string Input         ();
int    Solve         (string line);
int    FindSymbolIdx (char symbol);

int CharRomanNumerals[NUMERALS_COUNT] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};
int IntRomanNumerals [NUMERALS_COUNT] = { 1,   5,   10,  50,  100, 500, 1000};

//---------------------------

int main()
{
    string line = Input();

    cout << Solve(line);

    return 0;
}

//---------------------------

string Input()
{
    string str = "";

    getline(cin, str);

    return str;
}

int Solve(string line)
{
    int sum = 0;

    int lenght = line.size();

    for (int i = START_IDX; i < lenght - 1; ++i)
    {
        int left_idx  = FindSymbolIdx(line[i]);
        int right_idx = FindSymbolIdx(line[i + 1]);

        if (left_idx < right_idx)
        {
            int temp = IntRomanNumerals[right_idx] - IntRomanNumerals[left_idx];
            sum     += temp;

            ++i;
        }
        else
        {
            sum += IntRomanNumerals[left_idx];
        }
    }

    return sum;
}

int FindSymbolIdx(char symbol)
{
    for (int i = 0; i < NUMERALS_COUNT; ++i)
    {
        if (symbol == CharRomanNumerals[i])
        {
            return i;
        }
    }

    return -1;
}
