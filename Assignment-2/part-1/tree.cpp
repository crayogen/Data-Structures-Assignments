#ifndef __TREE_CPP
#define __TREE_CPP
#include "tree.h"
#include <math.h>
#include "queue.cpp"

// try to make get_node() and get_file() after tree initialization


Tree::Tree(string data)
{
	root = new TreeItem();
	root->id = 1;
	// write the rest of the code here
	root->container.push_back(VCS_node(data, 0));
	root->latest_data = data;
}

void deleteTreeItem(TreeItem* node) {
	if (node) {
		node->container.clear();
		vector <TreeItem*> child = node->child;
		for (int i=0; i<child.size(); i++) {
			deleteTreeItem(child[i]);
		}
		child.clear();
		delete node;
	}
}

Tree::~Tree()
{
	history.clear();
	TreeItem* node = root;
	deleteTreeItem(root);
}

void Tree::branch(int id)
{
	TreeItem* nodeToBranchFrom = get_node(id);
	if (nodeToBranchFrom) {
		vector <TreeItem*> *child = &nodeToBranchFrom->child;
		string latest_data = nodeToBranchFrom->latest_data;
		int new_id = id*10+child->size()+1;
		TreeItem *branch = new TreeItem(new_id, latest_data);
		branch->latest_data = latest_data;
		branch->container.push_back(VCS_node(latest_data, 0));
		child->push_back(branch);
	}	
}

void Tree::merge(int src_id,int dst_id)
{
	TreeItem* dst_node = get_node(dst_id);
	TreeItem* src_node = get_node(src_id);
	if (src_node && dst_node) {
		history.push_back({src_id, dst_id});
		vector <VCS_node> *container = &dst_node->container;
		if (container->size() == 0)
			return;
		string DataToMerge = src_node->latest_data;
		container->back().data = 
			dst_node->latest_data += DataToMerge;
	}
}

void Tree::commit(int src_id, string new_data)
{
	TreeItem* nodeToCommitTo = get_node(src_id);
	if (nodeToCommitTo) {
		int t_stamp = (nodeToCommitTo->container).size();
		string latest_data = 
			nodeToCommitTo->latest_data += new_data;
		nodeToCommitTo->container
			.push_back(VCS_node(latest_data, t_stamp));
	}
}

string Tree::get_file(int id, int t_stamp)
{
	TreeItem *node = get_node(id);
	if (node) {
		vector <VCS_node> myContainer = node->container;
		for (int i=0; i<myContainer.size(); i++) {
			if (myContainer[i].time_stamp == t_stamp){
				return myContainer[i].data;
			}
		}
	}
	return "";
} 

TreeItem* Tree::get_node(int id)
{
	TreeItem* node = root;
	int	id_length = std::to_string(id).length();
	int treeHeight = 1;
	while (node != NULL) {
		if (node->id == id)
			break;
		vector<TreeItem*> vec = node->child;
		int id_digits_to_check = id;
		if (id_length > 2)
			id_digits_to_check = id / int (pow(10, id_length-++treeHeight));
		node = NULL;
		for (int i=0; i<vec.size(); i++) {
			if (vec[i]->id == id_digits_to_check) {
				node = vec[i];
				break;
			}
		}
	}
	return node;
} 

vector<merge_state> Tree::merge_history()
{
	return history;
}

void Tree::level_traversal()
{
	Queue<TreeItem*> Q; Q = Queue<TreeItem*>();
	Q.enqueue(root);
	while (!Q.isEmpty()) {
		TreeItem *node = Q.dequeue();
		cout << node->id << ", ";
		vector<TreeItem*> child = node->child;
		for (int i=0; i<child.size(); i++) {
			Q.enqueue(child[i]);
		}
	}
}


#endif
