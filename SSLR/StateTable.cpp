#include "StateTable.h"

StateTable::StateTable()
{
	TableGeneratorSLR t_slr;
	t_slr.GenerateTable();
	t_slr.ShowSlrTable();
	_slrTable = t_slr.GetSlrTable();
	_allGrammarValues = t_slr.GetAllGrammarValues();
}

void StateTable::DefineStateMap()
{
	int idx = 0;
	string stateName = "S";

	for (auto key : _slrTable)
	{
		_stateMap.insert(pair<string, string>(key.first, stateName + to_string(idx)));
		idx++;
	}

	for (auto e : _stateMap)
		cout << e.first << "    : " << e.second << endl;
}

void StateTable::GenerateStateTable()
{
	DefineStateMap();

	for (auto line : _slrTable)
	{
		vector<Cell> stateLine = GenerateEmptyTableLine();
		
		for (size_t i = 0; i < line.second.size(); i++)
		{
			if (line.second[i].second.empty())
			{
				if (_stateMap.find(line.second[i].second) != _stateMap.end())
					stateLine[i].second = _stateMap.at(line.second[i].second);
				else
					stateLine[i].second = line.second[i].second;
			}
		}

		_stateTable.insert(pair<string, vector<Cell>>(line.first, stateLine));
	}
}

vector<Cell> StateTable::GenerateEmptyTableLine()
{
	vector<Cell> etl; //Empty Table Line

	for (size_t i = 0; i < _allGrammarValues.size(); i++)
	{
		Cell cell; //cell - €чейка таблицы 
		cell.first = _allGrammarValues[i]; //»дентификатор €чейки = возможный символ грамматики
		cell.second = {};
		etl.push_back(cell);
	}
	return etl;
}

void StateTable::ShowStateTable()
{
	ofstream fout("stateTable.txt");
	int maxNameSize = 0;
	//инициализируем список размеров €чеек таблицы
	unordered_map<string, int> cellSize;
	for (auto e : _allGrammarValues)
		cellSize.insert(pair<string, int>(e, 0));
	int underLine = 0;

	CalcTablePrams(maxNameSize, cellSize, underLine);
	PrintUnderLine(underLine, fout);

	cout << setw(maxNameSize + INDENT) << " ";
	fout << setw(maxNameSize + INDENT) << " ";

	for (auto elem : _allGrammarValues)
	{
		cout << "|" << setw(cellSize.at(elem) + INDENT_WITH_BORDER) << left << elem;
		fout << "|" << setw(cellSize.at(elem) + INDENT_WITH_BORDER) << left << elem;
	}

	for (auto line : _stateTable)
	{
		cout << endl;
		fout << endl;

		//вычисл€ем максимальную длину названи€ строки
		PrintUnderLine(underLine, fout);
		cout << setw(maxNameSize + INDENT) << left;
		cout << line.first;

		fout << setw(maxNameSize + INDENT) << left;
		fout << line.first;

		for (auto cell : line.second)
		{
			string value = "";
			if (!cell.second.empty())
				value = cell.second;
			cout << "|" << setw(cellSize.at(cell.first) + INDENT_WITH_BORDER) << left << value;
			fout << "|" << setw(cellSize.at(cell.first) + INDENT_WITH_BORDER) << left << value;
		}
	}
	cout << endl;
	fout << endl;
}

void StateTable::CalcTablePrams(int& maxNameSize, unordered_map<string, int>& cellSize, int& underLine)
{
	for (auto line : _slrTable)
	{
		//вычисл€ем максимальную длину названи€ строки
		if (line.first.size() > maxNameSize)
			maxNameSize = line.first.size();

		for (auto cell : line.second)
		{
			int cSize = 0;
			cSize = cell.second.size();

			if (cellSize.at(cell.first) < cSize)
				cellSize.at(cell.first) = cSize;
		}
	}

	underLine += (maxNameSize + INDENT);

	for (auto elem : cellSize)
		underLine += (elem.second + INDENT);
}

void StateTable::PrintUnderLine(int& underLine, ofstream& fout)
{
	for (size_t i = 0; i < underLine; i++)
	{
		cout << "-";
		fout << "-";
	}
	cout << endl;
	fout << endl;
}