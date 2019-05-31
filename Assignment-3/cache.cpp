//include any header files as required
#ifndef CACHE_CPP
#define CACHE_CPP

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>

#include <vector>
#include "hashfunctions.cpp"

using namespace std;

class block{
    public:
        string key;
        string value;
        int freq;
        block(string _key, string _value, int _freq){
            this->key = _key;
            this->value = _value;
            this->freq = _freq;
        }
};

class Cache{
    private:
        block** hashTable;
        long tableSize;
        int least_frequent_index;
        unsigned long hash(string value); // return the corresponding key of the value
        long count; // keeps a count of the number of entries in the table.
    public:
        Cache();
        ~Cache();
        void insert(string key, string value);
		block* lookup(string key);
};

Cache::Cache(){
    tableSize = 1000;
    hashTable = new block*[tableSize];
    for (int i=0; i<tableSize; i++){
    	hashTable[i] = NULL;
    }
    count = 0;
}

Cache::~Cache(){
    delete [] hashTable;
}

unsigned long Cache :: hash(string value){
	return divCompression(polyHash(value, 41), 1000);
}

void Cache::insert(string key, string value){
	int threshold = tableSize-1;
	if (count > tableSize/2) {
		threshold = 3;
	}
	int my_hash = hash(key);
	for (int index = my_hash; index<my_hash+threshold; index++){
		index = (index+1) % tableSize;
		if (hashTable[index] && hashTable[index]->freq < 2) {
			delete hashTable[index];
		}
		else if (hashTable[index])
			continue;
		hashTable[index] = new block(key, value, 0);
		count++;
		break;
	}
}

block* Cache::lookup(string key){
	int my_hash = hash(key);
	int index = my_hash;
	while (hashTable[index] && hashTable[index]->key != key){
		index = (index+1) % tableSize;
	}
	if (hashTable[index]){
		hashTable[index]->freq++;
	}
	return hashTable[index];
}

class dictItem {
	public:
		string key;
		string value;
		dictItem(string k, string v) {
			this->key = k;
			this->value = v;
		}
};

int i=0;
int j=0;
string get_word(Cache *my_cache, string key) {
	block* my_block = my_cache->lookup(key);
	string value = "";
	if (my_block) {
		value = my_block->value;
		// cout << i++ << ". found in cache " << value << endl;
		// cout << j << endl;
	} else {
		ifstream file;
	    file.open("dictionary.txt");
	    string temp_key;
	 	string temp_value;

	    while(!file.eof()) {
	        file >> temp_key;
	        file >> temp_value;
	        if (key == temp_key) {
	        	my_cache->insert(key, value);
	        	file.close();
	        	// j++;
	        	return temp_value;
	        }
	    }
	    file.close();
	}
	return "";
}

int main() {
	using namespace std::chrono;

	Cache *my_cache = new Cache();

    high_resolution_clock::time_point timeStart = high_resolution_clock::now();;
    
    ifstream file;
    string temp;
    file.open("secret1.txt");
    while(!file.eof()){
	    getline(file, temp, ',');
	    if (temp[0] == ' ') {
	    	temp=temp.substr(1, temp.length());
	    	// cout << temp << endl;
	    	string v = get_word(my_cache, temp);
	    	// cout << "found " << v <<  endl;
		}
	}
	high_resolution_clock::time_point timeEnd = high_resolution_clock::now();;
    duration<double> totalTime = duration_cast<duration<double>>(timeEnd - timeStart);

    cout << "TEST PASSED IN : " << totalTime.count() << " SECONDS."<<endl;

	return 0;
}

#endif
