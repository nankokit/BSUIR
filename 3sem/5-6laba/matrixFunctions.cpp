#include "matrixFunctions.h"

void inputDimension(int &row, int &column)
{
    do
    {
        std::cin >> row >> column;
    } while (row <= 0 || column <= 0);
}

void inputNumOfMatrix(int &activeMatrix, int arraySize)
{
    do
    {
        std::cin >> activeMatrix;
    } while (activeMatrix > arraySize || activeMatrix < 1);
}

int *inputArray(int rows, int columns)
{
    std::cout << "Choose the filling method:\n1 - random\n2 - keyboard input\n";
    int method;
    do
    {
        std::cin >> method;
    } while (method != 1 && method != 2);

    int *array{new int[rows * columns]};
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
    return array;
}

int isEmpty(int arraySize)
{
    if (arraySize == 0)
    {
        std::cout << "There are no matrixs :(\n";
        return 1;
    }
    else
        return 0;
}

bool isEqualDimension(Matrix matrix1, Matrix matrix2)
{
    return (matrix1.getRows() == matrix2.getRows() && matrix1.getColumns() == matrix2.getColumns());
}

bool isGoodMul(Matrix matrix1, Matrix matrix2)
{
    return (matrix1.getColumns() == matrix2.getRows());
}

int inputOperation()
{
    std::cout << "\nEnter matrix operation: +, -, *, +=, -=, *=, <, >, ==\n";
    std::string operation;
    std::cin >> operation;
    std::string operations[9] = {"+", "-", "*", "+=", "-=", "*=", "<", ">", "=="};
    for (int i = 0; i < 9; i++)
    {
        if (operation == operations[i])
            return i + 1;
    }
    return -1;
}

void printAllMatrix(Array<std::shared_ptr<Matrix>> &matrixArray)
{
    int i = 1;
    std::shared_ptr<Matrix> temp;
    for (int i = 0; i < matrixArray.size(); i++)
    {
        temp = matrixArray[i];
        std::cout << "Matrix " << i++ << std::endl;
        temp->printMatrix();
        std::cout << std::endl;
    }
}

void menuMatrux(Array<std::shared_ptr<Matrix>> &matrixArray)
{
    printAllMatrix(matrixArray);
    int operation = inputOperation();
    switch (operation)
    {
    case 1: //+
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());
        auto result = std::make_shared<Matrix>();

        if (isEqualDimension(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *result = *(matrixArray[matrix1 - 1]) + *(matrixArray[matrix2 - 1]);
            matrixArray.push(std::shared_ptr<Matrix>(result));
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 2: //-
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());
        auto result = std::make_shared<Matrix>();

        if (isEqualDimension(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *result = *(matrixArray[matrix1 - 1]) - *(matrixArray[matrix2 - 1]);
            matrixArray.push(std::shared_ptr<Matrix>(result));
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 3: //*
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());
        auto result = std::make_shared<Matrix>();

        if (isGoodMul(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *result = *(matrixArray[matrix1 - 1]) * *(matrixArray[matrix2 - 1]);
            matrixArray.push(std::shared_ptr<Matrix>(result));
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 4: //+=
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (isEqualDimension(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *(matrixArray[matrix1 - 1]) += *(matrixArray[matrix2 - 1]);
            std::cout << "Succesfull\n";
            matrixArray[matrix1 - 1]->printMatrix();
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 5: //-=
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (isEqualDimension(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *(matrixArray[matrix1 - 1]) -= *(matrixArray[matrix2 - 1]);
            std::cout << "Succesfull\n";
            matrixArray[matrix1 - 1]->printMatrix();
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 6: //*=
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (isGoodMul(*(matrixArray[matrix1 - 1]), *(matrixArray[matrix2 - 1])))
        {
            *(matrixArray[matrix1 - 1]) *= *(matrixArray[matrix2 - 1]);
            std::cout << "Succesfull\n";
            matrixArray[matrix1 - 1]->printMatrix();
        }
        else
            std::cout << "Incorrect dimension" << std::endl;
        break;
    }
    case 7: //<
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (*(matrixArray[matrix1 - 1]) < *(matrixArray[matrix2 - 1]))
            std::cout << "Matrix " << matrix1 << " less than matrix " << matrix2 << std::endl;
        else
            std::cout << "Matrix " << matrix1 << " not less than matrix " << matrix2 << std::endl;
        break;
    }
    case 8: //>
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (*(matrixArray[matrix1 - 1]) > *(matrixArray[matrix2 - 1]))
            std::cout << "Matrix " << matrix1 << " more than matrix " << matrix2 << std::endl;
        else
            std::cout << "Matrix " << matrix1 << " not more than matrix " << matrix2 << std::endl;
        break;
    }
    case 9: //==
    {
        int matrix1, matrix2;
        std::cout << "\nEnter number of first matrix: ";
        inputNumOfMatrix(matrix1, matrixArray.size());
        std::cout << "\nEnter number of second matrix: ";
        inputNumOfMatrix(matrix2, matrixArray.size());

        if (*(matrixArray[matrix1 - 1]) == *(matrixArray[matrix2 - 1]))
            std::cout << "Equal\n";
        else
            std::cout << "Not equal\n";
        break;
    }
    default:
        break;
    }
}

void menu()
{
    Array<std::shared_ptr<Matrix>> matrixArray;
    unsigned int action = 0;
    do
    {
        std::cout << "Select an action...\n1 - add new matrix\n";
        std::cout << "2 - go to operations\n3 - delete matrix\n";
        std::cout << "4 - print all matrix\n5 - exit\n";
        std::cin >> action;
        switch (action)
        {
        case 1:
        {
            // int row, column;
            // std::cout << "Enter the number of rows and columns... \n";
            // inputDimension(row, column);
            // matrixArray.push_back(std::make_shared<Matrix>(row, column));
            auto newObject = std::make_shared<Matrix>();
            std::cin >> (*newObject);
            matrixArray.push(newObject);
            break;
        }
        case 2:
        {
            if (isEmpty(matrixArray.size()))
                ;
            else
            {
                menuMatrux(matrixArray);
            }
            break;
        }
        case 3:
        {

            std::cout << "\nSuccessful deletion of this matrix: ";
            std::cout << matrixArray.pop();
            break;
        }
        case 4:
        {
            for (int i = 0; i < matrixArray.size(); i++)
            {
                std::cout << "\nMatrix " << i + 1 << std::endl;
                std::cout << *(matrixArray[i]);
            }
            break;
        }
        case 5:
        {
            std::cout << "\nExit...\n";
            break;
        }
        default:
            std::cout << "Error \n";
            break;
        }
    } while (action != 5);
}
