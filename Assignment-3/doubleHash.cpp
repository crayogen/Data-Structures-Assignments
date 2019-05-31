#ifndef DOUBLEHASH_CPP
#define DOUBLEHASH_CPP
#include "doubleHash.h"
#include "hashfunctions.cpp"

HashD::HashD(){
    tableSize = 10000; // you cant change this
    hashTable = new block*[tableSize];
    for (int i=0; i<tableSize; i++){
    	hashTable[i] = NULL;
    }
    count = 0;
}

HashD::~HashD(){
    delete [] hashTable;
}

unsigned long HashD::hash1(string value) {
	return divCompression(5-polyHash(value, 41), tableSize);
}

unsigned long HashD::hash2(string value) {
	return divCompression(bitHash(value)+1, tableSize);
}
void HashD::resizeTable(){
	int old_tableSize = tableSize;
	block** old_hashTable = hashTable;

	tableSize *= 2;

	hashTable = new block*[tableSize];
	for (int i=0; i<tableSize; i++){
		hashTable[i] = NULL;
    }

    for (int i=0; i<old_tableSize; i++){
    	if (old_hashTable[i] && old_hashTable[i]->key != -1) {
    		insert(old_hashTable[i]->value);
       	}
    }

    delete [] old_hashTable;
}

void HashD::insert(string value){
	if (count > tableSize/3) {
		resizeTable();
	}
	
	int key = hash1(value);
	int index = key;
	int i = 1;

	while (hashTable[index] && hashTable[index]->key != -1) {
		index += ( i*hash2(value) );
		index %= tableSize;
		i++;
	}
	if (hashTable[index]) {
		delete hashTable[index];
	}
	hashTable[index] = new block(key, value);
	count++;
}

void HashD::deleteWord(string value){
	block* my_block = lookup(value);
	if (my_block) {
		my_block->key = -1;
		my_block->value = "";
		count--;
	}
}

block* HashD::lookup(string value){
	int key = hash1(value);
	int index = key;
	int i = 1;
	while (hashTable[index] && hashTable[index]->value != value) {
		index += i*hash2(value);
		index %= tableSize;
		i++;
	}
	return hashTable[index];
}

#endif