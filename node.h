#pragma once
#include <string>
#include <vector>
using namespace std;

//attribute
struct attribute
{
	string name;
	string value;
	int level;
};

class node
{
	string tag;   //tag with attributes
	string name;  //tag without attributes
	string JsonName;
	bool JsonMark;
	vector<attribute> attrs;
	int level;
	node* parent;
	vector<node*> childs;
public:
	node() { level = 0; JsonMark = false; parent = NULL; }
	node(string);
	void set_tag(string g) { tag = g; }
	string get_tag() { return tag; }
	void set_name(string n) { name = n; JsonName = '"' + name.substr(1, name.length() - 2) + '"'; }
	string get_name() { return name; }
	void set_JsonName(string j) { JsonName = '"' + j + '"'; }
	string get_JsonName() { return JsonName; }
	void set_Mark(bool m) { JsonMark = m; }
	bool get_Mark() { return JsonMark; }
	void set_attributes(vector<string>);
	vector<attribute> get_attributes() { return attrs; }
	void set_lvl(int l) { level = l; if (attrs.size() > 0) { for (int i = 0; i < attrs.size(); i++) { attrs[i].level = l + 1; } } }
	int get_lvl() { return level; }
	void set_parent(node* p) { parent = p; }
	node* get_parent() { return parent; }
	void set_child(node* ch) { childs.push_back(ch); childs.back()->set_parent(this); }
	vector<node*> get_childs() { return childs; }
};

