#include "SlrRunner.h"

int main()
{
	ifstream fin(INPUT_FILE);
	string data;
	SlrRunner slr;
	while (getline(fin, data))
	{
		slr.ProcessData(data);
	}
	
	system("pause");
	return 0;
}