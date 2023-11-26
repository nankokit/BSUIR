#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include "source.h"

void inputTime(int &hours, int &minutes);
void inputDate(int &day, int &month, int &year, int &weekDay);
void inputPriority(int &priority);
bool isAttended();
int dateRatio(int hours, int minutes, int days, int months, int years);
// void addDate();

std::shared_ptr<Meeting> createMeeting();
std::shared_ptr<Task> createTask();
std::shared_ptr<Reminder> createReminder();

void menu();