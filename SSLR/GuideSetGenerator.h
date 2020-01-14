#pragma once
#include "Header.h"
#include "GrammarReader.h"

class GuideSetGenerator
{
private:
	Rules _rules;
	vector<vector<string>> _firsts;
	vector<bool> _fstStatus;
	vector<vector<string>> _follows;
	vector<bool> _flwStatus;
	vector<vector<string>> _predicts;

	vector<string> _uniqueHeads;
	unordered_map <string, bool> _haveEmptyAlternative;

	//First
	void FindFirst(); 
	void FindFirstPlus();

	//Follow
	void FindFollow();
	void FindFollowPlus();

	//служебные функции
	void InitializeContainers(); // инициализирует пустые first follow predict fstStatus flwStatus
	bool IsTerminal(string firstToken); //проверяет, является ли символ терминалом
	void CheckEmptyAlternaties(); // определяет, есть ли у какого либо правила пустая альтернатива
	void AddElementToSet(vector<string> & guideSet, string& element);//если элемента нет в множестве, то добавить
	int ContainNoTerminal(vector<string>& _guideSet);
	void AddFirstFrom(string noTerm, int idxOfFst);
	void AddFollowFromFirst(int l, int r, int next);
	bool FirstContainEmpty();
	void ShowSet(string header, vector<vector<string>>& _set);

public:
	GuideSetGenerator();
	Rules GetRules();
	vector<vector<string>> GetPredict();
	vector<vector<string>> GetFirst();
	vector<vector<string>> GetFollow();
};

