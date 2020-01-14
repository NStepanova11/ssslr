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
		
		PrintStatus(data);

		for (auto& cell : tblLine)
		{

			ss << data[0];
			ss >> fst;

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
		for (size_t i = ruleSize; i > 0; i--)
		{
			if (_lexemStack.back() == _rules.rightParts[ruleNum][i])
			{
				_lexemStack.pop_back();
				_actionStack.pop_back();
			}
			else
				return false;
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
