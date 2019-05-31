#ifndef __DICTIONARY_CPP
#define __DICTIONARY_CPP
#include "dictionary_AVL.h"
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <string>

Dictionary::Dictionary() {
	wordsTree = new BST<string>();
}

Dictionary::~Dictionary(){
}

void Dictionary::initialize(string wordsFile) {
	ifstream inFile(wordsFile);
	if (inFile.is_open()) {
		string word;
		while ( getline(inFile, word) ) {
			wordsTree->insert(word, word);
		}
	}
}

bool Dictionary::deleteWord(string word) {
	wordsTree->delete_node(word);
	return false;
}
bool Dictionary::editWord(string oldWord, string newWord) {
	wordsTree->delete_node(oldWord);
	wordsTree->insert(newWord, newWord);
	return false;
}
bool Dictionary::insertWord(string word) {
	wordsTree->insert(word, word);
	return false;
}
node<string>* Dictionary::findWord(string word) {
	clock_t t; 
    t = clock();
    wordsTree->search(word);
    t = clock() - t; 
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds 
    cout << "findWord() took " << time_taken << "sec to search for " << word << endl;  
    return NULL; // TODO
}

#endif

void getWordsDistinctAlph(string wordsFile, string arr[]) {
	ifstream inFile(wordsFile);
	if (inFile.is_open()) {
		string word;
		char c = 'a';
		int counter = 0;
		int i = 0;
		while ( getline(inFile, word) ) {
			if (i > 51000 && word[0] == c) {
				c++;
				arr[counter++] = word;
				if (counter == 26) break;
			}
			else i++;
		}
	}
}

int main() {
	string wordsFile = "words1.txt";
	string wordsToSearch[26];
	getWordsDistinctAlph(wordsFile, wordsToSearch);

	Dictionary myD = Dictionary();
	cout << "begining initialization..." << endl;
	myD.initialize(wordsFile);
	cout << "initialize finished..." << endl;
	cout << "Time (sec) taken by AVL (t2)" << endl;
	for ( int i=0; i<26; i++ ) {
		myD.findWord(wordsToSearch[i]);
	}
	return 0;
}
