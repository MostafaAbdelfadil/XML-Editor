#pragma once
#include "node.h"
#include <fstream>
using namespace std;

class Tree
{
	node* root;
	ofstream out;

public:
	Tree() { root = NULL; }
	Tree(string FileLoc) { root = NULL; out.open(FileLoc); }
	~Tree() { out.close(); }
	bool Empty() { return (root == NULL); }
	node* get_root() { return root; }
	vector<vector<node*>> similar(vector<node*>);
	void insert(string);
	void print_attributes(node*);
	void print_minify(node*);
	void print_Json(node*);
};
