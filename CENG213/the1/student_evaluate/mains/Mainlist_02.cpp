//test with a class
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "LinkedList.hpp"

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
		

class tmpClass{
    public:
        tmpClass(string val1 = "", int val2 = 0): s_val(val1), i_val(val2) {};
        friend ostream &operator<<(ostream &out, const tmpClass& r){
            out << "["<<r.s_val << ":" << r.i_val<<"]";
            return out;
        };
        bool operator==(const tmpClass & rhs) const{
	    return (this->s_val==rhs.s_val);
        };
    private:
        string s_val;
        int i_val;
};


using namespace std;

int main(int argc, char* argv[]){

    srand (21);
    LinkedList<tmpClass> list = LinkedList<tmpClass>();
    list.print();
    for (int i = 0; i < 20; ++i){
        list.insertNode(list.getHead(),tmpClass(SSTR(rand()%10), rand()%10));
    }
    list.print();
    return 0;
}
