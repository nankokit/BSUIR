#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
    unordered_map<string, int> map = {{"apple", 1}, {"banana", 2}, {"orange", 3}};
    for (const auto &pair : map)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
    return 0;
}