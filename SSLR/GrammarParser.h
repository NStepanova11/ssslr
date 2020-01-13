#pragma once
#include "GrammarReader.h"

class GrammarParser
{
private: 
	Rules _rules;
	vector<string> _uniqueValues;
	vector<vector<string>> _ruleStrings;
	void GetValues();

public:
	GrammarParser();
	Rules GetRules();
	vector<string> GetUniqueValues();
	vector<vector<string>> GetRuleStrings();
	void ShowRules();
	void ShowRuleStrings();
	void ShowValues();
};

