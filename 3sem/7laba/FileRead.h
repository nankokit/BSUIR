#include <iostream>
#include <fstream>
#include <string>

enum class typeFile
{
    textFile,
    binFile
};

class FileReader
{
private:
    std::string m_filename;
    typeFile type;

public:
    FileReader(const std::string &filename) : m_filename(filename)
    {
        if (filename.find(".txt") != std::string::npos)
            type = typeFile::textFile;
        else if (filename.find(".bin") != std::string::npos)
            type = typeFile::binFile;
        else
        {
            std::cerr << "Error: incorrect filename " << m_filename << std::endl;
        }
    }

    std::string read()
    {
        std::ifstream file(m_filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Unable to open file " << m_filename << std::endl;
            return "";
        }

        std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();
        return content;
    }
};
