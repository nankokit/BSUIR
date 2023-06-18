#include <time.h>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
    FILE *fp;
    fp = fopen("top-1m.txt", "r");
    char *key, *value;
    key = (char *)malloc(255);
    value = (char *)malloc(255);
    unordered_map<char *, char *> m;
    double time_spent = 0.0;
    clock_t begin = clock();
    while (!feof(fp))
    {
        fscanf(fp, "%s%s", key, value);
        m[key] = value;
    }
    clock_t end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf\n", time_spent);
    return 0;
}