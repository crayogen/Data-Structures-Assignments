#ifndef GRAPH__CPP
#define GRAPH__CPP
#include "Graph.h"

struct GreaterThanByDist {
	bool operator() (const node* lhs, const node* rhs) { return lhs->tentative_dist > rhs->tentative_dist;}
};

node* findPerson(vector<node*> people, string person) {
	for (int i=0; i<people.size(); i++) {
		if ( (people.at(i))->name == person) {
			return people.at(i);
		}
	}
	return NULL;
}

Graph::Graph(char* file, bool isUnitLength=false)
{
	string line;
	ifstream myfile (file);
	bool division = false;
	int unique_id = 0;
	while ( getline(myfile, line) ) {
		if (line == "PEOPLE") {
			continue;
		} else if (line == "CONNECTIONS") {
			division = true;
			continue;
		}
		if (division) {
			string Origin_string = line.substr(0, line.find(" "));
			line.erase(0, line.find(" ")+1);
			string Dest_string = line.substr(0, line.find(" "));
			line.erase(0, line.find(" ")+1);
			float weight;
			if (isUnitLength)
				weight = 1;
			else
				weight = float (stoi(line));
			node* Origin_node = findPerson(people, Origin_string);
			node* Dest_node = findPerson(people, Dest_string);
			arcs.emplace_back(Origin_node, Dest_node, weight);
			Origin_node->edges.emplace_back(Origin_node, Dest_node, weight);
			Dest_node->edges.emplace_back(Dest_node, Origin_node, weight);
		} else {
			line.pop_back();
			node *person_node = new node(line);
			person_node->unique_id = unique_id++;
			people.push_back(person_node);
		}
	}
	// for (int i=0; i<people.size(); i++) {
	// 	cout << people[i]->name << endl;
	// 	for (int j=0; j<people[i]->edges.size(); j++) {
	// 		cout << "  " << people[i]->edges[j].Dest->name  << " " << people[i]->edges[j].weight << endl;
	// 	}
	// }
}

void Graph::display(node* temp)
{
	if(temp->visit)
		return;
	
	cout << "Person: " << temp->name;
	temp->visit = 1;
	for(int i=0; i< temp->edges.size(); i++)
	{
		if(!temp->edges[i].Origin->visit || !temp->edges[i].Dest->visit)
		cout << "->" << temp->edges[i].weight << endl;
		display(temp->edges[i].Origin);
		display(temp->edges[i].Dest);
	}
}

bool _Reachable(node* start_node, node* dest_node) {
	bool reachable = false;
	if (start_node && dest_node) {
		start_node->visit = 1;
		if (start_node->name == dest_node->name)
			return true;
		vector<edge> edges = start_node->edges;
		for (int i=0; i<edges.size(); i++) {
			node* next_node = edges.at(i).Dest;
			if (next_node->visit != 1) {
				reachable = reachable || _Reachable(next_node, dest_node);
			}
		}
	}
	return reachable;
}

bool Graph::Reachable(string start, string dest)
{
	node* start_node = findPerson(people, start);
	node* dest_node = findPerson(people, dest);
	for (int i=0; i<people.size(); i++) {
		people[i]->visit = 0;
	}
	return _Reachable(start_node, dest_node);
}

void deepCopyMST(node* iter, vector<node*> &copy, node* my_node = NULL) {
	if(iter->visit)
		return;
	iter->visit = 1;
	if (my_node == NULL)
		my_node = new node(iter->name);
	copy.push_back(my_node);
	for(int i=0; i< iter->mst_edges.size(); i++)
	{
		if(!iter->mst_edges[i].Origin->visit) {
			node* origin = new node(iter->mst_edges[i].Origin->name);
			edge e = edge(origin, my_node, iter->mst_edges[i].weight);
			my_node->edges.push_back(e);
			origin->edges.push_back(e);
			deepCopyMST(iter->mst_edges[i].Origin, copy, origin);
		} else if (!iter->mst_edges[i].Dest->visit) {
			node* dest = new node(iter->mst_edges[i].Dest->name);
			edge e = edge(my_node, dest, iter->mst_edges[i].weight);
			dest->edges.push_back(e);
			my_node->edges.push_back(e);
			deepCopyMST(iter->mst_edges[i].Dest, copy, dest);
		}
	}
}

vector<node*> Graph::Prims()
{
	float large_value = 100;
	for (int i=0; i<people.size(); i++) {
		people.at(i)->enqueued = 0;
		people.at(i)->tentative_dist = large_value;
		people.at(i)->visit = 0;
	}
	node* start_node = people[0];
	priority_queue<node*, vector<node*>, GreaterThanByDist> my_priority_queue;
	start_node->tentative_dist = 0;
	my_priority_queue.push(start_node);
	set<node*> spanning_nodes;
	while ( !my_priority_queue.empty() ) {
		node* parent = my_priority_queue.top();
		my_priority_queue.pop();
		vector<edge> edges = parent->edges;
		for (int i=0; i<edges.size(); i++) {
			edge e = edges.at(i);
			node* neighbour = e.Dest;
			if (neighbour->visit != 1) {
				if ( e.weight < neighbour->tentative_dist ) {
					neighbour->tentative_dist = e.weight;
					while (!neighbour->mst_edges.empty())
						neighbour->mst_edges.pop_back();
					neighbour->mst_edges.push_back(e);
				}
				if (!neighbour->enqueued) {
					my_priority_queue.push(neighbour);
					neighbour->enqueued = 1;
				}
			}
		}
		parent->visit = 1;
		vector<edge> es = parent->mst_edges;
		if (!es.empty()) {
			es.back().Origin->mst_edges.push_back(es.back());
		}
		spanning_nodes.insert(parent);
	}
	vector<node*> s(spanning_nodes.begin(), spanning_nodes.end());
	for (int i=0; i<people.size(); i++) {
		people[i]->visit = 0;
	}
	vector<node*> copy;
	deepCopyMST(s[0], copy);
	return copy;
}

struct GreaterThanByWeight {
	bool operator() (const edge &lhs, const edge &rhs) { return lhs.weight > rhs.weight;}
};

set<node*>** makeNodeSets(vector<node*> people) {
	int total_node = people.size();
	set<node*>** node_sets = new set<node*>*[total_node];
	for (int i=0; i<total_node; i++) {
		node_sets[i] = new set<node*>;
		node_sets[i]->insert(people[i]);
	}
	return node_sets;
}

set<node*>* SetUnion(set<node*>** node_sets, set<node*>* origin_set, set<node*>* dest_set) {
	set<node*>::iterator it = dest_set->begin();
	while (it != dest_set->end()) {
		node_sets[(*it)->unique_id] = origin_set;
		origin_set->insert(*it);
		it++;
	}
	delete dest_set;
}

vector<node*> Graph::Kruskal()
{
	set<node*>** node_sets = makeNodeSets(people);
	set<node*> spanning_nodes;
	priority_queue<edge, vector<edge>, GreaterThanByWeight> edges_priority_queue;
	for (int i=0; i<arcs.size(); i++) {
		edges_priority_queue.push(arcs.at(i));
	}
	while ( !edges_priority_queue.empty() ) {
		edge tentative_edge = edges_priority_queue.top();
		node* origin = tentative_edge.Origin;
		node* dest = tentative_edge.Dest;
		set<node*>* origin_set = node_sets[origin->unique_id];
		set<node*>* dest_set = node_sets[dest->unique_id];
		if (origin_set != dest_set) {
			SetUnion(node_sets, origin_set, dest_set);
			origin->mst_edges.push_back(tentative_edge);
			dest->mst_edges.push_back(tentative_edge);
			spanning_nodes.insert(origin);
			spanning_nodes.insert(dest);
		} else {
			// cout << tentative_edge.Origin->name << " " << tentative_edge.Dest->name << endl;
		}
		edges_priority_queue.pop();
	}

	vector<node*> s(spanning_nodes.begin(), spanning_nodes.end());
	vector<node*> copy;
	for (int i=0; i<people.size(); i++) {
		people[i]->visit = 0;
	}
	deepCopyMST(s[0], copy);
	return copy;
}


vector<string> Graph::Dijkstra(string start, string dest, float &d)
{
	float large_value = 100;
	for (int i=0; i<people.size(); i++) {
		people.at(i)->enqueued = 0;
		people.at(i)->visit = 0;
		people.at(i)->tentative_dist = large_value;
	}
	node* start_node = findPerson(people, start);
	node* dest_node = findPerson(people, dest);

	priority_queue<node*, vector<node*>, GreaterThanByDist> my_priority_queue;
	start_node->tentative_dist = 0;
	my_priority_queue.push(start_node);

	vector<string> shortest_path;
	while ( !my_priority_queue.empty() ) {
		node* parent = my_priority_queue.top();
		if ( parent->name  == dest ) {
			d = dest_node->tentative_dist;
			dest_node->tentative_path.push_back(dest);
			return dest_node->tentative_path;
		}
		my_priority_queue.pop();
		vector<edge> edges = parent->edges;
		for (int i=0; i<edges.size(); i++) {
			edge e = edges.at(i);
			node* neighbour = e.Dest;
			if (neighbour->visit != 1) {
				if (!neighbour->enqueued) {
					neighbour->enqueued = 1;
					my_priority_queue.push(neighbour);
				}
				int new_tentative_dist = e.weight + parent->tentative_dist;
				if ( new_tentative_dist < neighbour->tentative_dist ) {
					neighbour->tentative_path = parent->tentative_path;
					neighbour->tentative_path.push_back(parent->name);
					neighbour->tentative_dist = new_tentative_dist;
				}
			}
		}
		parent->visit = 1;
	}
	shortest_path;
}

int main()
{
	// write code for part 6 here
	char* file_name = (char*) "friends.txt";
	Graph *myGraph = new Graph(file_name);
	bool  a = myGraph->Reachable("Mannan", "Ali");
	bool  b = myGraph->Reachable("Mannan", "Trump");
	bool  c = myGraph->Reachable("Ali", "Trump");
	cout << "are Mannan and Ali connected? " << (a ? "yes" : "no") << endl;
	cout << "are Mannan and Trump connected? " << (b ? "yes" : "no") << endl;
	cout << "are Ali and Trump connected? " << (c ? "yes" : "no") << endl;
	cout << endl;
	
	Graph *unweightedG = new Graph(file_name, true);
	float ali_trump;
	unweightedG->Dijkstra("Ali", "Trump", ali_trump);
	cout << "Ali hops from Trump: " << ali_trump << endl;
	// 2 hops away

	float mannan_trump;
	unweightedG->Dijkstra("Mannan", "Trump", mannan_trump);
	cout << "Mannan hops from Trump: " << mannan_trump << endl;
	//3 hops away

	float ali_trump2;
	myGraph->Dijkstra("Ali", "Trump", ali_trump2);
	cout << "Ali IOF with Trump: " << ali_trump2 << endl;
	//Index of friendship is 12

	float mannan_trump2;
	myGraph->Dijkstra("Mannan", "Trump", mannan_trump2);
	cout << "Mannan IOF with Trump: " << mannan_trump2 << endl;
	//Index of friendship is 8

	cout << endl;

	vector<node*> prism_mst = myGraph->Prims();
	vector<node*> kruskal_mst = myGraph->Kruskal();
	vector<node*> mst_unweighted = unweightedG->Kruskal();

	cout << "-----MST UNWEIGHTED!-----" << endl;
	unweightedG->display(mst_unweighted[0]);
	cout << endl << endl;
	cout << "-----MST WIGHTED KRUSKAL!-----" << endl;
	myGraph->display(kruskal_mst[0]);
	cout << endl << endl;
	cout << "-----MST WIGHTED PRIMS-----" << endl;
	myGraph->display(prism_mst[0]);
	cout << endl;
}

// there can be more than one mst for a graph
// as seen by the different results I got using
// differnt algos (Prims and Kruskal)

#endif
