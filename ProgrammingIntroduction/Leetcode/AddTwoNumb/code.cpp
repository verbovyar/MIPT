#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

const int MAX_LEN   = 256;
const int MAX_SIZE  = 101;
const int FIRST_NUM = 1;

using std::cin;
using std::cout;
using std::string;

//-----------------

struct List {
    int* number       = nullptr;
    int  number_count = 0;
};

string GetString ();
List*  NewList   ();
void   Free      (List* list);

//-----------------

struct Solution {
    List list[2];

    List* GetNumber   (string string);
    void  AddNumbers  ();
    void  Output      ();
};

List* Solution::GetNumber(string string)
{
    int lenght = string.size() - 1;

    List* list = NewList();

    for (int i = FIRST_NUM; i < lenght; i += 3)
    {
        list->number[list->number_count] = string[i] - '0';
        ++list->number_count;
    }

    return list;
}

void Solution::AddNumbers()
{
    int count = 0;

    if (list[0].number_count > list[1].number_count)
    {
        count  = list[0].number_count;
    }
    else
    {
        List temp = list[0];
        list[0]   = list[1];
        list[1]   = temp;
        count     = list[1].number_count;
    }

    for (int i = 0; i < count; ++i)
    {
        int sum = list[0].number[i] + list[1].number[i];

        if (sum >= 10)
        {
            list[0].number[i + 1] += sum / 10;
            list[0].number[i] = sum - 10;
        }
        else
        {
            list[0].number[i] = sum;
        }
    }
}

void Solution::Output()
{
    cout << "[";

    if (list[0].number[list[0].number_count] != 0)
    {
        cout << list[0].number[list[0].number_count] << ", ";
    }

    for (int i = list[0].number_count - 1; i > 0; --i)
    {
        cout << list[0].number[i] << ", ";
    }

    cout << list[0].number[0] << "]";
}

//-----------------

int main()
{
    string first_string  = GetString();
    string second_string = GetString();

    Solution solution = {};
    solution.list[0]  = *solution.GetNumber(first_string);
    solution.list[1]  = *solution.GetNumber(second_string);

    solution.AddNumbers();

    solution.Output();

    Free(solution.list);

    return 0;
}

string GetString()
{
    string str;

    getline(cin, str);

    return str;
}

List* NewList()
{
    List* list = (List*)calloc(1, sizeof(List));

    list->number_count = 0;
    list->number       = (int*)calloc(MAX_SIZE, sizeof(int));

    return list;
}

void Free(List* list)
{
    free(list->number);
    free(list);
}
