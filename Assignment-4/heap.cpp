#ifndef __HEAP_CPP
#define __HEAP_CPP
#include "heap.h"


MinHeap::MinHeap(int cap)
{
	capacity = cap;
 	harr = new int[cap];
 	heap_size = 0;
}


void MinHeap::MinHeapify(int i)
{
	int _min;
	int _left = left(i);
	int _right = right(i);
	if (_left < heap_size && harr[_left] < harr[i]) {
		if (_right < heap_size && harr[_right] < harr[_left])
			_min = _right;
		else
			_min = _left;
	}
	else if (_right < heap_size && harr[_right] < harr[i])
		_min = _right;
	else
		return;
	int min_child = harr[_min];
	harr[_min] = harr[i];
	harr[i] = min_child;
	MinHeapify(_min);

}
 
int MinHeap::parent(int i)
{
	return (i-1)/2;
}
 
int MinHeap::left(int i)
{
	return 2*i+1;
}
 
int MinHeap::right(int i)
{
	return 2*i+2;
}
 
int MinHeap::extractMin()
{	
	int min = getMin();
	if (heap_size == 1) {
		heap_size--;
		return min;
	}
	harr[0] = harr[heap_size-1];
	heap_size--;
	MinHeapify(0);
	return min;
}
 
void MinHeap::decreaseKey(int i, int new_val)
{
	harr[i] = new_val;
	if (i==0)
		return;
	for (int j=i; j>=0;) {
		int _parent = parent(j);
		if (harr[_parent] > harr[j]) {
			int p = harr[_parent];
			harr[_parent] = harr[j];
			harr[j] = p;
			j = parent(j);
		}
		else
			break;
	}
}
 
int MinHeap::getMin()
{
	return harr[0];
}
 
void MinHeap::deleteKey(int i)
{
	if (i >= heap_size)
		return;
	if (heap_size == 1) {
		heap_size--;
		return;
	}
	harr[i] = harr[heap_size-1];
	heap_size--;
	if (harr[i] < harr[parent(i)]) {
		cout << "her";
		decreaseKey(i, harr[i]);
	}
	else
		MinHeapify(i);
}
 
void MinHeap::insertKey(int k)
{
	decreaseKey(heap_size, k);
	heap_size++;
}

int* MinHeap::getHeap()
{
	return harr;
}

#endif