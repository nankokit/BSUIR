#pragma once
#include <iostream>

class Matrix
{
private:
    int rows;
    int columns;
    int *array;

public:
    // Конструкторы
    Matrix() = default;

    Matrix(int, int);

    Matrix &operator=(const Matrix &other)
    {
        rows = other.rows;
        columns = other.columns;
        std::cout << "begin1";
        array = new int[other.rows * other.columns];
        std::cout << "begin2";
        std::copy(other.array, other.array + other.rows * other.columns, array);
        std::cout << "begin3";
        std::cout << rows << " " << columns;
        return *this;
    }

    Matrix(const Matrix &);

    // Деструктор
    ~Matrix()
    {
        // std::cout << "deleting";
        if (rows * columns > 0)
            delete[] array;
    }

    void fillCell(int, int);

    void fillMatrix();

    void addRow();

    void addColumn();

    void deleteRow();

    void deleteColumn();

    void printMatrix();

    void transpose();

    int checkIn(int row, int column);

    int isEmpty();

    int getColumns() const;

    int getRows() const;
};

Matrix *appendArray(Matrix *, int &, int, int);

Matrix *reduceArray(Matrix *, int &, int);

void menuMatrix(Matrix &);

void menu();