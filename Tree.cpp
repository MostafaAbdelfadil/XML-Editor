#include "Tree.h"
#include <stack>
#include "Functions.h"
#include "Spaces.h"

string first_tag = "#text";
string text_tag = '"' + first_tag + '"';


vector<vector<node*>> Tree::similar(vector<node*> same)
{
	vector<vector<node*>> a;
	for (int i = 0; i < same.size(); i++)
	{
		vector<node*> temp;
		if (!same[i]->get_Mark())
		{
			temp.push_back(same[i]);
			same[i]->set_Mark(true);
		}
		for (int j = i + 1; j < same.size(); j++)
		{
			if ((same[i]->get_name() == same[j]->get_name()) && (!same[j]->get_Mark()))
			{
				temp.push_back(same[j]);
				same[j]->set_Mark(true);
			}
		}
		if (temp.size() > 0)
			a.push_back(temp);
	}
	for (int i = 0; i < same.size(); i++)
		same[i]->set_Mark(false);
	return a;
}

void Tree::print_attributes(node* n)
{
	for (int i = 0; i < n->get_attributes().size(); i++) //printing attributes
	{
		out << space(n->get_attributes()[i].level + 1) << n->get_attributes()[i].name
			<< ": " << n->get_attributes()[i].value << "," << endl;
	}
}

void Tree::print_minify(node* rootptr)
{
	if (rootptr->get_tag()[0] == '<')
	{
		out << rootptr->get_tag();
		int size = rootptr->get_childs().size();
		for (int i = 0; i < size; i++)
			print_minify(rootptr->get_childs()[i]);

		out << "</" + rootptr->get_name().substr(1);
	}
	else
		out << rootptr->get_tag();
}