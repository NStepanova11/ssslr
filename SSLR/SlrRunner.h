#pragma once
#include "StateTable.h"

class SlrRunner
{
private:
	Rules _rules;
	unordered_map<string, vector<Cell>> _stateTable;
	vector<string> _lexemStack;
	vector<string> _actionStack;

	void GetRuleNum(string value, int& ruleNum);
	void AddToStacks(string lexem, string state);
	bool PerformReduce(int ruleNum, string& data);
	void PrintStatus(string data);
public:
	SlrRunner();
	bool ProcessData(string data);

};

