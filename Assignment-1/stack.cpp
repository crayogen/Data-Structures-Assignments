#ifndef __STACK_CPP
#define __STACK_CPP
#include "stack.h"

template <class T>
Stack<T>::Stack()
{
	list = LinkedList<T>();
}
    
template <class T>
Stack<T>::Stack(Stack<T>& otherStack):list(otherStack.list)
{
}

template <class T>
Stack<T>::~Stack()
{
}

template <class T>
void Stack<T>::push(T item)
{
	list.insertAtHead(item);
}

template <class T>
T Stack<T>::top()
{
	return list.getHead()->value;
}

template <class T>
T Stack<T>::pop()
{
	T val = top();
	list.deleteHead();
	return val;
}

template <class T>
int Stack<T>::length()
{
	return list.length();
}

template <class T>
bool Stack<T>::isEmpty()
{
	return length() == 0;
}

#endif
