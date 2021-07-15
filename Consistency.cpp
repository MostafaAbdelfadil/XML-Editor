#include <iostream>
#include <stack>
#include <string>
#include<queue>
#include <fstream>
#include <vector>

#include "Spaces.h"
#include "Consistency.h"

using namespace std;

//count spaces untill find char (ignoring white spaces)
int spa(string s)
{
	char sp = ' ';
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != sp)
		{
			
				return i;
			
		}
	}
	return -1;
}

//check consistency and correcting if inconsistent
void consistency(string FileLocIn, string FileOutCorrect, string FileOutError)
{
	ofstream out(FileOutCorrect);
	ofstream out2(FileOutError);
	ifstream in(FileLocIn);
	stack<string>s, temp;
	stack<int>ss;
	queue<string>num, error;
	string f, sp;
	int u = 0;
	while (!in.eof())
	{

		getline(in, f);
		if (f == "")
			continue;
		if (spa(f) != 0 && u == 0)
			u = 1;
		if (s.empty())
		{
			num.push(f);
			error.push(f);
			ss.push(spa(f));
			int index1 = f.find(' ');
			if (index1 != -1)
				f = f.substr(1, index1 - 1) + '>';
			else
				f = f.substr(f.find('<') + 1);
			s.push(f);
		}
		else
		{
			if (f[spa(f)] != '<')
			{
				num.push(f);
				error.push(f);
			}
			else {
				int index = f.find('>');
				if (index != f.length() - 1) {
					if ((ss.top() == spa(f) && u == 1))
					{
						num.push(space1(ss.top()) + "</" + s.top());
						error.push(space1(ss.top()) + "         <-------");
						s.pop();
						ss.pop();

					}
					if (u == 0 && num.back()[0] != '<')
					{
						num.push(space1(ss.top()) + "</" + s.top());
						error.push(space1(ss.top()) + "         <-------");
						s.pop();
						ss.pop();
					}
					int ind = f.rfind('>');
					if (ind == f.length() - 1) {
						string d, d1, r;
						r = f.substr(0, spa(f));
						f = f.substr(spa(f));
						int tt = f.find(' ');
						if (tt != -1 && tt < f.find('>')) {
							d = f.substr(f.find('<') + 1, f.find(' ') - f.find('<') - 1);
							d1 = f.substr(f.rfind('<') + 2, f.length() - 3 - f.rfind('<'));
						}
						else
						{
							d = f.substr(f.find('<') + 1, f.find('>') - f.find('<') - 1);
							d1 = f.substr(f.rfind('<') + 2, f.length() - 3 - f.rfind('<'));
						}
						if (d == d1) {
							num.push(r + f);
							error.push(r + f);
						}
						else
						{
							error.push(r + f + "  <--------");

							f = f.substr(f.find('<'), f.rfind('<') - f.find('<'));
							num.push(r + f + "</" + d + ">");
						}
					}
					else
					{
						string r = f.substr(0, spa(f));
						f = f.substr(spa(f));
						int tt = f.find(' ');
						if (tt != -1 && tt < f.find('>'))
							num.push(r + f + "</" + f.substr(f.find('<') + 1, (f.find(' ') - f.find('<') - 1)) + ">");
						else
							num.push(r + f + "</" + f.substr(f.find('<') + 1, (f.find('>') - f.find('<'))));
						error.push(r + f + "  <--------");
					}
				}
				else {
					string co = s.top(), ff;
					sp = f.substr(0, f.find('<'));
					ff = f.substr(f.find('<') + 2);
					if (co == ff)
					{
						ff = sp + "</" + ff;
						num.push(ff);
						error.push(ff);
						s.pop();
						ss.pop();

					}
					else {
						int y = 0, yy = 0;
						if (u == 1 && num.back()[spa(num.back())] != '<')
						{
							string co = s.top(), ff;
							sp = f.substr(0, f.find('<'));
							ff = f.substr(f.find('<') + 2);
							if (co == ff) {
								ff = sp + "</" + ff;
								num.push(ff);
								error.push(ff);
								s.pop();
								ss.pop();
							}
							else {
								num.push(space1(ss.top()) + "</" + s.top());
								error.push(space1(ss.top()) + "         <-------");
								s.pop();
								ss.pop();
							}
						}
						else {

							while (!s.empty() && f.find('/') != -1 && (f.find('/') - f.find('<') == 1))
							{
								string co = s.top(), ff1;
								ff1 = f.substr(f.find('<') + 2);
								if (co == ff1)
								{
									y = 1;
									break;
								}
								y++;
								temp.push(s.top());
								s.pop();

							}


							while (!temp.empty()) {
								s.push(temp.top());
								temp.pop();
							}
							if ((ss.top() >= spa(f) && u == 1) && (f.find('/') == -1 || (f.find('/') - f.find('<') > 1)))
								y = 1;
							if (y > 1)
							{
								f = f.substr(spa(f));
								num.push(space1(ss.top()) + "</" + s.top());
								error.push(space1(ss.top()) + f + "   <---------");
								s.pop();
								ss.pop();
							}
							yy = 0;
							if (((f.find('/') != -1 && (f.find('/') - f.find('<') == 1)) || (ss.top() >= spa(f) && u == 1)) && y == 1)
							{
								yy = 1;
								if (f.find('/') != -1 && (f.find('/') - f.find('<') == 1)) {
									int c = 0;
									vector<string>vect;
									while (c == 0)
									{
										ff = f.substr(f.find('<') + 2);
										if (ff == s.top())
										{
											c = 1;
											num.push(space1(ss.top()) + "</" + s.top());
											error.push(space1(ss.top())+"</" + s.top());
											s.pop();
											ss.pop();


										}
										else
										{
											num.push(space1(ss.top()) + "</" + s.top());
											error.push(space1(ss.top()) + "     <-------");
											s.pop();
											ss.pop();

										}
									}

								}
								else if (ss.top() >= spa(f))
								{

									if (ss.top() > spa(f)) {
										while (ss.top() != spa(f))
										{
											num.push(space1(ss.top()) + "</" + s.top());
											error.push(ss.top() + "         <-------");
											s.pop();
											ss.pop();

										}
									}
									if (ss.top() == spa(f))
									{
										num.push(space1(ss.top()) + "</" + s.top());
										error.push(ss.top() + "         <-------");
										s.pop();
										ss.pop();

									}
									num.push(f);
									error.push(f);
									ss.push(spa(f));
									ff = f.substr(f.find('<') + 1);

									int index1 = ff.find(' ');
									if (index1 != -1)
										f = f.substr(f.find('<') + 1, index1) + '>';
									else
										f = f.substr(f.find('<') + 1);
									s.push(f);

								}

							}
						}
						if ((f.find('/') == -1 || (f.find('/') - f.find('<') > 1)) && yy == 0) {
							num.push(f);
							error.push(f);
							ss.push(spa(f));
							ff = f.substr(f.find('<') + 1);
							int index1 = ff.find(' ');
							if (index1 != -1)
								f = ff.substr(0, index1) + '>';
							else
								f = f.substr(f.find('<') + 1);
							s.push(f);
						}
					}
				}
			}

		}


	}
	if (!s.empty())
	{
		while (!s.empty())
		{
			num.push(space1(ss.top()) + "</" + s.top());
			error.push(ss.top() + "         <-------");
			s.pop();
			ss.pop();

		}
	}
	int size = num.size();
	for (int i = 0; i < size; i++)
	{
		out << num.front() << endl;
		out2 << error.front() << endl;
		num.pop();
		error.pop();
	}


}
