#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <iterator>

using namespace std;

const string INPUT_FILE = "1_input.txt";
const string GRAMMAR_FILE = "grammar.txt";
const int START_RULE = 1;
const string EMPTY_CHAR = "#";
const string FINAL_CHAR = "$";
const int INDENT = 4;
const int INDENT_WITH_BORDER = 3;
const char DEL = '-';

enum CharType
{
	LEFT_NO_TERM=11,
	RIGHT_NO_TERM=1,
	TERMINAL=4,
	EMPTY=0
};

struct Rules {
	string startNoTerminal;
	int idxOfStartNoTerminal;
	vector<string> leftParts;
	vector<vector<string>> rightParts;
};

struct ParseLine {
	int num;
	string value;
	vector<string> guideSet;
	bool shift;
	int go_to;
	bool to_stack;
	bool err;
	bool final;
	int rule_num;
	int next_alt;
	CharType char_type;
};

struct LL {
	string value;
	vector<string>set;
	int go_to;
	bool is_left;
	int rule_num;
	CharType char_type;
};

