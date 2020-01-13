#pragma once
#include "Header.h"

class GrammarReader
{
private:
	Rules _rules;
	vector<vector<string>> ruleStrings;

	void ReadGrammarFromFile();
	void ReadGrammarStringsFromFile();

public:
	Rules GetRules();
	vector<vector<string>> GetRuleStrings();
};

