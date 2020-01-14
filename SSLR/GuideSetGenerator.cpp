#include "GuideSetGenerator.h"

GuideSetGenerator::GuideSetGenerator()
{
	GrammarReader gr;

	_rules = gr.GetRules();
	//gr.ShowGrammar();

	InitializeContainers();
	CheckEmptyAlternaties();
}

Rules GuideSetGenerator::GetRules()
{
	return _rules;
}

vector<vector<string>> GuideSetGenerator::GetPredict()
{
	GetFirst();
	ShowSet("First:", _firsts);

	if (FirstContainEmpty())
	{
		GetFollow();
		ShowSet("Follows:", _follows);

		for (size_t i = 0; i < _firsts.size(); i++)
		{
			if (find(_firsts[i].begin(), _firsts[i].end(), EMPTY_CHAR) != _firsts[i].end())
				_predicts[i] = _follows[i];
			else
				_predicts[i] = _firsts[i];
		}
		ShowSet("Predicts:", _predicts);
		return _predicts;
	}
	return _firsts;
}

vector<vector<string>> GuideSetGenerator::GetFirst()
{
	FindFirst();
	FindFirstPlus();

	ShowSet("First:", _firsts);
	return _firsts;
}

vector<vector<string>> GuideSetGenerator::GetFollow()
{
	FindFollow();
	FindFollowPlus();

	ShowSet("Follows:", _follows);
	return _follows;
}

void GuideSetGenerator::FindFirst()
{
	for (size_t str = 0; str < _rules.leftParts.size(); str++)
	{
		int el = 0;

		while (true)
		{
			//если символ=терминал, то добавляем его если его еще нет во множестве
			if (IsTerminal(_rules.rightParts[str][el]))
			{
				AddElementToSet(_firsts[str], _rules.rightParts[str][el]);
				break;
			}
			else //если символ = нетерминал
			{
				AddElementToSet(_firsts[str], _rules.rightParts[str][el]);

				if (_haveEmptyAlternative.at(_rules.rightParts[str][el])) //Если у НТ есть пустая альтернатива
				{
					if (el < _rules.rightParts[str].size() - 1)
						el++;
					else
						break;
				}
				else
					break;
			}
		}
	}
}

void GuideSetGenerator::FindFirstPlus()
{
	while (find(_fstStatus.begin(), _fstStatus.end(), false) != _fstStatus.end())
	{
		for (size_t start = 0; start < _firsts.size(); start++)
		{
			int pos_in_fst = ContainNoTerminal(_firsts[start]);
			if (pos_in_fst == -1)
				_fstStatus[start] = true;
			else
			{
				int idxOfFst = start; //индекс правила, которое содержит нетерминал в множестве Firsts
				string noTerm = _firsts[start][pos_in_fst]; //запоминаем найденный нетерминал
				AddFirstFrom(noTerm, idxOfFst); //добавляем в firsts правила множества нетерминала
				_firsts[start].erase(_firsts[start].begin() + pos_in_fst); //удаляем нетерминал из множества
			}
		}
	}
}

void GuideSetGenerator::FindFollow()
{
	for (size_t l = 0; l < _rules.leftParts.size(); l++)
	{
		if (l == 0)
			_follows[l].push_back(FINAL_CHAR);

		for (size_t r = 0; r < _rules.rightParts.size(); r++)
		{
			for (size_t e = 0; e < _rules.rightParts[r].size(); e++)
			{
				//если нашли символ для которого нужно найти follow
				if (_rules.rightParts[r][e] == _rules.leftParts[l])
				{
					int next = e + 1;

					while (true)
					{
						if (next < _rules.rightParts[r].size() - 1) //то если существует соседний элемент
						{
							if (!IsTerminal(_rules.rightParts[r][next]))
							{
								AddFollowFromFirst(l, r, next); //добавляет first без пустого символа
								if (_haveEmptyAlternative.at(_rules.rightParts[r][next]))
									next++;
								else
									break;
							}
							else
							{							
								AddElementToSet(_follows[l], _rules.rightParts[r][next]);
								break;
							}
						}
						else if (next == _rules.rightParts[r].size() - 1)
						{
							if (!IsTerminal(_rules.rightParts[r][next]))
							{
								AddFollowFromFirst(l, r, next); //добавляет first без пустого символа
								if (_haveEmptyAlternative.at(_rules.rightParts[r][next]))
								{
									AddElementToSet(_follows[l], _rules.leftParts[r]);
									_flwStatus[l] = false;
								}
								break;
							}
							else
							{
								AddElementToSet(_follows[l], _rules.rightParts[r][next]);
								break;
							}
						}
						else
						{
							AddElementToSet(_follows[l], _rules.leftParts[r]);
							_flwStatus[l] = false;
							break;
						}
					}
				}
			}
		}
	}
}

void GuideSetGenerator::FindFollowPlus()
{
	while (find(_flwStatus.begin(), _flwStatus.end(), false) != _flwStatus.end())
	{
		for (size_t flw = 0; flw < _follows.size(); flw++)
		{
			for (size_t e = 0; e < _follows[flw].size(); e++)
			{
				if (!IsTerminal(_follows[flw][e])) //если в каком то follow найден нетерминал
				{
					for (size_t l = 0; l < _rules.leftParts.size(); l++) // ищем его в левой части
					{
						if (_rules.leftParts[l] == _follows[flw][e])
						{
							for (auto el : _follows[l]) //пушим follow нетерминала в текущий follow
							{
								if (find(_follows[flw].begin(), _follows[flw].end(), el) == _follows[flw].end())
								{
									if (el != _rules.leftParts[flw])
										_follows[flw].push_back(el);
								}
							}
						}
					}
					_follows[flw].erase(_follows[flw].begin() + e);
					e = -1;
				}
			}

			if (ContainNoTerminal(_follows[flw]) == -1)
				_flwStatus[flw] = true;
		}
	}
}


void GuideSetGenerator::AddFollowFromFirst(int l, int r, int next)
{
	for (size_t fst = 0; fst < _rules.leftParts.size(); fst++)
	{
		if (_rules.leftParts[fst] == _rules.rightParts[r][next])
			if (find(_firsts[fst].begin(), _firsts[fst].end(), EMPTY_CHAR) == _firsts[fst].end())
				for (size_t el = 0; el < _firsts[fst].size(); el++)
				{
					if (find(_follows[l].begin(), _follows[l].end(), _firsts[fst][el]) == _follows[l].end())
						_follows[l].push_back(_firsts[fst][el]);
				}
	}
}

void GuideSetGenerator::AddFirstFrom(string noTerm, int idxOfFst)
{
	for (size_t i = 0; i < _rules.leftParts.size(); i++)
	{
		if (_rules.leftParts[i] == noTerm)
		{
			if (_rules.rightParts[i][0] != EMPTY_CHAR)
			{
				for (size_t k = 0; k < _firsts[i].size(); k++)
				{
					AddElementToSet(_firsts[idxOfFst], _firsts[i][k]);
				}
			}
		}
	}
}

void GuideSetGenerator::InitializeContainers()
{
	for (int i = 0; i < _rules.leftParts.size(); i++)
	{
		_firsts.push_back({});
		_fstStatus.push_back(false);

		_follows.push_back({});
		_flwStatus.push_back(true);

		_predicts.push_back({});
	}
}

bool GuideSetGenerator::IsTerminal(string firstToken)
{
	if (find(_rules.leftParts.begin(), _rules.leftParts.end(), firstToken) == _rules.leftParts.end())
		return true;
	else return false;
}

void GuideSetGenerator::CheckEmptyAlternaties()
{
	//собираем все нетерминалы в один список
	for (size_t i = 0; i < _rules.leftParts.size(); i++)
	{
		if (find(_uniqueHeads.begin(), _uniqueHeads.end(), _rules.leftParts[i]) == _uniqueHeads.end())
			_uniqueHeads.push_back(_rules.leftParts[i]);
	}

	//если  у этого нетерминала есть пустая альтернатива, то haveEmpty=true (те A -> # найдено парпямую)
	for (size_t u = 0; u < _uniqueHeads.size(); u++)
	{
		bool haveEmpty = false;
		for (size_t l = 0; l < _rules.leftParts.size(); l++)
		{
			if (_rules.leftParts[l] == _uniqueHeads[u])
				if (find(_rules.rightParts[l].begin(), _rules.rightParts[l].end(), EMPTY_CHAR) != _rules.rightParts[l].end())
				{
					haveEmpty = true;
				}
		}
		_haveEmptyAlternative.insert(pair<string, bool>(_uniqueHeads[u], haveEmpty));
	}
}

void GuideSetGenerator::AddElementToSet(vector<string>& guideSet, string& element)
{
	if (find(guideSet.begin(), guideSet.end(), element) == guideSet.end())
		guideSet.push_back(element);
}

int GuideSetGenerator::ContainNoTerminal(vector<string>& _guideSet)
{
	for (size_t i = 0; i < _guideSet.size(); i++)
		if (find(_rules.leftParts.begin(), _rules.leftParts.end(), _guideSet[i]) != _rules.leftParts.end())
			return i;

	return -1;
}

void GuideSetGenerator::ShowSet(string header, vector<vector<string>>& _set)
{
	cout << endl;
	cout << header << endl;
	for (size_t i = 0; i < _rules.leftParts.size(); i++)
	{
		cout << _rules.leftParts[i] << " : ";
		for (auto s : _set[i])
		{
			cout << s << " ";
		}
		cout << endl;
	}
}

bool GuideSetGenerator::FirstContainEmpty()
{
	for (size_t i = 0; i < _firsts.size(); i++)
	{
		if (find(_firsts[i].begin(), _firsts[i].end(), EMPTY_CHAR) != _firsts[i].end())
			return true;
	}
	return false;
}