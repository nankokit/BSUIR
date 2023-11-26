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

    void setTime(const int hours, const int minutes);
    void setNowTime();
    void printTime() const;

    bool operator>(Time &other) const;
    bool operator<(Time &other) const;
    bool operator>=(Time &other) const;
    bool operator<=(Time &other) const;
    bool operator==(Time &other) const;
};

class Date
{
public:
    int days, months, years, weekDay;

    Date(){};
    Date(const int days, const int months, const int year);
    // ~Date();

    void setData(const int days, const int months, const int years);
    void setNowData();
    void setWeekDay();
    void printDate() const;

    bool operator>(Date &other) const;
    bool operator<(Date &other) const;
    bool operator>=(Date &other) const;
    bool operator<=(Date &other) const;
    bool operator==(Date &other) const;
};

class Day
{
public:
    std::shared_ptr<Time> time;
    std::shared_ptr<Date> date;
    static inline unsigned long long count{};

    Day(int hours, int minutes, int days, int months, int years);
    // void printDay() const;
    virtual void print() const = 0;
};

class Meeting : public Day
{
private:
    int uid;
    std::string status;

public:
    Meeting(const int hours, const int minutes, const int days, const int months, const int years, const std::string status);
    void print() const override;
};

class Task : public Day
{
private:
    int uid;
    std::string status;

public:
    Task(const int hours, const int minutes, const int days, const int months, const int years, const std::string status);
    void print() const override;
    void congratulations() const;
};

class Reminder : public Day
{
private:
    int uid;
    std::string message;

public:
    Reminder(const int hours, const int minutes, const int days, const int months, const int years, const std::string message);
    void print() const override;
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