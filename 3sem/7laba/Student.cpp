#include "Student.h"
#include <ostream>
#include <string>
#include <iostream>

int Student::getId() const
{
    return this->id;
}

void Student::setId(int id)
{
    this->id = id;
}

const std::string &Student::getInfo() const
{
    return this->info;
}

void Student::setInfo(const std::string &info)
{
    this->info = info;
}

std::ostream &operator<<(std::ostream &output, const Student &student)
{
    int id = student.getId();
    std::string ids = std::to_string(id);
    std::string info = student.getInfo();
    std::string out = "{id: " + ids + " | info: " + info + "}\n";
    output << out;
    return output;
}

std::istream &operator>>(std::istream &input, Student &student)
{
    char c;
    input >> c;                             // read '{'
    input >> c;                             // read 'id'
    input >> c;                             // read ':'
    input >> student.id;                    // read id
    input >> c;                             // read '|'
    input >> c;                             // read 'info'
    input >> c;                             // read ':'
    input.ignore();                         // skip space after ':'
    std::getline(input, student.info, '}'); // read info untill '}'
    return input;
}