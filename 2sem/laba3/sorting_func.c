#include "functions.h"

int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition)
{
    switch (condition)
    {
    case 1:
        return (a.id > b.id);
        break;
    case 2:
        return (strcmp(a.name, b.name) > 0);
        break;
    case 3:
    {
        if (a.year_deadline == b.year_deadline)
        {
            if (a.month_deadline == b.month_deadline)
            {
                return (a.day_deadline > b.day_deadline);
            }
            else
                return (a.month_deadline > b.month_deadline);
        }
        else
            return (a.year_deadline > b.year_deadline);
        break;
    }
    case 4:
        return (a.cost > b.cost);
        break;
    case 5:
        return (a.status > b.status);
        break;
    default:
        break;
    };
}

int equal_condition(ITCOMPANY a, ITCOMPANY b, int condition)
{
    switch (condition)
    {
    case 1:
        return (a.id == b.id);
        break;
    case 2:
        return (strcmp(a.name, b.name) == 0);
        break;
    case 3:
    {
        if (a.year_deadline == b.year_deadline)
        {
            if (a.month_deadline == b.month_deadline)
            {
                return (a.day_deadline == b.day_deadline);
            }
        }
        else
            return 0;
        break;
    }
    case 4:
        return (a.cost == b.cost);
        break;
    case 5:
        return (a.status == b.status);
        break;
    default:
        return 0;
        break;
    };
}

int other_conditions(ITCOMPANY a, ITCOMPANY b, int *condition, int count_conditions)
{
    int match = 0;
    for (int i = 0; i < count_conditions; i++)
    {
        match += equal_condition(a, b, condition[i]);
    }
    if (match == count_conditions)
        return 1;
    else
        return 0;
}

void sorting_first(ITCOMPANY *exemplar, int count, int condition)
{
    ITCOMPANY temporary;
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (j > 0 && sort_condition(exemplar[j - 1], exemplar[j], condition))
        {
            temporary = exemplar[j - 1];
            exemplar[j - 1] = exemplar[j];
            exemplar[j] = temporary;
            j--;
        }
    }
}

void sorting_second(ITCOMPANY *exemplar, int count, int *condition, int count_conditions)
{
    sorting_first(exemplar, count, condition[0]);
    ITCOMPANY temporary;
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (other_conditions(exemplar[j - 1], exemplar[j], condition, i))
        {
            sorting_first(exemplar, count, condition[i]);
            j++;
        }
    }
}
