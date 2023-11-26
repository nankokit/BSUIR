#include <iostream>
#include "source.h"
#include "funcs.h"

int main()
{
    srand((int)time(NULL));
    Time time;
    int hour = 10, minute = 12;
    time.setTime(hour, minute);
    menu();
    return 1;
}