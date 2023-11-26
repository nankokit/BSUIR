#include <iostream>
#include <iomanip>
#include "source.h"

Time::Time(const int hours, const int minutes)
{
    this->hours = hours;
    this->minutes = minutes;
}

void Time::setTime(int hours, int minutes)
{
    this->hours = hours;
    this->minutes = minutes;
}

void Time::setNowTime()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    this->hours = now->tm_hour;
    this->minutes = now->tm_min;
}

void Time::printTime() const
{
    std::cout << std::setfill('0');
    std::cout << "Time: " << std::setw(2) << hours << ":";
    std::cout << std::setw(2) << minutes << std::endl;
    std::cout << std::setfill(' ');
}

bool Time::operator>(Time &other)
{
    if (hours > other.hours)
        return true;
    else if (hours == other.hours && minutes > other.minutes)
        return true;
    return false;
}

bool Time::operator<(Time &other)
{
    if (hours < other.hours)
        return true;
    else if (hours == other.hours && minutes < other.minutes)
        return true;
    return false;
}

bool Time::operator>=(Time &other)
{
    if (hours >= other.hours)
        return true;
    else if (hours == other.hours && minutes >= other.minutes)
        return true;
    return false;
}

bool Time::operator<=(Time &other)
{
    if (hours <= other.hours)
        return true;
    else if (hours == other.hours && minutes <= other.minutes)
        return true;
    return false;
}

bool Time::operator==(Time &other)
{
    if (hours == other.hours && minutes == other.minutes)
        return true;
    else
        return false;
}

//////////////

Date::Date(int days, int months, int years)
{
    this->days = days;
    this->months = months;
    this->years = years;
    setWeekDay();
};

void Date::setData(int days, int months, int years)
{
    this->days = days;
    this->months = months;
    this->years = years;
    setWeekDay();
}

void Date::setNowData()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    this->days = now->tm_mday;
    this->months = now->tm_mon + 1;
    this->years = now->tm_year + 1900;
    this->weekDay = now->tm_wday;
}

void Date::setWeekDay()
{
    int a;
    int y;
    int m;
    int R;
    a = (14 - months) / 12;
    y = years - a;
    m = months + 12 * a - 2;
    R = 7000 + (days + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12);
    weekDay = R % 7;
}

void Date::printDate() const
{
    std::cout << std::setfill('0');
    std::cout << "Date: " << std::setw(2) << days << ".";
    std::cout << std::setw(2) << months << "." << std::setw(4) << years << " ";
    std::string week[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    std::cout << week[weekDay] << std::endl;
    std::cout << std::setfill(' ');
}

bool Date::operator>(Date &other)
{
    if (years > other.years)
        return true;
    else if (years == other.years && months > other.months)
        return true;
    else if (years == other.years && months == other.months && days > other.days)
        return true;
    else
        return false;
}

bool Date::operator<(Date &other)
{
    if (years < other.years)
        return true;
    else if (years == other.years && months < other.months)
        return true;
    else if (years == other.years && months == other.months && days < other.days)
        return true;
    else
        return false;
}

bool Date::operator>=(Date &other)
{
    if (years >= other.years)
        return true;
    else if (years == other.years && months >= other.months)
        return true;
    else if (years == other.years && months == other.months && days >= other.days)
        return true;
    else
        return false;
}

bool Date::operator<=(Date &other)
{
    if (years <= other.years)
        return true;
    else if (years == other.years && months <= other.months)
        return true;
    else if (years == other.years && months == other.months && days <= other.days)
        return true;
    else
        return false;
}

bool Date::operator==(Date &other)
{
    if (years == other.years && months == other.months && days == other.days)
        return true;
    else
        return false;
}

///////////

Day::Day(int hours, int minutes, int days, int months, int years)
{
    std::shared_ptr<Time> time{std::make_shared<Time>(hours, minutes)};
    std::shared_ptr<Date> date{std::make_shared<Date>(days, months, years)};
    this->time = time;
    this->date = date;
    count++;
}

void Day::print() const
{
    time->printTime();
    date->printDate();
}

///////////

Meeting::Meeting(int hours, int minutes, int days, int months, int years, std::string status) : Day(hours, minutes, days, months, years)
{
    uid = count;
    this->status = status;
}

void Meeting::print() const
{
    std::cout << "Meeting uid:" << uid << "\n";
    std::cout << "Status: " << status << "\n";
    time->printTime();
    date->printDate();
}

Task::Task(int hours, int minutes, int days, int months, int years, std::string status) : Day(hours, minutes, days, months, years)
{
    uid = count;
    this->status = status;
}

void Task::print() const
{
    std::cout << "Task uid:" << uid << "\n";
    std::cout << "Status: " << status << "\n";
    time->printTime();
    date->printDate();
}

Reminder::Reminder(int hours, int minutes, int days, int months, int years, std::string message) : Day(hours, minutes, days, months, years)
{
    uid = count;
    this->message = message;
}

void Reminder::print() const
{
    std::cout << "Reminder uid:" << uid << "\n";
    std::cout << "Reminder message: \"" << message << "\"\n";
    time->printTime();
    date->printDate();
}

///////////

Event::Event(std::shared_ptr<Meeting> meeting)
{
    type = static_cast<int>(Type::meeting);
    this->meeting = meeting;
}

Event::Event(std::shared_ptr<Task> task)
{
    type = static_cast<int>(Type::task);
    this->task = task;
}

Event::Event(std::shared_ptr<Reminder> reminder)
{
    type = static_cast<int>(Type::reminder);
    this->reminder = reminder;
}

void Event::print() const
{
    switch (type)
    {
    case static_cast<int>(Type::meeting):
    {
        meeting->print();
        break;
    }
    case static_cast<int>(Type::task):
    {
        task->print();
        break;
    }
    case static_cast<int>(Type::reminder):
    {
        reminder->print();
        break;
    }

    default:
        break;
    }
}