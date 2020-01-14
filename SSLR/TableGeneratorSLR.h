#pragma once
#include "GrammarParser.h"
#include "GuideSetGenerator.h";

typedef pair <string, string> Cell;

class TableGeneratorSLR
{
private:
	Rules _rules;
	vector<string> _allGrammarValues;
	vector<vector<string>> _ruleStrings;
	unordered_map<string, vector<Cell>> _slrTable;
	vector<string> _ntTurn;
	vector<string> _turnToNewLine;
	vector<vector<string>> _firsts;
	vector<vector<string>> _follows;

	vector<Cell> GenerateEmptyTableLine();

	void GenerateFirstLine();
	void AddValueToCell(vector<Cell> &tblLine, string &cellKey, string &value);
	void GenerateNextLines();

	void ParseParams(int& ruleNum, int& posInRule, string& value, string elem);
	bool IsTerminal(string firstToken);
	void AddToNtTurn(string noTerminal);
	void UpdateTurnToNewLine(vector<Cell> tblLine);
	void AddInTurnToNewLine(string value);
	void UpdateNtTurn(string valuesList);
	void CalcTablePrams(int& maxNameSize, unordered_map<string, int>& cellSize, int& underLine);
	void PrintUnderLine(int& underLine, ofstream& fout);
	void ShowTurnToNewLine();
	void AddInTableToReduce(vector<Cell>& tblLine, int& ruleNum, int& posInRule);
	void AddInTableNext(vector<Cell>& tblLine, int& ruleNum, int& posInRule);
	void GetValuesFromNtRurn(vector<Cell>& tblLine);
	void ShowTblLine(string name, vector<Cell> line);

public:
	TableGeneratorSLR();
	void GenerateTable();
	void ShowSlrTable();
	unordered_map<string, vector<Cell>> GetSlrTable();
	vector<string> GetAllGrammarValues();
};

