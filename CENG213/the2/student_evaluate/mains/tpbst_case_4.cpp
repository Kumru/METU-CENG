#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "tpbst.hpp"

/*
 * Case 4 : Empty tree, remove, check if structure is broken, print.
 */
int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.remove("ceng213", "sec1")
            .print();

    return 0;
}
