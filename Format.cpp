#include <string>
#include <fstream>
#include <stack>
#include <queue>
#include "Spaces.h"
#include "Consistency.h"

using namespace std;

//Prettifying the file

void format(string FileLocIn, string FileLocOut) {
	ofstream out(FileLocOut);
	ifstream in(FileLocIn);
	stack<string>s;
	queue<string>num;
	string f;
	int nu = 0;
	while (!in.eof())
	{
		getline(in, f);
		int open_tag_second = f.find('<') + 1;
		if (s.empty())
		{
			f = f.substr(spa(f));
			num.push(f);
			//f = f.substr(1);
			int index1 = f.find(' ');
			if (index1 != -1)
				f = f.substr(1, index1 - 1) + '>';
			else
				f = f.substr(1);
			s.push(f);
		}
		else
		{
			if (f[spa(f)] != '<')
			{
				f = f.substr(spa(f));
				num.push(space(nu + 1) + f);
			}
			else {
				int index = f.find('>');
				if (index != f.length() - 1) {
					f = f.substr(spa(f));
					num.push(space(nu + 1) + f);
				}
				else {
					string co = s.top(), ff;
					ff = f.substr(open_tag_second+1);
					if (co == ff&&f.find('/')!=-1)
					{

						ff = space(nu) + "</" + ff;
						num.push(ff);
						s.pop();
						nu--;
					}
					else {
						nu++;
						f = f.substr(spa(f));
						num.push(space(nu) + f);
						int index1 = f.find(' ');
						if (index1 != -1)
							f = f.substr(1, index1 - 1) + '>';
						else
							f = f.substr(1);
						s.push(f);
					}
				}
			}

		}


	}
	int size = num.size();
	for (int i = 0; i < size; i++)
	{
		out << num.front() << endl;
		num.pop();
	}


}
