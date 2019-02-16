//*test with integer type
#include <iostream>
#include <cstdlib>
#include "LinkedList.hpp"

		
using namespace std;

int main(int argc, char* argv[]){
    LinkedList<int>  a = LinkedList<int>();
    a.print();
    for(int i=0;i<100;i++) a.insertNode(a.getHead(),i);
    a.print();
    return 0;
}	
