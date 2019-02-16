#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head; 
    int length;  
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data); 
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template <class T>
LinkedList<T>::LinkedList(){
    head = new Node<T>;
    length = 0;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
    head = new Node<T>;
    length = 0;
    *this = ll;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
    if(this != &ll){
        clear();

        Node<T>* h = getHead();
        const Node<T>* fr = ll.first();
        while(fr){
            insertNode(h, fr->getData());
            fr = fr->getNext();
            h = h->getNext();
        }
    }
    return *this;
}

template <class T>
LinkedList<T>::~LinkedList(){
    clear();
    delete head;
}

template <class T>
Node<T>* LinkedList<T>::getHead() const{
    return head;
}

template <class T>
Node<T>* LinkedList<T>::first() const{
    return head->getNext();
}

template <class T>
Node<T>* LinkedList<T>::findNode(const T& data) const{
    Node<T>* a = first();
    while(a){
        if(a->getData() == data) return a;
        a = a->getNext();
    }
    return NULL;
}

template <class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const{
    Node<T>* a = getHead();
    while(a->getNext()){
        if(a->getNext()->getData() == data) return a;
        a = a->getNext();
    }
    return NULL;
}

template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){
    Node<T>* temp = new Node<T>(data);
    temp->setNext(prev->getNext());
    prev->setNext(temp);
    length++;
}

template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
    Node<T>* temp = prevNode->getNext();
    prevNode->setNext(temp->getNext());
    delete temp;
    length--;
}

template <class T>
void LinkedList<T>::clear(){
    while(head->getNext()){
        deleteNode(head);
    }
}

template <class T>
size_t LinkedList<T>::getLength() const{
    return length;
}

template <class T>
void LinkedList<T>::swap(int index1, int index2){
    Node<T>* fst = getHead();
    Node<T>* scd = getHead();
    T tmp;

    for(int i=0;i<=index1;i++) fst = fst->getNext();
    for(int i=0;i<=index2;i++) scd = scd->getNext();

    tmp = scd->getData();
    *(scd->getDataPtr()) = fst->getData();
    *(fst->getDataPtr()) = tmp;
}
/*
void LinkedList<T>::swap(int index1, int index2){
    Node<T>* fstPrev = getHead();
    Node<T>* scdPrev = getHead();
    Node<T>* tmp     = NULL;

    for(int i=0;i<index1;i++) fstPrev = fstPrev->getNext();
    for(int i=0;i<index2;i++) scdPrev = scdPrev->getNext();

    tmp = scdPrev->getNext();
    scdPrev->setNext(fstPrev->getNext());
    fstPrev->setNext(tmp);
    tmp = fstPrev->getNext()->getNext();
    fstPrev->getNext()->setNext(scdPrev->getNext()->getNext());
    scdPrev->getNext()->setNext(tmp);
}*/

/* end of your implementations*/
#endif	

