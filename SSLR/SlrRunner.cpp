#include "SlrRunner.h"

SlrRunner::SlrRunner()
{
	StateTable st;
	st.GenerateStateTable();
	st.ShowStateTable();

	_rules = st.GetRules();
	_stateTable = st.GetStates();
}

bool SlrRunner::ProcessData(string data)
{
	stringstream ss;
	string fst;
	int ruleNum = 0;

	string startState = "S0";
	_actionStack.push_back(startState);

	vector<Cell> tblLine;

	while (true)
	{
		string currentState = _actionStack.back();
		tblLine = _stateTable.at(currentState);
		
		//PrintStatus(data);

		fst.clear();
		fst += data[0];

		for (auto& cell : tblLine)
		{
			if (cell.first == fst)
			{
				if (cell.second.empty()) //если €чейка пуста€
				{
					return false;
				}
				else if (cell.second[0] == 'S')
				{
					AddToStacks(fst, cell.second);
					data.erase(data.begin());
					break;
				}
				else if (cell.second[0] == 'R')
				{
					GetRuleNum(cell.second, ruleNum);
					if (!PerformReduce(ruleNum, data))
						return false;
					else break;
				}
				else if (cell.second == "OK ")
				{
					if (_actionStack.back()==startState && data==cell.first+FINAL_CHAR && _lexemStack.empty())
						return true;
					else
						return false;
				}
			}
		}
	}
	return false;
}

void SlrRunner::GetRuleNum(string value, int& ruleNum)
{
	value.erase(value.begin());
	stringstream sn;
	sn << value;
	sn >> ruleNum;
}

void SlrRunner::AddToStacks(string lexem, string state)
{
	_actionStack.push_back(state);
	_lexemStack.push_back(lexem);
}

bool SlrRunner::PerformReduce(int ruleNum, string &data)
{
	int ruleSize = _rules.rightParts[ruleNum].size();
	if (_lexemStack.size() < ruleSize)
		return false;
	else
	{
		for (int i = ruleSize-1; i >= 0; i--)
		{
			if (i >= 0)
			{
				if (_lexemStack.back() == _rules.rightParts[ruleNum][i])
				{
					_lexemStack.pop_back();
					_actionStack.pop_back();
				}
				else
					return false;
			}
		}
		data = _rules.leftParts[ruleNum] + data;
	}
}

void SlrRunner::PrintStatus(string data)
{
	cout << "------------------------------------" << endl;
	cout << "data:   " << data << endl;

	cout << "lexem stack:   ";
	for (auto elem : _lexemStack)
		cout << elem << " ";
	cout << endl;

	cout << "action stack:   ";
	for (auto elem : _actionStack)
		cout << elem << " ";
	cout << endl;
	cout << "------------------------------------" << endl;
}
