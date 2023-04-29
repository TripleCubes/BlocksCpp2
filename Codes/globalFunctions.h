#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <functional>
#include <vector>

int mod(int a, int b);
bool isMax(float num1, float num2, float num3);
bool isMin(float num1, float num2, float num3);

template <typename ListType, typename FindType>
int findInList(const std::vector<ListType> &list, FindType find, 
                const std::function<FindType(ListType)> &findFunction)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (findFunction(list[i]) == find)
        {
            return i;
        }
    }

    return -1;
}

template <typename FindType>
int findInList(const std::vector<FindType> &list, FindType find)
{
    int i = std::find(list.begin(), list.end(), find);
    if (i != list.end())
    {
        return i;
    }

    return -1;
}

#endif