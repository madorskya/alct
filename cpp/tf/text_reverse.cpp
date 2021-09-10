#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main ()
{
	ifstream is("patterns.txt");
	string str;
	while (!is.eof())
	{
		is >> str;
		int ln = str.length();
		for (int i = ln-1;  i >= 0; i--) cout << str[i];
		cout << endl;
	}	

	is.close();
}
