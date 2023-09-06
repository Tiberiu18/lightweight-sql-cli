#pragma once
#include "Table.h"
class Database
{
private:
	const int dbID = 0;
	const char* dbName = "Baza de date";
	list<Table*> tables;
	static const int count_DB = 1;
	int nrTables = 0;
	friend class FileHandling;
	friend class InsertHandling;
	friend class DisplayHandling;
	friend class Menu1;
	friend class Menu2;
	friend class Menu3;
	friend class getCmdType;

	
public:
	Database(int); // Deci, pot avea initial un database fara tabele si le adaug pe parcurs
	Database(Table**, int );
	Database(const Database& );
	Database& operator=(const Database&);
	~Database();
	Table& operator[](int);
	// db1 = db2 + 1
	Database operator+(unsigned int);
	// operator -- => --db 
	Database& operator--(); // PREDECREMENTARE
	Database operator--(int); // POSTDECREMENTARE
	explicit operator int();
	Database operator!();
	bool operator>=(const Database&);
	bool operator==(const Database&);
	int getNrTables();
	list<Table*> getTables();
	void setNrTables(Table** tables, int nrTables);
	// cand ii dau create table, adaug tabela creata la database

	void addTable(Table*);
	void dropTable(string);
	void displayTable(string);
	void insertInto(string, string*, int);
	void delFrom(string, string, string);
	void selectALL(string);
	bool Update(string, string, string, string);
	string  getdbName() const;
	friend ostream& operator<<(ostream&, const Database&);
	friend istream& operator>>(istream&, Database&);
};