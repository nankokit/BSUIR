#include <iostream>
#include <algorithm>
#include <stdexcept>

template <typename T /*y, typename std::enable_if<!std::is_same_v<T, char>>::type * = nullptr*/>

class Array
{
private:
    T *arr;
    size_t arraySize;
    size_t capacity;


public:
    Array() : arr(NULL), arraySize(0), capacity(1) {}

    Array(std::initializer_list<T> L)
    {
        arr = nullptr;
        arraySize = 0;
        capacity = 1;
        for (auto it : L)
        {
            push(it);
        }
    }

    size_t size() const
    {
        return arraySize;
    }

    void push(const T &value)
    {
        if (arraySize >= capacity - 1)
        {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T *newArr = new T[newCapacity];
            std::copy(arr, arr + arraySize, newArr);
            delete[] arr;
            arr = newArr;
            capacity = newCapacity;
        }
        arr[arraySize] = value;
        arraySize++;
    }

    T pop()
    {
        if (arraySize <= 0)
        {
            throw std::out_of_range("Cannot pop from an empty array");
        }
        arraySize--;
        return arr[arraySize];
    }

    T &operator[](const size_t index) const
    {
        if (index >= arraySize)
        {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    T &front() const
    {
        if (arraySize == 0)
        {
            throw std::out_of_range("Empty array");
        }
        return arr[0];
    }

    T &back() const
    {
        if (arraySize == 0)
        {
            throw std::out_of_range("Empty array");
        }
        return arr[arraySize - 1];
    }

    bool isEmpty() const
    {
        return (arraySize == 0);
    }

    ~Array()
    {
        delete[] arr;
    }

    friend std::ostream &operator<<(std::ostream &output, const Array &array)
    {
        for (size_t i = 0; i < array.size(); ++i)
        {
            output << array[i] << " ";
        }
        return output;
    }
};

// int main()
// {
//     Array<int> arr{2, 5, 8, 10};

//     for (int i = 0; i < 10; ++i)
//     {
//         arr.push(i * 10);
//     }
//     arr.pop();
//     arr.pop();
//     arr.pop();

//     std::cout << arr;
//     std::cout << std::endl;
//     std::cout << arr.front() << std::endl;
//     std::cout << arr.back() << std::endl;
//     return 0;
// }
