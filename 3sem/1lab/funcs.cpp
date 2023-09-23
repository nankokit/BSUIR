#include <iostream>
#include <iomanip>
#include "funcs.h"

Matrix::Matrix(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
    this->array = new int[rows * columns];
    fillMatrix();
    std::cout << "Succesfull\n";
    printMatrix();
}

Matrix::Matrix(const Matrix &other)
{
    rows = other.rows;
    columns = other.columns;
    // std::cout << "begin1";
    array = new int[other.rows * other.columns];
    // std::cout << "begin2";
    std::copy(other.array, other.array + other.rows * other.columns, array);
    // std::cout << "begin3";
    std::cout << rows << " " << columns;
}

void Matrix::fillCell(int row, int column)
{
    std::cout << "Enter the cell value... \n";
    std::cin >> array[(row - 1) * (columns) + (column - 1)];
}

void Matrix::fillMatrix()
{
    std::cout << "Choose the filling method:\n1 - random\n2 - keyboard input\n";
    int method;
    std ::cin >> method;
    switch (method)
    {
    case 1:
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                array[i * columns + j] = rand() % 90 + 10;
            }
        }
        break;
    case 2:
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                std::cin >> array[i * columns + j];
            }
        }
        break;

    default:
        break;
    }
}

void Matrix::addRow()
{
    int *newArray = new int[(rows + 1) * columns];
    std::copy(array, array + rows * columns, newArray);
    delete[] array;
    array = newArray;
    rows++;
    std::cout << "Enter new row... \n";
    for (size_t i = (columns) * (rows - 1); i < (columns) * (rows - 1) + columns; i++)
    {
        std::cin >> array[i];
    }
}

void Matrix::addColumn()
{
    int *newArray = new int[rows * (columns + 1)];
    for (int i = 0; i < rows; i++)
    {
        std::copy(array + i * columns, array + i * columns + columns, newArray + i * (columns + 1));
    }
    delete[] array;
    array = newArray;
    columns++;
    std::cout << "Enter new column... \n";
    for (size_t i = 0; i < rows; i++)
    {
        std::cin >> array[(i + 1) * (columns - 1) + i];
    }
}

void Matrix::deleteRow()
{
    if (rows > 1)
    {
        int *newArray = new int[(rows - 1) * columns];
        std::copy(array, array + (rows - 1) * columns, newArray);
        delete[] array;
        array = newArray;
        rows--;
    }
    else
        std::cout << "Cannot be deleted\n";
}

void Matrix::deleteColumn()
{
    if (columns > 1)
    {
        int *newArray = new int[rows * (columns - 1)];
        for (int i = 0; i < rows; i++)
        {
            std::copy(array + i * columns, array + i * columns + columns - 1, newArray + i * (columns - 1));
        }
        delete[] array;
        array = newArray;
        columns--;
    }
    else
        std::cout << "Cannot be deleted\n";
}

void Matrix::printMatrix()
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            std::cout << std::setw(4);
            std::cout << array[i * columns + j] << " ";
        }
        std::cout << "\n";
    }
}

void Matrix::transpose()
{
    int *newArray = new int[columns * rows];
    for (int i = 0; i < rows * columns; i++)
        newArray[i] = 0;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            newArray[j * rows + i] = array[i * columns + j];
        }
    }
    for (int i = 0; i < rows * columns; i++)
        array[i] = newArray[i];
    // array = newArray;
    delete[] newArray;
    int temp = columns;
    this->columns = rows;
    this->rows = temp;
}

int Matrix::checkIn(int row, int column)
{
    return (row <= rows && column <= columns);
}

int Matrix::isEmpty()
{
    return (columns == 0 || rows == 0);
}

int Matrix::getColumns() const
{
    return columns;
}

int Matrix::getRows() const
{
    return rows;
}

Matrix *appendArray(Matrix *array, int &arraySize, int row, int column)
{
    Matrix *newArray = new Matrix[arraySize + 1];
    for (size_t i = 0; i < arraySize; i++)
    {
        newArray[i] = array[i];
    }
    newArray[arraySize] = Matrix(row, column);
    if (arraySize > 0)
        delete[] array;
    arraySize++;
    return newArray;
}

Matrix *reduceArray(Matrix *array, int &arraySize, int numDel)
{
    Matrix *newArray = new Matrix[arraySize - 1];
    for (size_t i = 0; i < arraySize; i++)
    {
        if (numDel != arraySize)
            newArray[i] = array[i];
    }
    if (arraySize > 0)
        delete[] array;
    arraySize--;
    return newArray;
}

void menuMatrix(Matrix &matrix)
{
    int action;
    std::cout << "Select an action...\n1 - overwrite a matrix cell\n";
    std::cout << "2 - add column\n3 - delete column\n4 - add row\n";
    std::cout << "5 - delete row\n6 - transpose the matrix\n7 - exit\n";
    std ::cin >> action;
    switch (action)
    {
    case 1:
    {
        int row, column;
        std::cout << "Enter the cell row and column (row < " << matrix.getRows();
        std::cout << " and column < " << matrix.getColumns() << ")...\n";
        std::cin >> row >> column;
        while (!matrix.checkIn(row, column))
        {
            std::cout << "Incorrect. Enter the cell row and column... \n";
            std::cin >> row >> column;
        }
        matrix.fillCell(row, column);
        break;
    }
    case 2:
    {
        matrix.addColumn();
        break;
    }
    case 3:
    {
        matrix.deleteColumn();
        break;
    }
    case 4:
    {
        matrix.addRow();
        break;
    }
    case 5:
    {
        matrix.deleteRow();
        break;
    }
    case 6:
    {
        matrix.transpose();
        break;
    }
    default:
        break;
    }
}

void menu()
{
    Matrix *array = new Matrix;
    int arraySize = 0;
    unsigned int action = 0;
    do
    {
        std::cout << "Select an action...\n1 - add new matrix\n";
        std::cout << "2 - go to an existing matrix\n3 - delete an existing matrix\n4 - exit\n";
        std::cin >> action;
        switch (action)
        {
        case 1:
        {
            int row, column;
            do
            {
                std::cout << "Enter the number of rows and columns... \n";
                std::cin >> row >> column;
            } while (row <= 0 || column <= 0);
            array = appendArray(array, arraySize, row, column);
            // Matrix newMatrix(row, column);
            break;
        }
        case 2:
        {
            if (arraySize == 0)
                std::cout << "There are no matrixs :(\n";
            else
            {
                for (size_t i = 0; i < arraySize; i++)
                {
                    std::cout << "\nMatrix " << i + 1 << ":\n";
                    array[i].printMatrix();
                }
                int activeMatrix;
                do
                {
                    std::cout << "\nEnter the number of the matrix you want to work with...\n";
                    std::cin >> activeMatrix;
                } while (activeMatrix > arraySize || activeMatrix < 1);
                menuMatrix(array[activeMatrix - 1]);
            }
            break;
        }
        case 3:
        {
            if (arraySize == 0)
                std::cout << "There are no matrixs :(\n";
            else
            {
                for (size_t i = 0; i < arraySize; i++)
                {
                    std::cout << "\nMatrix " << i + 1 << ":\n";
                    array[i].printMatrix();
                }
                int activeMatrix;
                do
                {
                    std::cout << "\nEnter the number of the matrix you want to delete...\n";
                    std::cin >> activeMatrix;
                } while (activeMatrix > arraySize || activeMatrix < 0);
                array = reduceArray(array, arraySize, activeMatrix);
            }
            break;
        }
        case 4:
            std::cout << "Exit... \n";
            break;
        default:
            std::cout << "Error \n";
            break;
        }
    } while (action != 4);
    delete[] array;
}
