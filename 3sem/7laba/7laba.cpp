#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Student.h"
#include "FileRead.h"

bool uniqID(std::vector<Student *> studentVector, int id)
{
    for (const auto &student : studentVector)
    {
        if (student->getId() == id)
            return false;
    }
    return true;
}

int inputID(std::vector<Student *> studentVector)
{
    bool flag = true;
    int id;
    do
    {
        flag = true;
        std::cout << "Enter ID: ";
        std::cin >> id;
        std::cin.ignore();
        if (id != (int)id || id <= 0 || !uniqID(studentVector, id))
        {
            flag = false;
            std::cout << "Incorrect ID" << std::endl;
        }
    } while (!flag);
    return id;
}

std::string inputInfo()
{
    std::string info;
    std::cout << "Enter info: ";
    std::getline(std::cin, info);
    return info;
}

void inputStudent(std::vector<Student *> &studentVector)
{
    Student *student = new Student();
    std::cout << "Entering Student ID and info" << std::endl;
    student->setId(inputID(studentVector));
    student->setInfo(inputInfo());
    studentVector.push_back(student);
}

int inputInt(int min, int max)
{
    int action;
    bool flag = true;
    do
    {
        std::cout << "Your choice: ";
        std::cin >> action;
        std::cin.ignore();
        if (action != (int)action && (action < min || action > max))
        {
            flag = false;
            std::cout << "Incorrect. Try again" << std::endl;
        }
    } while (!flag);
    return action;
}

void fillStudentVector(std::vector<int> &IDs, std::vector<Student *> &studentVector)
{
    std::cout << "Choose action:\n1 - add a student\n2 - fill files\n\n";
    int choice = inputInt(1, 2);
    while (choice == 1)
    {
        inputStudent(studentVector);
        std::cout << "Choose action:\n1 - add a student\n2 - upload a studentVector to a binary file\n\n";
        choice = inputInt(1, 2);
    }

    for (const auto &student : studentVector)
        IDs.push_back(student->getId());
}

void mixIDs(std::vector<int> &IDs)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(IDs.begin(), IDs.end(), g);
}

void fillTxtFile(std::vector<int> &IDs)
{
    mixIDs(IDs);
    std::ofstream txtFile("txtFile.txt");
    if (txtFile.is_open())
    {
        for (const auto &id : IDs)
        {
            txtFile << id << "\n";
        }
        txtFile.close();
        std::cout << "Txt file succesfull created" << std::endl;
    }
    else
    {
        std::cerr << "Unable to open txt file" << std::endl;
    }
}

void fillBinFile(std::vector<Student *> &studentVector)
{
    std::ofstream outputFile("binFile.bin", std::ios::binary);
    if (outputFile.is_open())
    {
        for (const auto &student : studentVector)
        {
            // outputFile.write(reinterpret_cast<const char *>(&student), sizeof(Student));
            int id = student->getId();
            outputFile.write(reinterpret_cast<const char *>(&id), sizeof(int));

            std::string info = student->getInfo();
            int infoLength = info.size();
            outputFile.write(reinterpret_cast<const char *>(&infoLength), sizeof(int));
            outputFile.write(info.c_str(), infoLength);
        }

        outputFile.close();
        std::cout << "Bin file succesfull created" << std::endl;
    }
    else
    {
        std::cerr << "Unable to open bin file" << std::endl;
    }
}

void changeFile(int changeID)
{
    std::ifstream inFile("resultFile.txt");

    if (!inFile.is_open())
    {
        std::cerr << "Unable to open result file" << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line))
    {
        lines.push_back(line);
        // std::cout << line << std::endl;
    }
    inFile.close();

    std::ofstream outFile("resultFile.txt");

    if (!outFile.is_open())
    {
        std::cerr << "Unable to open result file" << std::endl;
        return;
    }

    for (std::string line : lines)
    {
        std::string num;
        num = line[5];
        int i = 6;
        while (line[i] != ' ')
            num += line[i++];
        if (changeID == std::atoi(num.c_str()))
        {
            num = "";
            std::cout << "Enter new data: " << std::endl;
            std::getline(std::cin, num, '\n');
            line = "{id: " + std::to_string(changeID) + " | info: " + num + "}";
        }
        outFile << line << std::endl;
    }
    outFile.close();
}

void changeMenu(std::vector<int> IDs)
{
    int changeID;
    bool flag = true;
    int choice = 0;
    do
    {
        std::cout << "Do you wanna change something?\n1-yes\n2-exit\n";
        choice = inputInt(1, 2);
        if (choice == 1)
        {
            do
            {
                std::cout << "Enter ID: ";
                std::cin >> changeID;
                std::cin.ignore();
                auto it = std::find(IDs.begin(), IDs.end(), changeID);

                if (it == IDs.end() && changeID != IDs.at(IDs.size() - 1))
                {
                    std::cout << "Incorrect ID. Try again." << std::endl;
                    flag = false;
                }
            } while (!flag);
            changeFile(changeID);
        }
    } while (choice != 2);
}

void fileParsing()
{
    std::ifstream txtFile("txtFile.txt");
    std::ifstream binFile("binFile.bin");
    std::ofstream resultFile("resultFile.txt");

    int id1, id2 = -1;
    Student *student = new Student();

    while (txtFile >> id1)
    {
        binFile.seekg(0, std::ios::beg);
        while (id1 != id2)
        {

            binFile.read(reinterpret_cast<char *>(&id2), sizeof(int));
            int infoLength;
            binFile.read(reinterpret_cast<char *>(&infoLength), sizeof(int));
            char *buffer = new char[infoLength + 1];
            binFile.read(buffer, infoLength);
            buffer[infoLength] = '\0';
            student->setId(id2);
            student->setInfo(buffer);
        }
        resultFile << *student;
    }
    delete student;

    txtFile.close();
    binFile.close();
    resultFile.close();
}

void fillFiles(std::vector<int> &IDs, std::vector<Student *> &studentVector)
{
    fillTxtFile(IDs);
    fillBinFile(studentVector);
    fileParsing();
}

int main()
{
    std::vector<Student *> studentVector;
    std::vector<int> IDs;
    fillStudentVector(IDs, studentVector);
    fillFiles(IDs, studentVector);

    FileReader reader2("resultFile.txt");
    std::string content2 = reader2.read();
    std::cout << "Result file content: \n"
              << content2 << std::endl;

    changeMenu(IDs);
    std::cout << "\n";

    return 0;
}