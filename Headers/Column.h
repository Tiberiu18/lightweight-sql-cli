#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <list>
#include <set>
using namespace std;
class Column
{
private:
	char* col_name = nullptr;
	float* vals = nullptr;
	int max_dim = 5;
	float default_val = 0;
	int rows = 0;
	string* text_vals = nullptr;
	string text_def_val = "DEFAULT";
	string type;
	friend class FileHandling;
public:
	Column();
	Column(const char*, string, int, float); // constructor coloana pt tipul FLOAT si INTEGER
	Column(const char*, string, int, string); // constructor coloana pt tipul TEXT
	Column(const Column&);
	void writeColumnDetails(fstream&);
	void readColumnDetails(fstream&);
	void writeColumnRows(fstream& );
	void writeAll(fstream&);
	void readAll(fstream&);
	Column& operator=(const Column&); 
	float& operator[](int);
	//d1 = d1 +5 => adauga 5 la max_dim
	Column operator+(int);
	// PREDECREMENTARE
	Column& operator++();
	//POSTDECREMENTARE
	Column operator++(int);
	operator int();
	Column operator!();
	bool operator<(const Column&); // verifica daca coloana this are mai putine inregistrari decat alta coloana
	bool operator==(const Column&);// verifica daca coloana this are acelasi nr de inregistrari decat alta coloana
	~Column(); // destructor
	string getType();
	string getColName(); 
	string getTextDefaultVal();
	float getFloatDefaultVal();
	int getRows();
	int getMaxDim();
	float* getNumValuesArr();
	void setColName(const char* col_name);
	static bool isNumber(string);

	void setMax_dim(int );
	void setRowsVals(int , float* );
	void setRowsText_Vals(int , string* );
	void setDefault_val(float );
	void setText_def_val(string );


	

	// INSERT SINGLE ROWS
	void insertRowNumber(float);
	void insertRowText(string);

	// INSERT MULTIPLE ROWS ( INSERT VALUES )
	void insertMultipleNum(float*, int);
	void insertMultipleText(string*, int);

	void TextDeleteFrom(string );
	void NumDeleteFrom(float ); 

	//4.	UPDATE nume_tabela SET nume_coloan? = valoare WHERE nume_coloan? = valoare(coloana SET poate fi diferit? de cea WHERE; UPDATE merge doar pentru o coloana)
	void UpdateText(string , string );
	void UpdateNum(float, float);
	
	friend ostream& operator<<(ostream&, const Column&);
	friend istream& operator>>(istream&, Column&);
	size_t returnSize();
};

