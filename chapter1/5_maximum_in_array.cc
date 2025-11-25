#include <iostream>
#include <vector>

int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<int> arr(n);

    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    int index = 0;
    int max = arr[0];

    for (int i = 1; i < n; i++)
    {
        if(arr[i]>max){
            index = i;
            max = arr[i];
        }
    }

    std::cout << max << " " << index ;

    return 0;
}