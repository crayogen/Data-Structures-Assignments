#ifndef __BST_CPP
#define __BST_CPP
#include "bst.h"
#include <iostream>
#include <queue>

// HELPER FUNCTIONS HERE...
template <class T>
BST<T>::BST() {
    root = NULL;
}

template <class T>
BST<T>:: ~BST() {
    while (root) root = removemin(root);
}

template <class T>
int BST<T> :: height (node<T>* p){
    if (p==NULL) return 0;
    else return (1 + max (height(p->left), height(p->right)));
}

template <class T>
int BST<T> :: balanceFactor(node<T>* p){
    if (p==NULL)
        return 0;
    return ( height(p->left) - height(p->right) );
}

template <class T>
void BST<T> :: fixHeight(node<T>* p){ //fix height after insertion or deletion
    if (p) {
        p->height = height(p);
    }
}

// ROTATING HELPERS 
template <class T>
node<T>* BST<T> :: rotateleft(node<T>* p){
    node<T> *new_p = p->right;
    p->right = new_p->left;
    new_p->left = p;
    return new_p;
}

template <class T>
node<T>* BST<T> :: rotateright(node<T>* p){
    node<T> *new_p = p->left;
    p->left = new_p->right;
    new_p->right = p;   
    return new_p;
}

template <class T>
node<T>* BST<T> :: balance(node<T>* p) {
    return p;
    // Balancing code goes here. You might need to find balance factor and call appropriate rotations.
}

// CLASS FUNCTIONS HERE
template <class T>
void BST<T> :: insert(string value,T k){
    root = insertHelper(value,k,root);
}
// insertion helper
template <class T>
node<T>* BST<T> :: insertHelper(string value,T k, node<T> *p) { // note we need a helper because we need recursive calls
    if (p==NULL)
        return new node<T>(k, value);
    if (p->key > k)
        p->left = insertHelper(value, k, p->left);
    else
        p->right = insertHelper(value, k, p->right);
    return balance(p);
}

template<class T>
node<T>* BST<T> :: search(T key){
    node<T>* node = root;
    while(node != NULL) {
        if (node->key == key)
            return node;
        if (node->key > key)
            node = node->left;
        else
            node = node->right;
    }
    return node;

}
// DELETE HELPER FUNCTIONS
template<class T>
node<T>* BST<T> :: findmin(node<T> *p) {
    node<T>* node = p;
    if (node == NULL)
        return NULL;
    while (node->left != NULL)
        node = node->left;
    return node;
}

template<class T>
node<T>* BST<T>::removemin(node<T>* p) {
    node<T>* toDelete = NULL;
    if (p->left == NULL) {
        toDelete = p;
        p = p->right;
    } else if (p->left->left == NULL) {
        toDelete = p->left;
        p->left = p->left->right;\
    } else {
        p->left = removemin(p->left);
    } if (toDelete) {
        delete toDelete;
    }
    return balance(p);
    // This function recursively finds the min node and deletes it.
}

template<class T>
void BST<T>::delete_node(T k) {
    root = remove(root, k);
}

template<class T>
node<T>*  BST<T>::remove(node<T>* p, T k) // k key deletion from p tree
{
    if (p==NULL)
        return p;
    if (p->key == k) {
        node<T>* minOfRightTree = findmin(p->right);
        if (minOfRightTree == NULL) {
            node<T>* toDelete = p;
            p = p->left;
            delete toDelete;
        } else {
            p->value = minOfRightTree->value;
            p->key = minOfRightTree->key;
            p->right = removemin(p->right);
        }
    } else if (p->key > k) {
        p->left = remove(p->left, k);
    } else {
        p->right = remove(p->right, k);
    }
    return balance(p);
    // This function uses findmin and deletemin to delete arbitrary node in the tree.
}
template<class T>
node<T>* BST<T>::getRoot(){
    return root;
}
#endif
