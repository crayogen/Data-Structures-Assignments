#ifndef LINEARPROBING_CPP
#define LINEARPROBING_CPP

#include "linearProbing.h"
#include "hashfunctions.cpp"

HashL::HashL(){
    tableSize = 1000; // you cant change this
    hashTable = new block*[tableSize];
    for (int i=0; i<tableSize; i++){
    	hashTable[i] = NULL;
    }
    count = 0;
}

HashL::~HashL(){
    delete [] hashTable;
}

unsigned long HashL :: hash(string value){
	return divCompression(polyHash(value, 41), tableSize);
}

void HashL::resizeTable(){
	int old_tableSize = tableSize;
	block** old_hashTable = hashTable;

	tableSize *= 2;
	count = 0;
	hashTable = new block*[tableSize];
	for (int i=0; i<tableSize; i++){
		hashTable[i] = NULL;
    }

    for (int i=0; i<old_tableSize; i++) {
    	if (old_hashTable[i]  && old_hashTable[i]->key != -1) {
    		insert(old_hashTable[i]->value);
       	}
    }
    delete [] old_hashTable;
	// do this
}

void HashL::insert(string value){
	if (count > tableSize/2) {
		resizeTable();
	}
	int key = hash(value);
	int index = key;

	while (hashTable[index] && hashTable[index]->key != -1) {
		index = (index+1) % tableSize;
	}
	if (hashTable[index]) {
		delete hashTable[index];
	}
	hashTable[index] = new block(key, value);
	count++;
}

void HashL::deleteWord(string value){
	block* my_block = lookup(value);
	if (my_block) {
		my_block->key = -1;
		my_block->value = "";
		count--;
	}
}
block* HashL::lookup(string value){
	int key = hash(value);
	int index = key;
	while (hashTable[index] && hashTable[index]->value != value) {
		index = (index+1) % tableSize;
	}
	return hashTable[index];
}
#endif
