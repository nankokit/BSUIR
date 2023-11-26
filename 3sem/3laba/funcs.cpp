#include "funcs.h"
#include <string.h>

void inputTime(int &hours, int &minutes)
{
    std::cout << "Entering time\n";
    std::cout << "Choose the input method:\n1 - random\n2 - keyboard input\n";
    int method;
    do
    {
        std::cin >> method;
    } while (method != 1 && method != 2);

    switch (method)
    {
    case 1:
    {
        hours = rand() % 24;
        minutes = rand() % 60;

        break;
    }
    case 2:
    {

        do
        {
            std::cout << "Enter hours: ";
            std::cin >> hours;
        } while (hours > 24 && hours < 0);
        do
        {
            std::cout << "Enter minutes: ";
            std::cin >> minutes;
        } while (minutes > 59 && minutes < 0);
        break;
    }
    default:
        break;
    }
    std::cout << "Result: " << hours << ":" << minutes << "\n";
}

void inputDate(int &day, int &month, int &year)
{
    std::cout << "Entering date\n";
    std::cout << "Choose the input method:\n1 - random\n2 - keyboard input\n";
    int method;
    do
    {
        std::cin >> method;
    } while (method != 1 && method != 2);

    switch (method)
    {
    case 1:
    {
        day = rand() % 31 + 1;
        month = rand() % 12 + 1;
        year = rand() % 48 + 1990;
        break;
    }
    case 2:
    {
        do
        {
            std::cout << "Enter day: ";
            std::cin >> day;
        } while (day > 31 && day < 1);
        do
        {
            std::cout << "Enter month: ";
            std::cin >> month;
        } while (month > 12 && month < 1);
        do
        {
            std::cout << "Enter year: ";
            std::cin >> year;
        } while (year > 2038 && year < 1970);
    }
    default:
        break;
    }
    std::cout << "Result: " << day << "." << month << "." << year << "\n";
}

void inputPriority(int &priority)
{
    std::cout << "Entering priority\n";
    std::cout << "Choose the input method:\n1 - random\n2 - keyboard input\n";
    int method;
    do
    {
        std::cin >> method;
    } while (method != 1 && method != 2);

    switch (method)
    {
    case 1:
    {
        priority = rand() % 5 + 1;
        break;
    }
    case 2:
    {
        do
        {
            std::cout << "Enter priority (1 - 5) ";
            std::cin >> priority;
        } while (priority > 5 && priority < 1);
    }
    default:
        break;
    }
}

bool isAttended()
{
    int answer;
    do
    {
        std::cout << "Did you attend the meeting? (1 - yes/2 - no)\n";
        std::cin >> answer;
    } while (answer != 1 && answer != 2);
    return (answer == 1);
}

bool isCompleted()
{
    int answer;
    do
    {
        std::cout << "Is the task completed? (1 - yes/2 - no)\n";
        std::cin >> answer;
    } while (answer != 1 && answer != 2);
    return (answer == 1);
}

int dateRatio(int hours, int minutes, int days, int months, int years)
{
    Time time(hours, minutes);
    Date date(days, months, years);
    Time nowTime;
    nowTime.setNowTime();
    Date nowDate;
    nowDate.setNowData();

    if (date < nowDate || date == nowDate && time < nowTime)
    {
        return -1;
    }
    else if (date == nowDate && time == nowTime)
    {
        return 0;
    }
    else
        return 1;
}

// void addDate()
// {
//     int hours, minutes, days, months, years, priority;
//     inputTime(hours, minutes);
//     inputDate(days, months, years);
//     inputPriority(priority);
//     std::string status;
//     if (dateRatio(hours, minutes, days, months, years) == 1)
//     {
//         status = "Planned";
//     }
//     else
//     {
//         if (isAttended())
//             status = "Attended";
//         else
//             status = "Not attended";
//     }
//     Meeting meet(hours, minutes, days, months, years, status);
//     std::cout << "result:\n";
//     meet.time->printTime();
//     meet.date->printDate();
// }

std::shared_ptr<Meeting> createMeeting()
{
    int hours, minutes, days, months, years;
    inputTime(hours, minutes);
    inputDate(days, months, years);
    std::string status;
    if (dateRatio(hours, minutes, days, months, years) == 1)
    {
        status = "Planned";
    }
    else
    {
        if (isAttended())
            status = "Attended";
        else
            status = "Not attended";
    }
    std::shared_ptr<Meeting> meeting;
    meeting = std::make_shared<Meeting>(hours, minutes, days, months, years, status);
    return meeting;
}

std::shared_ptr<Task> createTask()
{
    int hours, minutes, days, months, years;
    inputTime(hours, minutes);
    inputDate(days, months, years);
    std::string status;
    if (dateRatio(hours, minutes, days, months, years) == 1)
    {
        status = "Planned";
    }
    else
    {
        if (isCompleted())
            status = "Completed";
        else
            status = "Not completed";
    }
    std::shared_ptr<Task> task = std::make_shared<Task>(hours, minutes, days, months, years, status);
    return task;
}

std::shared_ptr<Reminder> createReminder()
{
    int hours, minutes, days, months, years;
    inputTime(hours, minutes);
    inputDate(days, months, years);
    std::string status;
    if (dateRatio(hours, minutes, days, months, years) <= 0)
    {
        std::cout << "You can not create a reminder for a time that has already passed\n";
    }
    else
    {
        std::string message;
        std::cout << "Enter a reminder message\n";
        std::cin >> message;
        std::shared_ptr<Reminder> reminder = std::make_shared<Reminder>(hours, minutes, days, months, years, message);
        return reminder;
    }
    return nullptr;
}

void menu()
{
    std::vector<std::shared_ptr<Event>> eventArray;
    unsigned int action = 0;
    do
    {
        std::cout << "Select an action...\n1 - add new meeting\n";
        std::cout << "2 - add new task\n3 - add new reminder\n";
        std::cout << "4 - print all events\n5 - exit\n";
        std::cin >> action;
        switch (action)
        {
        case 1:
        {
            auto newEvent = std::make_shared<Event>(createMeeting());
            eventArray.push_back(newEvent);
            break;
        }
        case 2:
        {
            auto newEvent = std::make_shared<Event>(createTask());
            eventArray.push_back(newEvent);
            break;
        }
        case 3:
        {
            std::shared_ptr<Reminder> reminder;
            reminder = createReminder();
            if (reminder != nullptr)
            {
                auto newEvent = std::make_shared<Event>(reminder);
                eventArray.push_back(newEvent);
            }
            break;
        }
        case 4:
        {
            for (int i = 0; i < eventArray.size(); i++)
            {
                eventArray[i]->print();
                std::cout << std::endl;
            }

            break;
        }
        case 5:
        {
            std::cout << "\nExit...\n";
            break;
        }
        default:
            std::cout << "Error \n";
            break;
        }
    } while (action != 5);
}
