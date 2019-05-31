#ifndef VCS_CPP
#define VCS_CPP
#include "VCS.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

//base constructor, init
//The reason two stacks are used are because undo and redo are both fifo operations
//i.e the last chnage to be undone will be the first to be redone and vice versa.

VCS::VCS(string filename)
{
	this->filename = filename;
	container1 = Stack<VCS_node>();
	container2 = Stack<VCS_node>();
	curr_size=0;
}

//destructor
VCS::~VCS()
{
}

string readFromFileAndWriteToAnotherFile(string filenameToReadFrom, string filenameToWriteTo)
{
	ifstream inf (filenameToReadFrom + ".txt");
	ofstream outf (filenameToWriteTo+".txt");
	string line, data = "";
	while (getline(inf, line)) {
		data = data + line;
		outf << line;
	}
	outf.close();
	return data;
}


//time to commit
void VCS::commit(string msg)
{
	VCS_node node = VCS_node(msg, curr_size);
	container1.push(node);
	string nextFilename = filename+to_string(curr_size);
	curr_size++;
	readFromFileAndWriteToAnotherFile(filename, nextFilename);
}

//print status of every version
void VCS::status()
{		
	for (int i=0; i<curr_size; i++) {
		VCS_node node = container1.pop();
		//cout << "message: " << node.message << " with " << "time stamp: " << node.time_stamp << endl;
		container2.push(node);
	}
	for (int i=0; i<curr_size; i++) {
		container1.push(container2.pop());
	}
}

//goes one step back
string VCS::undo()
{
	VCS_node remove = container1.pop();
	int t = remove.time_stamp-1;
	string readfile = filename+to_string(t);
	container2.push(remove);
	return readFromFileAndWriteToAnotherFile(readfile, filename);

}

//one step ahead
string VCS::redo()
{
	VCS_node addBack = container2.pop();
	int t = addBack.time_stamp;
	string readfile = filename+to_string(t);
	container1.push(addBack);
	return readFromFileAndWriteToAnotherFile(readfile, filename);
}

//goes t_stamp step back or ahead

string VCS::revert(int t_stamp)
{
	string data = "", line;
	int currentTimeStamp = container1.top().time_stamp;
	int numOfVersionstoStep = currentTimeStamp-t_stamp;
	bool stepBack = (numOfVersionstoStep>0);
	if (numOfVersionstoStep == 0) {
		ifstream inf (filename+to_string(currentTimeStamp)+".txt");
		while (getline(inf, line))
			data = data + line;
	}
	for (int i=0; i<abs(numOfVersionstoStep); i++) {
		if (stepBack) data = undo();
		else data = redo();
	}
	return data;
}

#endif