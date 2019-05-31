#ifndef __SORTS_CPP
#define __SORTS_CPP
#include "sorts.h"
#include "heap.cpp"
#include "list.cpp"

//=====================================================================================
void InsertionSortArray(long* arr, int size) {
	for (int i=1; i<size; i++) {
        for (int j=i; j>0; j--) {
            if (arr[j] < arr[j-1]) {
                long temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = temp;
            } else
                break;
        }
    }
}

vector<long> InsertionSort(vector<long> nums)
{
	int size = nums.size();
    InsertionSortArray(nums.data(), size);
    return nums;
}

//=====================================================================================
LinkedList<long>* merge(LinkedList<long>* l, LinkedList<long>* r) {
    if (l->length() == 0)
        return r;
    if (r->length() == 0)
        return l;
    long last_l = l->getTail()->value;
    long last_r = r->getTail()->value;
    long to_push_back;
    if (last_r < last_l) {
        l->deleteTail();
        to_push_back = last_l;
    } else {
        r->deleteTail();
        to_push_back = last_r;
    }
    LinkedList<long>* merged = merge(l, r);
    merged->insertAtTail(to_push_back); 
    return merged;
}

LinkedList<long>* MergeSortList(LinkedList<long>* list) {
    if (list->length() == 1)
        return list;
    LinkedList<long>* r = list->split();
    LinkedList<long>* l = list;
    return merge(MergeSortList(l), MergeSortList(r));
}

vector<long> MergeSort(vector<long> nums)
{
    LinkedList<long> *numsList = new LinkedList<long>();
    int size = nums.size();
    for (int i=0; i<size; i++) {
        numsList->insertAtHead(nums[i]);
    }
    numsList = MergeSortList(numsList);
    for (int i=0; i<size; i++) {
        nums[i] = (numsList->getHead())->value;
        numsList->deleteHead();
    }
    return nums;
}

//=====================================================================================
void swap(long* nums, int i, int j) {
    long temp = nums[i];
    nums[i] = nums[j];
    nums[j] = temp;
}


void medianOfThreeAsPivot(long* nums, int size) {
	int first = rand()%size;
	int second = rand()%size;
	int third = rand()%size;
	if (nums[second]  < nums[first])
		swap(first, second);
	if (nums[third] < nums[second])
		swap(second, third);
	if (nums[third] < nums[first])
		swap(first, third);
	swap(nums, second, 0);
}

void lastAsPivot(long* nums, int size) {
	swap(nums, size-1, 0);
}

long* _QuickSortArray(long* nums, int size) {
    if (size == 1 || size == 0)
        return nums;
    // lastAsPivot(nums, size);
	// medianOfThreeAsPivot(nums, size);
    long pivot = nums[0];
    int lo_size = 0, hi_size = 0;
    int index_to_compare=1;
    for (int j=0; j<size-1; j++) {
        int temp, hi_index;
        if (nums[index_to_compare] < pivot)
            nums[lo_size++] = nums[index_to_compare++];
        else {
            hi_index = size-hi_size-1;
            swap(nums, hi_index, index_to_compare);
            hi_size++;
        }
    }
    _QuickSortArray(nums, lo_size);
    nums[lo_size] = pivot;
    _QuickSortArray(nums+lo_size+1, hi_size);
    return nums;
}

vector<long> QuickSortArray(vector<long> nums)
{
    int size = nums.size();
    _QuickSortArray(nums.data(), size);
    return nums;
}


//=====================================================================================
void deleteNode(LinkedList<long>* nums, ListItem<long>* node) {
    if (node->prev == NULL) {
        nums->deleteHead();
        return;
    }
    if (node->next == NULL) {
        nums->deleteTail();
        return;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    node = NULL;
}

long getRandomPivot(LinkedList<long>* nums) {
    int size = nums->length();
    int pivot_loc = rand() % size;
    ListItem<long>* node = nums->getHead();
    for (int i=0; i<pivot_loc; i++)
        node=node->next;
    long pivot_value = node->value;
    deleteNode(nums,node);
    return pivot_value;
}

LinkedList<long>* _QuickSortList(LinkedList<long>* nums)
{
    int size = nums->length();
    if (size == 1 || size == 0)
        return nums;
    long pivot = getRandomPivot(nums);
    LinkedList<long>* lo = new LinkedList<long>();
    LinkedList<long>* hi = new LinkedList<long>();
    ListItem<long>* item = nums->getHead();
    while (item != NULL) {
        long value = item->value;
        if (value < pivot)
            lo->insertAtHead(value);
        else
            hi->insertAtHead(value);
        item = item->next;
    }
    LinkedList<long> *lo_sorted = _QuickSortList(lo);
    LinkedList<long> *hi_sorted = _QuickSortList(hi);
    hi_sorted->insertAtHead(pivot);
    lo_sorted->joinLists(hi_sorted);
    return lo_sorted;
}

vector<long> QuickSortList(vector<long> nums)
{
    LinkedList<long> *numsList = new LinkedList<long>();
    int size = nums.size();
    for (int i=0; i<size; i++) {
        numsList->insertAtHead(nums[i]);
    }
    numsList = _QuickSortList(numsList);
    for (int i=0; i<size; i++) {
        nums[i] = (numsList->getHead())->value;
        numsList->deleteHead();
    }
    return nums;
}

//=====================================================================================
vector<long> HeapSort(vector<long> nums)
{
	int size = nums.size();
    MinHeap* heap = new MinHeap(size);
	for (int i=0; i<size; i++) {
		heap->insertKey(nums[i]);
	
	}
	for (int i=0; i<size; i++) {
		nums[i] = heap->extractMin();
	}
	return nums;
}

//=====================================================================================
struct Item
{
    long value;
    int size_negative;
    int size_positive;
    Item(long value)
    {
        this->value = value;
        this->size_positive = 0;
        this->size_negative = 0;
    }
};

int size;
Item** resizeArray(Item** arr) {
	int new_size = size*2;
	Item** new_arr = new Item*[new_size];
	for (int i=0; i<size; i++)
		new_arr[i] = arr[i];
	for (int i=size; i<new_size; i++)
		new_arr[i] = NULL;
	size = new_size;
	return new_arr;
}

Item** _QueeenOfAllSorts(long* nums, int nums_size) {
	size = nums_size;
	Item** arr = new Item*[size];
	for (int i=0; i<size; i++)
		arr[i] = NULL;
	for (int i=0; i<nums_size;) {
		long item_to_sort = abs(nums[i]);
		if (item_to_sort >= size) {
			arr = resizeArray(arr);
			continue;
		}
		if (!arr[item_to_sort])
			arr[item_to_sort] = new Item(item_to_sort);
		if (nums[i++] < 0)
			arr[item_to_sort]->size_negative ++;
		else
			arr[item_to_sort]->size_positive ++;
	}
	return arr;
}

vector<long> QueenOfAllSorts(vector<long> nums)
{
    int nums_size = nums.size();
    Item **sorted = _QueeenOfAllSorts(nums.data(), nums_size);
    int lo_index=0;
    int hi_index=nums_size-1;
    for (int i=size-1; i>=0; i--) {
    	if (sorted[i]) {
    		long value = sorted[i]->value;
    		while(sorted[i]->size_positive--) {
    			nums[hi_index--] = value;
    		}
    		while(sorted[i]->size_negative--) {
    			nums[lo_index++] = -value;
    		}
    	}
    }
    // for (int i=0; i<nums_size; i++) {
    // 	cout << nums[i] << " " <<std::flush;
    // }
    return nums;
}
#endif