#include "Spaces.h"
using namespace std;


//white spaces
string space(int n)
{
	string space = "    ", st;
	for (int i = 0; i < n; i++)
	{
		st += space;
	}
	return st;
}


//white spaces For Consistency
string space1(int n)
{
	string space = " ", st;
	for (int i = 0; i < n; i++)
	{
		st += space;
	}
	return st;
}