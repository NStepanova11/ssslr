#include "SlrRunner.h"

int main()
{
	ifstream fin(INPUT_FILE);
	string data;
	SlrRunner slr;
	while (getline(fin, data))
	{
		if (data[data.size()-1]!='$')
			data += FINAL_CHAR;

		cout << setw(15) << left << data << " : " << boolalpha << slr.ProcessData(data) << endl;
	}
	
	system("pause");
	return 0;
}