#include <iostream>
#include <string>
#include <vector>

std::string encryption(std::string str, int key)
{
    std::vector<std::vector<char>> array(key, std::vector<char>(str.size(), ' '));
    int flag = 0;
    for (int i = 0, j = 0; i < str.size(); i++)
    {
        array[j][i] = str[i];

        if (j == key - 1)
        {
            flag = 1;
        }
        else if (j == 0)
            flag = 0;

        if (flag == 0)
        {
            j++;
        }
        else
            j--;
    }

    std::string result;
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < str.size(); j++)
        {
            std::cout << array[i][j];
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < str.size(); j++)
        {
            if (array[i][j] != ' ')
                result += array[i][j];
        }
    }
    return result;
}

std::string decryption(std::string str, int key)
{
    std::vector<std::vector<char>> array(key, std::vector<char>(str.size(), ' '));
    int flag = 0, temp = 0;
    for (int i = 0, j = 0; i < str.size(); i++)
    {
        array[j][i] = '*';

        if (j == key - 1)
        {
            flag = 1;
        }
        else if (j == 0)
            flag = 0;

        if (flag == 0)
        {
            j++;
        }
        else
            j--;
    }
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < str.size(); j++)
        {
            if (array[i][j] == '*')
                array[i][j] = str[temp++];
        }
    }

    std::string result;
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < str.size(); j++)
        {
            std::cout << array[i][j];
        }
        std::cout << std::endl;
    }
    for (int i = 0, j = 0; i < str.size(); i++)
    {
        result += array[j][i];

        if (j == key - 1)
        {
            flag = 1;
        }
        else if (j == 0)
            flag = 0;

        if (flag == 0)
        {
            j++;
        }
        else
            j--;
    }
    return result;
}

int main()
{
    std::string str;
    int key, action;

    while (1)
    {
        std::cout << "Enter action: 1 - encryption, 2 - decryption\n";
        std::cin >> action;
        if (action == 1)
        {
            std::cout << "Enter message for encryption\n";
            std::cin >> str;
            std::cout << "Enter key\n";
            std::cin >> key;
            str = encryption(str, key);
            std::cout << "Result encryption: " << str << std::endl;
        }
        else if (action == 2)
        {
            std::cout << "Enter message for decryption\n";
            std::cin >> str;
            std::cout << "Enter key\n";
            std::cin >> key;
            str = decryption(str, key);
            std::cout << "Result decryption: " << str << std::endl;
        }
        else
            break;
    }
}