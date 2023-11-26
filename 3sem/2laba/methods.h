#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

class Matrix
{
private:
    int rows;
    int columns;
    int *array = nullptr;

public:
    Matrix() = default;

    Matrix(int, int);

    Matrix(int, int, int *);

    Matrix(const Matrix &);

    Matrix &operator=(const Matrix &other)
    {
        rows = other.rows;
        columns = other.columns;
        array = new int[other.rows * other.columns];
        std::copy(other.array, other.array + other.rows * other.columns, array);
        return *this;
    }

    Matrix operator+(const Matrix &other)
    {
        int *result = new int[rows * columns];
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                result[i * columns + j] = array[i * columns + j] + other.array[i * columns + j];
            }
        }
        Matrix resultMatrix(rows, columns, result);
        return resultMatrix;
    }

    Matrix operator-(const Matrix &other)
    {
        int *result = new int[rows * columns];
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                result[i * columns + j] = array[i * columns + j] - other.array[i * columns + j];
            }
        }
        Matrix resultMatrix(rows, columns, result);
        return resultMatrix;
    }

    Matrix operator*(const Matrix &other)
    {
        int *result = new int[rows * other.columns];
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < other.columns; j++)
            {
                result[i * other.columns + j] = 0;
                for (int k = 0; k < other.rows; k++)
                {
                    result[i * other.columns + j] += array[i * columns + k] * other.array[k * other.columns + j];
                }
            }
        }
        Matrix resultMatrix(rows, other.columns, result);
        return resultMatrix;
    }

    void operator+=(const Matrix &other)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                array[i * columns + j] = array[i * columns + j] + other.array[i * columns + j];
            }
        }
    }

    void operator-=(const Matrix &other)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                array[i * columns + j] = array[i * columns + j] - other.array[i * columns + j];
            }
        }
    }

    void operator*=(const Matrix &other)
    {
        int *result = new int[rows * other.columns];
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < other.columns; j++)
            {
                result[i * other.columns + j] = 0;
                for (int k = 0; k < other.rows; k++)
                {
                    result[i * other.columns + j] += array[i * columns + k] * other.array[k * other.columns + j];
                }
            }
        }
        columns = other.columns;
        array = result;
    }

    bool operator>(const Matrix &other) const
    {
        return (rows + columns > other.rows + other.columns);
    }

    bool operator<(const Matrix &other) const
    {
        return (rows + columns < other.rows + other.columns);
    }

    bool operator==(const Matrix &other) const
    {
        bool equal = true;
        if (rows != other.rows || columns != other.columns)
        {
            return false;
        }
        for (int i = 0; i < rows * columns; i++)
        {
            if (array[i] != other.array[i])
                equal = false;
        }
        return equal;
    }

    void *operator new(size_t size)
    {
        std::cout << "Memory allocation for a Matrix object" << std::endl;
        return ::operator new(size);
    }

    void operator delete(void *ptr)
    {
        std::cout << "Freeing up memory for the Matrix object" << std::endl;
        if (ptr)
            ::operator delete(ptr);
    }

    ~Matrix();

    void fillRows(int);

    void fillColumns(int);

    void fillMatrix(int *);

    void printMatrix();

    int checkIn(int row, int column);

    int getColumns() const;

    int getRows() const;

    friend std::ostream &operator<<(std::ostream &, Matrix);

    friend std::istream &operator>>(std::istream &, Matrix &);
};

void inputDimension(int &, int &);

void inputNumOfMatrix(int &, int);

int *inputArray(int, int);

int isEmpty(int);

bool isEqualDimension(Matrix, Matrix);

bool isGoodMul(Matrix, Matrix);

int inputOperation();

void menuMatrix(std::vector<std::shared_ptr<Matrix>> &);

void menu();
