#include "GrammarReader.h"


void GrammarReader::ReadGrammarFromFile()
{
	ifstream fin(GRAMMAR_FILE);
	string line;
	stringstream ss;
	int lineNum = 0;

	while (getline(fin, line))
	{
		lineNum++; //индекс прочитанной строки

		ss << line;
		vector<string> ruleBody = {};
		string lexem = "";

		string leftNonTerminal;
		ss >> leftNonTerminal;

		//стартовый нетерминал должен находиться в первой строке в файле
		if (lineNum == START_RULE)
			_rules.startNoTerminal = leftNonTerminal;

		_rules.leftParts.push_back(leftNonTerminal);
		_rules.idxOfStartNoTerminal = _rules.leftParts.size() - 1;

		while (ss >> lexem)
		{
			if (lexem == "->")
				continue;
			else if (lexem == "|")
			{
				_rules.leftParts.push_back(leftNonTerminal);
				_rules.rightParts.push_back(ruleBody);
				ruleBody.clear();
			}
			else
				ruleBody.push_back(lexem);
		}
		_rules.rightParts.push_back(ruleBody); // тк. последнее правило формируется но после последней лексемы выходит из цикла
		ss.clear();
	}
}

void GrammarReader::ReadGrammarStringsFromFile()
{
	ifstream fin(GRAMMAR_FILE);
	string line;
	stringstream ss;
	int lineNum = 0;

	while (getline(fin, line))
	{
		ss << line;
		vector<string> ruleBody = {};
		string lexem = "";
		string ruleString;

		string leftNonTerminal;
		ss >> leftNonTerminal;
		ruleBody.push_back(leftNonTerminal);

		while (ss >> lexem)
		{
			if (lexem == "->")
				continue;
			else if (lexem == "|")
			{
				ruleStrings.push_back(ruleBody);
				ruleBody.clear();
				ruleBody.push_back(leftNonTerminal);
			}
			else
				ruleBody.push_back(lexem);
		}
		ruleStrings.push_back(ruleBody); // тк. последнее правило формируется но после последней лексемы выходит из цикла
		ss.clear();
	}
}

vector<vector<string>> GrammarReader::GetRuleStrings()
{
	ReadGrammarStringsFromFile();
	return ruleStrings;
}

Rules GrammarReader::GetRules()
{
	ReadGrammarFromFile();
	return _rules;
}