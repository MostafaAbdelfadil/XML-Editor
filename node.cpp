#include "node.h"

node::node(string t)
{
	name = t;
	JsonName = '"' + name.substr(1, name.length() - 2) + '"';
	JsonMark = false;
	level = 0;
	parent = NULL;
}

void node::set_attributes(vector<string> toks)
{
	for (int i = 1; i < toks.size(); i++)
	{
		attribute tr;
		int equal = toks[i].find('=');
		tr.name = '"' + toks[i].substr(0, equal) + '"';
		tr.value = toks[i].substr(equal + 1);
		attrs.push_back(tr);
	}
}

