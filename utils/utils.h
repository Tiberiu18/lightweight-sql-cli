#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <Windows.h>
#include <locale>
#include <codecvt>
#include "../Headers/Database.h"
using namespace std;
class Utility
{
public:
	static int getNrDigits(float);
	static bool checkEqualSign(string);
	static bool checkEqualSignUpdate(string);
	static int countPars(string&);
	static bool checkParenthesis(string);
	static bool checkParenthesisInsert(string);
	static string formStringInsert(string );
	static string* regexInsert(string );
	static bool checkParsOrder(string );
	static bool file_exists(const string&);
	static string ws2s(const wstring&);
	static void getFileNames(vector<string>*);
	static int getLastSelectNum();
	static string getPath();
	static void reverseSlashes(string&);
	static wstring s2ws(const string&);
	static int calcNrCmds1(string); // calculeaza cate comenzi avem intr-un fisier text 
	static string* getArgCmds(int, string);
	static string toLower(string&);
	static void toUpper(string&);
	static bool findSep(string);
	static bool check_Nothing_After(string);
	static bool check_Not_Double_Commas(string);
	static bool check_Nothing_After_Ins(string);
	static bool checkImport(string);
};

