#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "tpbst.hpp"

/*
 * Case 2 : Empty tree, print by primary key.
 */
int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.print("ceng213");

    return 0;
}
