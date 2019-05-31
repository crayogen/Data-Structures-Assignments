//include any header files as required
#ifndef NOCACHE_CPP
#define NOCACHE_CPP

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>

#include <vector>
#include "hashfunctions.cpp"

using namespace std;

string get_word(string key) {
	ifstream file;
    file.open("dictionary.txt");
    string temp_key;
 	string temp_value;

    while(!file.eof()) {
        file >> temp_key;
        file >> temp_value;
        if (key == temp_key) {
        	file.close();
        	// cout << temp_value << endl;
        	return temp_value;
        }
    }
    file.close();
	return "";
}

int main() {
	using namespace std::chrono;

	high_resolution_clock::time_point timeStart = high_resolution_clock::now();;
    
    ifstream file;
    string temp;
    file.open("secret1.txt");
    while(!file.eof()){
	    getline(file, temp, ',');
	    if (temp[0] == ' ') {
	    	temp=temp.substr(1, temp.length());
	    	string v = get_word(temp);
		}
	}
	high_resolution_clock::time_point timeEnd = high_resolution_clock::now();;
    duration<double> totalTime = duration_cast<duration<double>>(timeEnd - timeStart);

    cout << "FINISHED IN: " << totalTime.count() << " SECONDS."<<endl;

	return 0;
}

#endif
