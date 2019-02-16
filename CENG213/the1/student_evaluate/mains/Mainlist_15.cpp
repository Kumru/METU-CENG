//swap V2
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "LinkedList.hpp"

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class tmpClass {
public:

    tmpClass(string val1 = "", int val2 = 0) : s_val(val1), i_val(val2) {
    };

    friend ostream &operator<<(ostream &out, const tmpClass& r) {
        out << "[" << r.s_val << ":" << r.i_val << "]";
        return out;
    };

    bool operator==(const tmpClass & rhs) const {
        return (this->s_val == rhs.s_val);
    };

private:
    string s_val;
    int i_val;
};


using namespace std;

int main(int argc, char* argv[]) {
    LinkedList<tmpClass> list = LinkedList<tmpClass>();
    list.insertNode(list.getHead(), tmpClass("Node4", 4));
    list.insertNode(list.getHead(), tmpClass("Node3", 3));
    list.insertNode(list.getHead(), tmpClass("Node2", 2));
    list.insertNode(list.getHead(), tmpClass("Node1", 1));
    list.print();

    list.swap(1,7); //out bound
    
    list.print();
    return 0;
}
