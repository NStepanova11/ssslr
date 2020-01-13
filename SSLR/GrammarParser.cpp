#include "GrammarParser.h"

GrammarParser::GrammarParser()
{
	GrammarReader gr;
	_rules = gr.GetRules();
	ShowRules();

	_ruleStrings = gr.GetRuleStrings();

	GetValues();
}

Rules GrammarParser::GetRules()
{
	return _rules;
}

vector<string> GrammarParser::GetUniqueValues()
{
	return _uniqueValues;
}

vector<vector<string>> GrammarParser::GetRuleStrings()
{
	return _ruleStrings;
}

void GrammarParser::GetValues()
{
	for (size_t str = 0; str < _ruleStrings.size(); str++)
	{
		for (size_t el = 0; el < _ruleStrings[str].size(); el++)
		{
			if (find(_uniqueValues.begin(), _uniqueValues.end(), _ruleStrings[str][el]) == _uniqueValues.end())
			{
				_uniqueValues.push_back(_ruleStrings[str][el]);
			}
		}
	}

	if (find(_uniqueValues.begin(), _uniqueValues.end(), FINAL_CHAR) == _uniqueValues.end())
		_uniqueValues.push_back(FINAL_CHAR);
}


void GrammarParser::ShowRules()
{
	cout << "\n----- RULES -----" << endl;
	ofstream fout("parsed_grammar.txt");

	for (int i = 0; i < _rules.leftParts.size(); i++)
	{
		cout << _rules.leftParts[i] << " -> ";
		fout << _rules.leftParts[i] << " -> ";
		int n = 0;
		for (auto lexem : _rules.rightParts[i])
		{
			cout << " " << lexem;
			fout << " " << lexem;

		}
		cout << endl;
		fout << endl;
	}
}

void GrammarParser::ShowRuleStrings()
{
	cout << "RULE STRINGS" << endl;
	for (auto rs : _ruleStrings)
	{
		for (auto e : rs)
			cout << e << " ";
		cout << endl;
	}
}

void GrammarParser::ShowValues()
{
	cout << "\n All values of grammar: ";
	for (size_t i = 0; i < _uniqueValues.size(); i++)
	{
		cout << _uniqueValues[i] << " ";
	}
	cout << endl;
}
