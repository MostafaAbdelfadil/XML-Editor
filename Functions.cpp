#include <string>
#include "Functions.h"
using namespace std;

int FindFirstAngle(string s)
{
	for (int j = 0; j < s.length(); j++)
	{
		if (s[j] != ' ')
			return j;
	}
}


int Num_Of_Angle(string s)
{
	int count = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '<')
			count++;
	}
	return count;
}


vector<string> tokens(string s)
{
	vector<string> a;
	vector<int> spaces;
	int index = s.find('>');
	int count = 0;
	for (int i = 0; i < index + 1; i++)
	{
		if (s[i] == '"')
		{
			count++;
			if (count == 2)
				count = 0;
		}
		if (s[i] == ' ' && count == 0)
			spaces.push_back(i);
	}
	if (spaces.size() > 0)
	{
		a.push_back((s.substr(0, spaces[0])) + '>');
		for (int i = 0; i < spaces.size(); i++)
		{
			if (i < spaces.size() - 1)
				a.push_back(s.substr(spaces[i] + 1, spaces[i + 1] - spaces[i] - 1));
			else
				a.push_back(s.substr(spaces[i] + 1, index - spaces[i] - 1));
		}
	}
	else
		a.push_back(s.substr(0, index + 1));


	return a;
}

