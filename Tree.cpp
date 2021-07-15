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

void Tree::insert(string FileLoc)
{
	ifstream in(FileLoc);
	stack<string> tags;
	stack<int> lvls;
	node* p = NULL;

	while (!in.eof())
	{
		string t;
		getline(in, t);
		int n = FindFirstAngle(t);

		t = t.substr(n);
		vector<string> toks;
		if (t[0] == '<') //not text
			toks = tokens(t);

		if (Empty())
		{
			root = new node(toks[0]);
			p = root;
			root->set_tag(t);
			p->set_attributes(toks);
			tags.push(toks[0]);
			lvls.push(0);
			root->set_lvl(0); continue;
		}

		if (t[0] != '<') //text
		{
			node* ch = new node();
			ch->set_tag(t);
			ch->set_JsonName(t);
			p->set_child(ch);
			ch->set_lvl(ch->get_parent()->get_lvl() + 1);
		}
		else
		{
			if (t[1] == '/') //closing tag
			{
				tags.pop(); lvls.pop();
				p = p->get_parent();
			}
			else //openning tag
			{
				if (Num_Of_Angle(t) == 1) //several lines tag
				{
					tags.push(toks[0]);
					lvls.push(lvls.top() + 1);
					node* ch = new node(toks[0]);
					p->set_child(ch);
					p = ch;
					p->set_tag(t);
					p->set_attributes(toks);
					p->set_lvl(lvls.top());
				}
				else if (Num_Of_Angle(t) > 1) //same line tag
				{
					int index = t.find('>');
					int index1 = t.rfind('<');
					tags.push(toks[0]);
					lvls.push(lvls.top() + 1);
					node* ch = new node(toks[0]);
					p->set_child(ch);
					p = ch;
					p->set_tag(t.substr(0, index + 1));
					p->set_attributes(toks);
					p->set_lvl(lvls.top());

					//text in same line tag
					node* text = new node();
					text->set_tag(t.substr(index + 1, index1 - index - 1));
					text->set_JsonName(text->get_tag());
					p->set_child(text);
					text->set_lvl(text->get_parent()->get_lvl() + 1);
					tags.pop(); lvls.pop();
					p = p->get_parent();
				}
			}
		}

	}
	in.close();
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