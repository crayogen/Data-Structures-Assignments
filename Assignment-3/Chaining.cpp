#ifndef CHAINING_CPP
#define CHAINING_CPP
#include "Chaining.h"
#include "hashfunctions.cpp"
#include "LinkedList.h"

HashC::HashC(int size){
	tableSize = size;
	hashTable = new LinkedList<string>[size];
}

HashC::~HashC(){
	for (int i=0; i<tableSize; i++){
		hashTable[i].~LinkedList();
	}
	delete [] hashTable;
}

unsigned long HashC :: hash(string input){
	return divCompression(polyHash(input, 41), tableSize);
}

void HashC::insert(string word){
	hashTable[hash(word)].insertAtHead(word);
} 

ListItem<string>* HashC :: lookup(string word){
	return hashTable[hash(word)].searchFor(word);
}

void HashC :: deleteWord(string word){
	hashTable[hash(word)].deleteElement(word);
}


#endif