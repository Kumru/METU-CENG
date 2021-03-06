#ifndef NODE_HPP
#define	NODE_HPP

#include <iostream>
#include <iomanip>

using namespace std;

template<class T>
class Node;

template<class T>
ostream &operator<<(ostream &out, const Node<T>& n);

/*....DO NOT EDIT BELOW....*/
template<class T>
class Node {
    private:
        Node<T> *next;
        T data;
    public:
        Node();
        Node(const T& d);
        Node<T>* getNext() const;
        T getData() const;
        T* getDataPtr();
        void setNext(Node<T> *newNext); 
        friend ostream &operator<< <> (ostream &out, const Node<T>& n);
};
template<class T>
ostream &operator<<(ostream &out, const Node<T>& n){
    out << n.data ; 
    return out;
};
/*....DO NOT EDIT ABOVE....*/


/* TO-DO: method implementations below */
 

/* end of your implementations*/
#endif	

