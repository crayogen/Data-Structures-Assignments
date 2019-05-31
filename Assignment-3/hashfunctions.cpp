#ifndef HASHFUNCTIONS_CPP
#define HASHFUNCTIONS_CPP
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
// this takes in a string and returns a 64bit hash.
unsigned long polyHash(std::string value,int a = 9) {
	int length = value.length();
	unsigned long poly_hash = value.at(0);
	for (int i=1; i<length; i++) {
		poly_hash = poly_hash*a + value.at(i);
	}
	return poly_hash;
}

//does some bitwise operation and generates a 64bit hash.
unsigned long bitHash(std::string value){
	int length = value.length();
	unsigned long bit_hash = 0;
	for (int i=0; i<length; i++) {
		bit_hash ^= (bit_hash<<5) + (bit_hash>>2) + value.at(i);
	}
	return bit_hash;
}
// Size is the size of array maintained by the hashtable.
unsigned long divCompression(unsigned long hash,long size){
	return (hash % size);
}

// multiplication addition and division compression. 
unsigned long madCompression(unsigned long hash,long size,int m = 1993,int a = 1637){
	return (a*hash + m) % size;
}
#endif
// you may write your own program to test these functions.