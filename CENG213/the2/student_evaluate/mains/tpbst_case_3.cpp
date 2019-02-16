#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "tpbst.hpp"

/*
 * Case 3 : Empty tree, print by primary key and secondary key.
 */
int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.print("ceng213", "sec1");

    return 0;
}
