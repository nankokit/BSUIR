#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <windows.h>

namespace randomsheep
{
    class vector{
        
    };
} // namespace name

struct Cell
{
    int candidate;
    int region;

    Cell()
    {
        candidate = 0;
        region = 0;
    }
};

int random(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

void inputDimension(int &n, int &m)
{
    std::cout << "Enter dimension (min 6x6) :\n";
    do
    {
        std::cin >> n >> m;
    } while (n < 6 || m < 6);
}

void inputNumOfCandidates(int &num)
{
    std::cout << "Enter number of candidates (min 2) : \n";
    do
    {
        std::cin >> num;
    } while (num < 2);
}

void inputNumOfRegions(int &num)
{
    std::cout << "Enter number of regions (min 5) : \n";
    do
    {
        std::cin >> num;
    } while (num < 5);
}

void printMatrix(std::vector<std::vector<Cell>> &matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            std::cout << std::setw(2) << matrix[i][j].region << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void makeRegion(std::vector<std::vector<Cell>> &matrix, int n, int m, int maxSize, int i, int j, int regionNumber, std::vector<std::vector<Cell *>> &regionsElements)
{
    std::queue<std::pair<int, int>> q;
    int size = 0;
    q.push({i, j});
    while (size != maxSize && !q.empty())
    {
        std::pair<int, int> p;
        p = q.front();
        i = p.first;
        j = p.second;
        q.pop();
        if (matrix[i][j].region != 0)
            continue;
        matrix[i][j].region = regionNumber;
        regionsElements[regionNumber - 1].push_back(&matrix[i][j]);
        size++;
        if (i != 0 && matrix[i - 1][j].region == 0)
        {
            q.push({i - 1, j});
        }
        if (j != 0 && matrix[i][j - 1].region == 0)
        {
            q.push({i, j - 1});
        }
        if (i != n - 1 && matrix[i + 1][j].region == 0)
        {
            q.push({i + 1, j});
        }
        if (j != m - 1 && matrix[i][j + 1].region == 0)
        {
            q.push({i, j + 1});
        }
    }
}

void divideToRegions(int n, int m, int regions, std::vector<std::vector<Cell>> &matrix, std::vector<std::vector<Cell *>> &regionsElements)
{
    int maxSize = n * m / regions;
    int istart = random(0, n - 1);
    int jstart = random(0, m - 1);
    makeRegion(matrix, n, m, maxSize, istart, jstart, 1, regionsElements);
    int q = 2;

    for (int i = 0; i < n && q <= regions; i++)
    {
        for (int j = 0; j < m && q <= regions; j++)
        {
            if (matrix[i][j].region == 0)
            {
                makeRegion(matrix, n, m, maxSize, i, j, q, regionsElements);
                q++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (matrix[i][j].region == 0)
            {
                if (i != 0 && matrix[i - 1][j].region != 0)
                {
                    matrix[i][j].region = matrix[i - 1][j].region;
                    regionsElements[matrix[i - 1][j].region - 1].push_back(&matrix[i][j]);
                }
                else if (j != 0 && matrix[i][j - 1].region != 0)
                {
                    matrix[i][j].region = matrix[i][j - 1].region;
                    regionsElements[matrix[i][j - 1].region - 1].push_back(&matrix[i][j]);
                }
                else if (i != n - 1 && matrix[i + 1][j].region != 0)
                {
                    matrix[i][j].region = matrix[i + 1][j].region;
                    regionsElements[matrix[i + 1][j].region - 1].push_back(&matrix[i][j]);
                }
                else if (j != m - 1 && matrix[i][j + 1].region != 0)
                {
                    matrix[i][j].region = matrix[i][j + 1].region;
                    regionsElements[matrix[i][j + 1].region - 1].push_back(&matrix[i][j]);
                }
            }
        }
    }
}

void countVoices(std::vector<Cell *> &region, const int candidatesNumber)
{
    int i;
    std::vector<int> a(candidatesNumber);

    for (i = 0; i < region.size(); i++)
    {
        a[region[i]->candidate - 1]++;
    }

    int maxVoices = a[0], maxCell = 0, nullRegion = 0;
    for (i = 0; i < candidatesNumber; i++)
    {
        if (a[i] > maxVoices)
        {
            maxVoices = a[i];
            maxCell = i;
        }
    }
    for (i = 0; i < candidatesNumber; i++)
    {
        if (a[i] == maxVoices)
            nullRegion++;
    }
    if (nullRegion >= 2)
    {
        for (i = 0; i < region.size(); i++)
            region[i]->candidate = 0;
    }
    else
    {
        for (i = 0; i < region.size(); i++)
            region[i]->candidate = maxCell + 1;
    }
}

int main()
{
    srand(time(0));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int n, m, candidates, regions;
    inputDimension(n, m);
    inputNumOfRegions(regions);
    inputNumOfCandidates(candidates);
    std::vector<std::vector<Cell>> matrix(n, std::vector<Cell>(m));
    std::vector<std::vector<Cell *>> regionsElements(regions);
    divideToRegions(n, m, regions, matrix, regionsElements);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            matrix[i][j].candidate = random(1, candidates);
    }
    std::cout << "Divided to regions:\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            SetConsoleTextAttribute(hConsole, matrix[i][j].region + 3);
            std::cout << std::setw(2) << matrix[i][j].candidate << " ";
            SetConsoleTextAttribute(hConsole, 7);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < regions; i++)
        countVoices(regionsElements[i], candidates);

    std::cout << "After counting the votes" << std::endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            SetConsoleTextAttribute(hConsole, matrix[i][j].region + 3);
            std::cout << std::setw(2) << matrix[i][j].candidate << " ";
            SetConsoleTextAttribute(hConsole, 7);
        }
        std::cout << std::endl;
    }

    return 0;
}