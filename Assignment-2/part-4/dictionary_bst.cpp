#ifndef __DICTIONARY_CPP
#define __DICTIONARY_CPP
#include "dictionary_bst.h"
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <string>

Dictionary::Dictionary() {
	wordsTree = new BST<string>();
}

Dictionary::~Dictionary(){
	delete wordsTree;
}

void Dictionary::initialize(string wordsFile) {
	ifstream inFile(wordsFile);
	if (inFile.is_open()) {
		string word;
		while ( getline(inFile, word) ){
			insertWord(word);
		}
	}
}

bool Dictionary::deleteWord(string word) {
	wordsTree->delete_node(word);
	return true;
}
bool Dictionary::editWord(string oldWord, string newWord) {
	deleteWord(oldWord);
	insertWord(newWord);
	return true;
}
bool Dictionary::insertWord(string word) {
	wordsTree->insert(word, word);
	return true;
}

node<string>* Dictionary::findWord(string word) {
	clock_t t; 
    t = clock();
    wordsTree->search(word);
    t = clock() - t; 
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds 
    cout << "findWord() took " << time_taken << " sec to search for " << word << endl;  
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
	cout << "Time (sec) taken by BST (t1)" << endl;
	for ( int i=0; i<26; i++ ) {
		myD.findWord(wordsToSearch[i]);
	}
	cout << "done" << endl;
	return 0;
}
