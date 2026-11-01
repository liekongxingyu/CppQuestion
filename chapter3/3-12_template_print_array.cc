#include <iostream>

template <typename T>
void printArray(T *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << std::endl;
    }
}

int main()
{
    int n;
    std::cin >> n;
    double *arr = new double[n];
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    printArray(arr, n);

    delete[] arr;

    return 0;
}
