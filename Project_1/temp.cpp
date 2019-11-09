
#include <iostream>
#include "list.hpp"

int main(int argc , char* argv[])
{
    // Bucket bck;
    int i = 2;

    int& j = i;

    std::cout << &i << std::endl;
    std::cout << &j << std::endl;
    std::cout << i << std::endl;
    std::cout << j << std::endl;

    // std::cout << bck.GetBucketSize() << '\n';
}
