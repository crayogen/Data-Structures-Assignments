#ifndef __LIST_CPP
#define __LIST_CPP

#include <cstdlib>
#include "list.h"
#include <iostream>
using namespace std;

template <class T>
LinkedList<T>::LinkedList()
{
	head = NULL;
	tail = NULL;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& otherLinkedList):LinkedList()
{
	ListItem<T>* ptr = otherLinkedList.head;
	while (ptr != NULL) {
		insertAtTail(ptr->value);
		ptr = ptr->next;
	}
}

template <class T>
LinkedList<T>::~LinkedList()
{
	ListItem<T>* node = head;
	while (node != NULL) {
		ListItem<T>* myNode = node;
		node = node->next;
		delete myNode;
		myNode = NULL;
	}
}

template <class T>
void LinkedList<T>::insertAtHead(T item)
{
	ListItem<T>* newNode = new ListItem<T>(item);
	newNode->next = head;
	head = newNode;
	if (newNode->next != NULL)
		newNode->next->prev = newNode;
	else
		tail = head;
}

template <class T>
void LinkedList<T>::insertAtTail(T item)
{
	ListItem<T>* newNode = new ListItem<T>(item);
	if (tail == NULL) {
		tail = newNode;
		head = tail;
		return;
	}
	tail->next = newNode;
	newNode->prev = tail;
	tail = newNode;
}

template <class T>
void LinkedList<T>::insertAfter(T toInsert, T afterWhat)
{
	ListItem<T>* node = head;
	while (node->value != afterWhat)
		node = node->next;
	if (node == NULL)
		return;
	else if (node == tail) insertAtTail(toInsert);
	else {
		ListItem<T>* newNode = new ListItem<T>(toInsert);
		newNode->next = node->next;
		newNode->prev = node->next->prev;
		node->next->prev = newNode;
		node->next = newNode;
	}
}


template <class T>
ListItem<T>* LinkedList<T>::getHead()
{
	return head;
}

template <class T>
ListItem<T>* LinkedList<T>::getTail()
{
	return tail;
}

template <class T>
ListItem<T> *LinkedList<T>::searchFor(T item)
{
	ListItem<T>* node = head;
	while ((node != NULL) && (node->value != item)) {
		node = node->next;
	}
	return node;
}

template <class T>
void LinkedList<T>::joinLists(LinkedList<T>* hi)
{
	if (hi->getHead() == NULL)
        return;
    if (head == NULL) {
        head = hi->getHead();
        tail = hi->getTail();
        return;
    }
    ListItem<long>* lo_end = tail;
    ListItem<long>* hi_begin = hi->getHead();
    lo_end->next = hi_begin;
    hi_begin->prev = lo_end;
    tail = hi->getTail();
}

template <class T>
void LinkedList<T>::deleteElement(T item)
{
	ListItem<T> *node = searchFor(item);
	if (node == NULL) return;
	if (node->prev == NULL) {
		deleteHead();
		return;
	}
	if (node->next == NULL) {
		deleteTail();
		return;
	}
	node->prev->next = node->next;
	node->next->prev = node->prev;
	delete node;
	node = NULL;
}

template <class T>
void LinkedList<T>::deleteHead()
{
	if (head == NULL) return;
	ListItem<T> *newHead = head->next;
	if (newHead != NULL) newHead->prev = NULL;
	else tail = NULL;
	delete head;
	head = newHead;
}

template <class T>
void LinkedList<T>::deleteTail()
{
	if (tail == NULL) return;
	ListItem<T> *newTail = tail->prev;
	if (newTail != NULL) newTail->next = NULL;
	else head = NULL;
	delete tail;
	tail = newTail;
}

template <class T>
LinkedList<T>* LinkedList<T>::split()
{
	ListItem<T>* node = head;
	for (int i=1; i<length()/2; i++) {
		node = node->next;
	}
	ListItem<T>* leftEnd = node;
	LinkedList<T> *right = new LinkedList();
	for (int i=length()/2; i<length(); i++) {
		right->insertAtTail(node->next->value);
		node = node->next;
	}
	leftEnd->next = NULL;
	tail = leftEnd;
	return right;
}

template <class T>
void LinkedList<T>::display()
{
	ListItem<T>* node = head;
	while (node != NULL) {
		cout << node->value << " ";
		node = node->next;
	}
	cout << endl;
}

template <class T>
int LinkedList<T>::length()
{
	int len = 0;
	ListItem<T>* node = head;
	while (node != NULL) {
		node = node->next;
		len++;
	}
	return len;
}

#endif
