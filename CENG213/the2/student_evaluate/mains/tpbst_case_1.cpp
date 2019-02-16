#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "tpbst.hpp"

/*
 * Case 1 : Empty tree, print.
 */
int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.print();

    return 0;
}
