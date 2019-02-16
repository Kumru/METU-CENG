#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);

    TwoPhaseBST<T>::SecondaryNode *findMin(struct SecondaryNode *t) const;
    void removeRec(const std::string &x, struct SecondaryNode * & t) const;

private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    if(!root){
        SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
        PrimaryNode* prim = new PrimaryNode(primaryKey, NULL, NULL, sec);
        root = prim;
        return *this;
    }
    else{
        PrimaryNode* primPlace = root;
        while(primPlace->key != primaryKey){
            if(primaryKey < primPlace->key){
                if(!primPlace->left){
                    SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
                    PrimaryNode* prim = new PrimaryNode(primaryKey, NULL, NULL, sec);
                    primPlace->left = prim;
                    return *this;
                }
                primPlace = primPlace->left;
            }
            else{
                if(!primPlace->right){
                    SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
                    PrimaryNode* prim = new PrimaryNode(primaryKey, NULL, NULL, sec);
                    primPlace->right = prim;
                    return *this;
                }
                primPlace = primPlace->right;
            }
        }
        SecondaryNode* secPlace = primPlace->rootSecondaryNode;
        if(!secPlace){
            SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
            primPlace->rootSecondaryNode = sec;
            return *this;
        }
        else{
            while(secPlace->key != secondaryKey){
                if(secondaryKey < secPlace->key){
                    if(!secPlace->left){
                        SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
                        primPlace->rootSecondaryNode = sec;
                        return *this;
                    }
                    secPlace = secPlace->left;
                }
                else{
                    if(!secPlace->right){
                        SecondaryNode* sec = new SecondaryNode(secondaryKey, data, NULL, NULL);
                        primPlace->rootSecondaryNode = sec;
                        return *this;
                    }
                    secPlace = secPlace->right;
                }
            }
        }
    }
}
/*
template <class T>
typename TwoPhaseBST<T>::SecondaryNode *
TwoPhaseBST<T>::findMin(struct SecondaryNode *t) const {
   if(t == NULL)
       return NULL;
   if(t->left == NULL)
       return t;
   return findMin(t->left);
}

template <class T>
void TwoPhaseBST<T>::removeRec(const std::string &x, struct SecondaryNode * & t) const {
   if(t == NULL)
      return;                 // item not found; do nothing
   if(x < t->key)
      removeRec(x, t->left);     // trying to find
   else if(t->key < x)
      removeRec(x, t->right);    // trying to find
   else if(t->left != NULL && t->right != NULL) { 
       // found – it has two children: copy min of right subtree here
       t->data = findMin(t->right)->data;
       t->key = findMin(t->right)->key;
       removeRec(t->key, t->right);  // and delete its original node
   }
   else { // it has zero or one child: move that or right child here
      SecondaryNode *oldNode = t;
      t = (t->left != NULL) ? t->left : t->right;
      delete oldNode;  // delete it
    }
}
*/
template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    PrimaryNode* primPlace = root;
    if(!primPlace) return *this;
    while(primaryKey != primPlace->key){
        if(primaryKey < primPlace->key){
            if(!primPlace->left) return *this;
            primPlace = primPlace->left;
        }
        else{
            if(!primPlace->right) return *this;
            primPlace = primPlace->right;
        }
    }
    
    /*removeRec(secondaryKey, primPlace->rootSecondaryNode);
    return *this;*/

    SecondaryNode* secPlace = primPlace->rootSecondaryNode;
    SecondaryNode* prevPlace = NULL;
    bool isRightFlag = false;
    if(!secPlace) return *this;
    while(secondaryKey != secPlace->key){
        if(secondaryKey < secPlace->key){
            if(!secPlace->left) return *this;
            prevPlace = secPlace;
            secPlace = secPlace->left;
            isRightFlag = false;
        }
        else{
            if(!secPlace->right) return *this;
            prevPlace = secPlace;
            secPlace = secPlace->right;
            isRightFlag = true;
        }
    }
    //we found the node that we want to remove and its predecessor

    //case 1: it's a leaf
    if(!secPlace->left && !secPlace->right){
        //root
        if (!prevPlace){
            primPlace->rootSecondaryNode = NULL;
            delete secPlace;
            return *this;
        }
        delete secPlace;
        if(isRightFlag)
            prevPlace->right = NULL;
        else
            prevPlace->left = NULL;

        return *this;
    }

    //case 2: it has only 1 child
    else
    if(!secPlace->left || !secPlace->right){
        //it has only left child
        SecondaryNode *child = NULL;
        if(!secPlace->right)
            child = secPlace->left;
        else
            child = secPlace->right;
        
        //root
        if(!prevPlace){
            primPlace->rootSecondaryNode = child;
            delete secPlace;

            return *this;
        }
        else{
            if(isRightFlag)
                prevPlace->right = child;
            else
                prevPlace->left = child;
            delete secPlace;

            return *this;
        }
    }
    //case 3: unfortunately, has 2 child
    else{
        SecondaryNode *sex, *prevSex;
        bool isSexRightFlag = true;
        prevSex = secPlace;
        sex = secPlace->right;
        while(sex->left){
            prevSex = sex;
            sex = sex->left;
            isSexRightFlag = false;
        }
        //this is some hardcore shit, hope it works
        //there is 2, 2 different cases.
        if(isRightFlag){
            if(isSexRightFlag){
                sex->left = prevSex->left;
                prevPlace->right = sex;
                delete secPlace;
                return *this;
            }
            else{
                prevSex->left = sex->right;
                sex->right = secPlace->right;
                sex->left = secPlace->left;
                delete secPlace;
                return *this;
            }
        }
        else{
            if(isSexRightFlag){
                sex->left = prevSex->left;
                prevPlace->left = sex;
                delete secPlace;
                return *this;
            }
            else{
                prevSex->left = sex->right;
                sex->right = secPlace->right;
                sex->left = secPlace->left;
                delete secPlace;
                return *this;
            }
        }
    }
}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    if(primaryKey.empty() && secondaryKey.empty()){
        std::stack<TwoPhaseBST::PrimaryNode *> primStack;
        PrimaryNode* currPrim;
        std::stack<TwoPhaseBST::SecondaryNode *> secStack;
        SecondaryNode* currSec;

        currPrim = root;
        std::cout << "{";
        while(currPrim || !primStack.empty()){
            while(currPrim){
                primStack.push(currPrim);
                currPrim = currPrim->left;
            }

            currPrim = primStack.top();
            primStack.pop();

            std::cout << "\"" << currPrim->key << "\" : {";

            //print the subtree
            while(currSec || !secStack.empty()){
                while(currSec){
                    secStack.push(currSec);
                    currSec = currSec->left;
                }
                
                currSec = secStack.top();
                secStack.pop();

                std::cout << "\"" << currSec->key << "\" : \"" << currSec->data << "\"";
                if(!secStack.empty()) std::cout << ", ";

                currSec = currSec->right;
            }

            std::cout << "}";
            if(!primStack.empty()) std::cout << ", ";

            currPrim = currPrim->right;
        }
        std::cout << "}";
        return *this;
    }

    
}

template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    PrimaryNode* primPlace = root;
    if(!primPlace) return NULL;
    while(primaryKey != primPlace->key){
        if(primaryKey < primPlace->key){
            if(!primPlace->left) return NULL;
            primPlace = primPlace->left;
        }
        else{
            if(!primPlace->right) return NULL;
            primPlace = primPlace->right;
        }
    }
    SecondaryNode* secPlace = primPlace->rootSecondaryNode;
    if(!secPlace) return NULL;
    while(secondaryKey != secPlace->key){
        if(secondaryKey < secPlace->key){
            if(!secPlace->left) return NULL;
            secPlace = secPlace->left;
        }
        else{
            if(!secPlace->right) return NULL;
            secPlace = secPlace->right;
        }
    }
    return &(secPlace->data);
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

#endif //TWOPHASEBST_H
