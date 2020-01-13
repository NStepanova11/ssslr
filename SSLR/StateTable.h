#pragma once
#include "TableGeneratorSLR.h"

class StateTable
{
private:
	unordered_map<string, vector<Cell>> _slrTable;
	unordered_map<string, string> _stateMap;
	unordered_map<string, vector<Cell>> _stateTable;
	vector<string> _allGrammarValues;
	vector<Cell> GenerateEmptyTableLine();

	void CalcTablePrams(int& maxNameSize, unordered_map<string, int>& cellSize, int& underLine);
	void PrintUnderLine(int& underLine, ofstream& fout);
public:
	StateTable();
	void DefineStateMap();
	void GenerateStateTable();
	void ShowStateTable();
};

