#pragma once
#include "Column.h"
class Table
{
private: 
	Column** cols = NULL;
	int nrCols = 0;
	const int tableID;
	char* table_name;
	static int count_tables;
	friend class FileHandling;
	friend class InsertHandling;
public:

	Table();
	Table(int);
	Table(const char*, Column**, int);
	Table(const Table&);
	Table& operator=(const Table&);
	Column& operator[](int );
	// t - 1 => elimina ultima coloana a lui t
	Table operator-(int);
	Table& operator--(); // --t 
	Table operator--(int); // t--;

	~Table();

	// Column getCols
	int getNrCols();
	Column** getColsArr();
	int getTableID();
	string getTableName();
	static int getTablesCount();
	void setNrcols(Column, int); // deocamdata nu il implementez
	void setTableName(const char* table_name);
	
	operator string();//conversie char* table_name in string
	Table operator!();//eliminare valoarea lui char* table_name
	bool operator>(int );// comparare tableID cu o valoare(de preferat 0, insemnand ca tabela exista)
	bool operator==(const Table& );// verificare daca esxista doua tabele cu aceeasi denumire
	void setNrCols(Column** , int );


	void selectALL();

	void ColInsertText(string, int);
	void ColInsertNum(float, int);

	void ColUpdateNum(string , float , float );
	void ColUpdateText(string, string, string);

	void colDeleteFromNum(string, float);
	void colDeleteFromText(string , string );
	static void incrementStatic();
	
	void ColInsertInto(string*, int);
	static bool isNumber(string);
	friend ostream& operator<<(ostream&, const Table&);
	friend istream& operator>>(istream&, Table&);
	size_t calcSize();
};