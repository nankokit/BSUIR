#include <iostream>
#include <fstream>
#include <string>

class FileWriter
{
public:
    FileWriter(const std::string &filename) : m_filename(filename) {}

    void write(const std::string &content)
    {
        std::ofstream file(m_filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open file " << m_filename << " for writing" << std::endl;
            return;
        }

        file << content;
        file.close();
    }

private:
    std::string m_filename;
};
