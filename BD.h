#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<filesystem>
#include"json.hpp"
#include"DL.h"
#include"dict.h"
#include"Tools.h"
#include "Tree.h"

using namespace std;

using json = nlohmann::json;
using eqls = DL<Pair<Pair<string,string>, Pair<string,string>>>;
using eql = Pair<Pair<string,string>, Pair<string,string>>;
using print = dict<dict<DL<string>>>;

struct DataOfTable
{
	string name;
	string path;
	DL<string> columns;
	int quantityColumns;
	DataOfTable() {};
	DataOfTable(string name, string path, DL<string> columns) : name(name), path(path), columns(columns)
	{ quantityColumns = columns.len; }
	string& operator[](int& index)
	{
		return columns[index];
	}
};

struct BD
{
	int tuple_limits;
	dict<DataOfTable> structure;
	string nameSheme;

	BD(string schema);
	void pk_sequence(string path, string);
	void lock(string path, string);
	void parsSheme(string schema);
	string separateCommand(DL<string>& command);
	string INSERT(DL<string>& command);
	DL<string> parsForInsert(string values);
	bool rqstForIntervention(string tableName);
	string GetNumberOfLastFile(string tableName);
	void CreateNewCSVFile(string path);
	void ForbidIntervention(string tableName);
	void allowIntervention(string tableName);
	void PlusOneSequence(string tableName);

	string SELECT(DL<string>& command);

	Tree WHERE(DL<string>& command, DL<Pair<string,string>>& Lexems, DL<Pair<string,string>>& allLexemFromWhere);
	void readFile(DL<Pair<string,string>> allLexemFromWhere, print& ForPrint);
	void readOneFile(string path, DL<string>& oneColumn, string tableName, int numberOfColumn);

	void GetAllLexemFromWhere(DL<string>& command, DL<Pair<string,string>>& Lexems, DL<Pair<string,string>>& allLexemFromWhere);

	void CreateStructure(DL<Pair<string,string>> allLexemFromWhere, print& ForPrint);
	Pair<int,DL<string>> fillter(print& ForPrint, TreeNode* Where,/* DL<int>& Indexes,*/ string table, string column);
	void compareConst(DL<string>& list, string Const);
	void compare(DL<string>& leftColumn, DL<string>& rightColumn);
	string findConst(int left, int right);
	string PrintColumn(print&, DL<Pair<string, string>>&);

	string DELETE(DL<string>& command);
	void WriteInFile(print& ForWrite, string table);
	void WriteOneFile(print& ForWrite, string table, string path, int countRow);
	void specialSelect(string table, DL<Pair<string,string>>& allLexemsFromWhere, print& ForPrint);
};