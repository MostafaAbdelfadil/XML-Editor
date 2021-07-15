#include "Tree.h"
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
