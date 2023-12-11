#include <cstddef>
#include <string>
#include <memory>

class Student
{
private:
    int id;
    std::string info;

public:
    Student() {}
    int getId() const;
    void setId(int id);
    const std::string &getInfo() const;
    void setInfo(const std::string &info);

    friend std::ostream &operator<<(std::ostream &os, const Student &student);
    friend std::istream &operator>>(std::istream &is, Student &student);
};
