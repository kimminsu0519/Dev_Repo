#include <iostream>

int main()
{
    int a = 20;
    int b = 10;
    int result = 0;

    result = (a > b) ? a++ : b--;


    std::cout << result;
    
    return 0;
}