#include "StateTable.h"

int main()
{
	StateTable st;
	st.DefineStateMap();
	st.GenerateStateTable();
	st.ShowStateTable();

	system("pause");
	return 0;
}