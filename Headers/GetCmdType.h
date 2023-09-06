#pragma once
#include "../utils/utils.h"
#include "Database.h"


class getCmdType
{
private:
	
	string input;
	bool syntaxCreateT = false;
	int count_cols = 0;
	char* obj_name = nullptr;
	friend class DropHandling;
public:
	int return_pos = -1;
	string res2[200] = {};
	getCmdType(string, const char*);
	getCmdType(string);
	getCmdType(const getCmdType&);
	getCmdType operator=(const getCmdType&);
	~getCmdType();
	string operator[](int);
	getCmdType operator+(string);
	getCmdType& operator++(); // dubleaza ultima valoare in vectorul de stringuri res2
	getCmdType operator++(int);
	operator string();
	getCmdType operator!();
	bool operator<(const getCmdType& );
	bool operator==(const getCmdType& );
	friend ostream& operator<<(ostream& , const getCmdType& ); // verifica daca functioneaza 
	friend istream& operator>>(istream& , getCmdType& );
	string getInput();
	int getNrCols();
	string getName();
	void setInput(string );
	void setNrCols(int, string);
	void setName(const char*);
	bool verifyCreateTable();
	Table* createCols();
	void displayTable(Database&);
	void dropTable(Database&);

	//1.	INSERT INTO nume_tabela VALUES(...), valorile sunt despartite prin ,
	bool InsertVerify(Database&);
	void insertInto(Database&, string*);
	// SELECT ALL FROM NUME_TABEL
	void selectALL(Database& );
	void deleteFrom(Database&);

	//4.	UPDATE nume_tabela SET nume_coloană = valoare WHERE nume_coloană = valoare (coloana SET poate fi diferită de cea WHERE; UPDATE merge doar pentru o coloana)
	// UPDATE STUDENT SET INALTIME = 50 WHERE INALTIME = 10
	bool Update(Database&);
	int getCountCols();


};