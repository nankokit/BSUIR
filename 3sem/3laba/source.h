#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

class Time
{
public:
    int hours, minutes;

    Time(){};
    Time(const int hours, const int minutes);
    // ~Time();

    void setTime(int hours, int minutes);
    void setNowTime();
    void printTime() const;

    bool operator>(Time &other);
    bool operator<(Time &other);
    bool operator>=(Time &other);
    bool operator<=(Time &other);
    bool operator==(Time &other);
};

class Date
{
public:
    int days, months, years, weekDay;

    Date(){};
    Date(int days, int months, int year);
    // ~Date();

    void setData(int days, int months, int years);
    void setNowData();
    void setWeekDay();
    void printDate() const;

    bool operator>(Date &other);
    bool operator<(Date &other);
    bool operator>=(Date &other);
    bool operator<=(Date &other);
    bool operator==(Date &other);
};

class Day
{
public:
    std::shared_ptr<Time> time;
    std::shared_ptr<Date> date;
    static inline unsigned long long count{};

    Day(int hours, int minutes, int days, int months, int years);
    void print() const;
};

class Meeting : public Day
{
private:
    int uid;
    std::string status;

public:
    Meeting(int hours, int minutes, int days, int months, int years, std::string status);
    void print() const;
};

class Task : public Day
{
private:
    int uid;
    std::string status;

public:
    Task(int hours, int minutes, int days, int months, int years, std::string status);
    void print() const;
};

class Reminder : public Day
{
private:
    int uid;
    std::string message;

public:
    Reminder(int hours, int minutes, int days, int months, int years, std::string message);
    void print() const;
};

enum class Type
{
    meeting = 1,
    task,
    reminder
};

class Event
{
public:
    int type;
    std::shared_ptr<Meeting> meeting;
    std::shared_ptr<Task> task;
    std::shared_ptr<Reminder> reminder;

    Event(std::shared_ptr<Meeting>);
    Event(std::shared_ptr<Task>);
    Event(std::shared_ptr<Reminder>);
    void print() const;
};