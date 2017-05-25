#include "parse.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

int main()
{
	string expr;
	while (cin >> expr)
	{
		Parser parser(expr);
		if (!parser.err_set())
		{
			try
			{
				cout << parser.go() << endl;
			}
			catch (std::exception const &_E)
			{
				parser.log_err(_E);
			}
		}
	}
	system("pause");
	return 0;
}