#include "TableGeneratorSLR.h"

vector<Cell> TableGeneratorSLR::GenerateEmptyTableLine()
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

void TableGeneratorSLR::GenerateFirstLine()
{
	vector<Cell> fstLine = GenerateEmptyTableLine();
	string startSymbol = _ruleStrings[0][0]; //Ѕерем первое правило

	int turnIdx = -1;

	while (true)
	{		
		for (size_t line = 0; line < _ruleStrings.size(); line++)
		{
			int fst = 1;
			if (_ruleStrings[line][0] == startSymbol)
			{
				for (auto cell : fstLine)
				{
					if (cell.first == _ruleStrings[line][fst])
					{
						string value = _ruleStrings[line][fst] + DEL + to_string(line) + DEL + to_string(fst);
						AddValueToCell(fstLine, cell.first, value);

						if (!IsTerminal(_ruleStrings[line][fst]))
							AddToNtTurn(_ruleStrings[line][fst]);
						break;
					}
				}
			}
		}

		if (!_ntTurn.empty())
		{
			int s = _ntTurn.size() - 1;
			if (turnIdx < s)
			{
				turnIdx++;
				startSymbol = _ntTurn[turnIdx];
			}
			else
				break;
		}
		else
			break;
		
	}


	for (Cell cell : fstLine)
	{
		
		if (cell.first == _ruleStrings[0][0])
		{
			string value = "OK";
			AddValueToCell(fstLine, cell.first, value);
			break;
		}
	}
	_slrTable.insert(pair<string, vector<Cell>>("Start", fstLine));
	UpdateTurnToNewLine(fstLine);
	_ntTurn.clear();
}

void TableGeneratorSLR::GenerateNextLines()
{
	while (!_turnToNewLine.empty())
	{
		vector<Cell> tblLine = GenerateEmptyTableLine();
		string newLineName = _turnToNewLine[0];
		_turnToNewLine.erase(_turnToNewLine.begin());
		
		if (_slrTable.find(newLineName) == _slrTable.end())//если в таблице не найдена строка дл€ данного символа
		{
			int ruleNum, posInRule;	string value;
			stringstream ss; ss << newLineName; string elem;
			while (ss >> elem)
			{
				ParseParams(ruleNum, posInRule, value, elem);
				if (posInRule == _ruleStrings[ruleNum].size() - 1) // если символ €вл€етс€ последним в строке то свертка
					AddInTableToReduce(tblLine, ruleNum, posInRule);

				else
				{
					int posOfNext = posInRule + 1;
					AddInTableNext(tblLine, ruleNum, posOfNext);

					if (!IsTerminal(_ruleStrings[ruleNum][posOfNext])) // && _cellStatus.at(_ruleStrings[ruleNum][posOfNext]) == false) //если он €вл€етс€ нетерминалом, то внести его в очередь на раскрытие
						AddToNtTurn(_ruleStrings[ruleNum][posOfNext]);
				}
			}
		}

		GetValuesFromNtRurn(tblLine);
		_slrTable.insert(pair<string, vector<Cell>>(newLineName, tblLine));
		UpdateTurnToNewLine(tblLine);
		_ntTurn.clear();
	}
}

TableGeneratorSLR::TableGeneratorSLR()
{
	GrammarParser grp;
	_rules = grp.GetRules();
	_ruleStrings = grp.GetRuleStrings();
	_allGrammarValues = grp.GetUniqueValues();

	GuideSetGenerator gg;
	_firsts = gg.GetFirst();
	_follows = gg.GetFollow();
}

void TableGeneratorSLR::GenerateTable()
{
	GenerateFirstLine();
	GenerateNextLines();
}

bool TableGeneratorSLR::IsTerminal(string firstToken)
{
	if (find(_rules.leftParts.begin(), _rules.leftParts.end(), firstToken) == _rules.leftParts.end())
		return true;
	else return false;
}

void TableGeneratorSLR::AddToNtTurn(string noTerminal)
{
	if (find(_ntTurn.begin(), _ntTurn.end(), noTerminal) == _ntTurn.end())
		_ntTurn.push_back(noTerminal);
}

void TableGeneratorSLR::AddValueToCell(vector<Cell>& tblLine, string& cellKey, string& value)
{
	stringstream ss;
	string elem;
	bool contain = false;

	for (auto &cell : tblLine)
	{
		if (cell.first == cellKey)
		{
			ss << cell.second;
			while (ss >> elem)
			{
				if (elem == value)
					contain = true;
			}
			if (contain == false)
				cell.second += (value + " ");
		}
	}
}

void TableGeneratorSLR::AddInTableNext(vector<Cell>& tblLine, int& ruleNum, int& posInRule)
{
	string cellKey = _ruleStrings[ruleNum][posInRule];
	string value = cellKey + DEL + to_string(ruleNum) + DEL + to_string(posInRule);

	stringstream ss;
	string elem;
	bool contain = false;

	for (auto& cell : tblLine)
	{
		if (cell.first == cellKey)
		{
			ss << cell.second;
			while (ss >> elem)
			{
				if (elem == value)
					contain = true;
			}
			if (contain == false)
				cell.second += (value + " ");
		}
	}

	//auto it = find(_allGrammarValues.begin(), _allGrammarValues.end(), value);
	//auto pos = distance(_allGrammarValues.begin(), it);
	//tblLine[pos].second += (value + to_string(ruleNum) + to_string(posInRule) + " ");
}

void TableGeneratorSLR::AddInTableToReduce(vector<Cell>& tblLine, int& ruleNum, int& posInRule)
{
	auto it = find(_allGrammarValues.begin(), _allGrammarValues.end(), FINAL_CHAR);
	auto pos = distance(_allGrammarValues.begin(), it);
	tblLine[pos].second += ("R" + to_string(ruleNum) + " ");
}

void TableGeneratorSLR::UpdateTurnToNewLine(vector<Cell> tblLine)
{
	for (Cell &cell : tblLine)
	{
		string lineName = cell.second;
		
		if (!lineName.empty())
		{
			if (lineName != "OK ")
			{
				
				if (find(lineName.begin(), lineName.end(), 'R') == lineName.end())
				{
					if (_slrTable.find(lineName) == _slrTable.end())
						AddInTurnToNewLine(lineName);
				}
				
			}
		}
	}
}

void TableGeneratorSLR::AddInTurnToNewLine(string value)
{
	// очередное значение добавл€етс€ в очередь, если еще нет строки таблицы с таким ключом
	// и в очереди такой ключ тоже не найден

	if (find(_turnToNewLine.begin(), _turnToNewLine.end(), value) == _turnToNewLine.end())
		_turnToNewLine.push_back(value);

}

void TableGeneratorSLR::UpdateNtTurn(string valuesList)
{
	stringstream ss;
	string element, value; 
	int ruleNum, posInRule;
	
	ss << valuesList;
	while (ss>> element)
	{
		ParseParams(ruleNum, posInRule, value, element);
		if (!IsTerminal(value))
			AddToNtTurn(value);
	}
}

void TableGeneratorSLR::ParseParams(int& ruleNum, int& posInRule, string& value, string elem)
{
	for (auto &e : elem)
		if (e == DEL)
			e = ' ';

	stringstream ss;
	ss << elem;

	ss >> value;
	ss >> ruleNum;
	ss >> posInRule;
}

void TableGeneratorSLR::ShowSlrTable()
{
	ofstream fout("slrTable.txt");
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

	for (auto line : _slrTable)
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

unordered_map<string, vector<Cell>> TableGeneratorSLR::GetSlrTable()
{
	return _slrTable;
}

vector<string> TableGeneratorSLR::GetAllGrammarValues()
{
	return _allGrammarValues;
}

void TableGeneratorSLR::CalcTablePrams(int& maxNameSize, unordered_map<string, int>& cellSize, int& underLine)
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

void TableGeneratorSLR::PrintUnderLine(int& underLine, ofstream& fout)
{
	for (size_t i = 0; i < underLine; i++)
	{
		cout << "-";
		fout << "-";
	}
	cout << endl;
	fout << endl;
}

void TableGeneratorSLR::ShowTurnToNewLine()
{
	cout << "\nturn: ";
	for (auto e : _turnToNewLine)
	{
		cout << e << ", ";
	}
	cout << endl;
}

void TableGeneratorSLR::GetValuesFromNtRurn(vector<Cell>& tblLine)
{
	if (!_ntTurn.empty())
	{
		int turnIdx = -1;
		string startSymbol;

		while (true)
		{
			int s = _ntTurn.size() - 1;
			if (turnIdx < s)
			{
				turnIdx++;
				startSymbol = _ntTurn[turnIdx];


				for (size_t line = 0; line < _ruleStrings.size(); line++)
				{
					int fst = 1;
					if (_ruleStrings[line][0] == startSymbol)
					{
						for (auto cell : tblLine)
						{
							if (cell.first == _ruleStrings[line][fst])
							{
								string value = _ruleStrings[line][fst] + DEL + to_string(line) + DEL + to_string(fst);
								AddValueToCell(tblLine, cell.first, value);

								if (!IsTerminal(_ruleStrings[line][fst]))
									AddToNtTurn(_ruleStrings[line][fst]);
								break;
							}
						}
					}
				}
			}
			else
				break;
		}
	}
}

void TableGeneratorSLR::ShowTblLine(string name, vector<Cell> line)
{
	cout << name << " : ";
	for (auto cell : line)
	{
		cout << setw(10) << left << cell.second;
	}
	cout << endl;
}
