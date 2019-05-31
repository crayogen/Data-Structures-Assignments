#ifndef GRAPH__H
#define GRAPH__H
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <set>
#include <algorithm>
#include <queue>
#include <list>

// You may include more libraries.

using namespace std;
class node;

class edge
{
	// You may add more variables and functions in this class
	public:
		node* Origin;
		node* Dest;
		float weight;
		edge (node* Origin, node* Dest, float weight) {
			this->Origin = Origin;
			this->Dest = Dest;
			this->weight = weight;
		}
};
class node
{
	// You may add more variables and functions in this class
	public:
		int unique_id;
		int tentative_dist;
		int visit = 0; // flag to check if the node has been already visited
		int enqueued = 0;
		string name;
		vector<edge> mst_edges;
		vector<edge> edges; // edges connected to node
		vector<string> tentative_path;
		node (string name) {
			this->visit = 0;
			this->name = name;
		};
};


class Graph
{
	public:
		vector<node*> Mst; // minimum spanning tree
		vector<node*> people;
		vector<edge> arcs;
		Graph(char* filename, bool isUnitLenght);
		void display(node* temp);// displays the graph

		bool Reachable(string start, string dest);
		vector<node*> Prims();
		vector<node*> Kruskal(); //runs kruskal algorithm and returns the Minimum spanning tree
		vector<string> Dijkstra(string start, string dest, float &d);
};

#endif

