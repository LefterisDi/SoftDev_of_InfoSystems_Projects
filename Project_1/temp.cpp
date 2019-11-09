
#include <iostream>
#include "list.hpp"

using namespace std;

int main(int argc , char* argv[])
{
    List<int> list(24 , sizeof(int));

    list.ListInsert(1);
    list.ListInsert(2);
    list.ListInsert(3);
    list.ListInsert(4);
    list.ListInsert(5);
    list.ListInsert(6);
    list.ListInsert(7);

    list.ListPrint();

    // std::cout << bck.GetBucketSize() << '\n';
}
