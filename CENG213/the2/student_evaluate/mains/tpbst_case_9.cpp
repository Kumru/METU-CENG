#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "tpbst.hpp"

/*
 * Case 9 : Empty tree, insert single item, find it, check if structure is broken, print.
 */
int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.insert("ceng213", "sec1", 31);

    int *temp = NULL;
    if ((temp = tpbst.find("ceng213", "sec1")) != NULL)
        std::cout << *temp << std::endl;

    tpbst.print();

    return 0;
}
