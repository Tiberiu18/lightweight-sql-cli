﻿#define CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include "Headers/Column.h"
#include "Headers/Table.h"
#include "Headers/Database.h"
#include "utils/utils.h"
#include "Headers/GetCmdType.h"
#include "Headers/Phase2.h"
#include "Headers/Menu.h"
#include <regex>
#include <vector>
#include <stack>
#include <set>
#include <memory>
#include <io.h>
using namespace std;

void readME()
{
	ifstream f("ReadMe.md");
	if (f.is_open())
		cout << f.rdbuf();
	f.close();
}


int FileHandling::pos = 0;



int main(int argc, char* argv[])
{

	// primim 2 fisiere: comenzi. txt si comenzi2.txt
	// comenzi.txt o sa contina instructiuni  CREATE TABLE una dupa alta pana la /n\
	// al doilea fisier contine comenzi de tipul INSERT
	int count_CREATE = 0;
	int count_INSERTS = 0;
	int nrCmds = 0;
	int nrCmds2 = 0;
	string comenzi1Path = Utility::getPath() + "\\comenzi.txt";
	string comenzi2Path = Utility::getPath() + "\\comenzi2.txt";
	Utility::reverseSlashes(comenzi1Path);
	Utility::reverseSlashes(comenzi2Path);
	nrCmds = Utility::calcNrCmds1(comenzi1Path);
	nrCmds2 = nrCmds + Utility::calcNrCmds1(comenzi2Path);
	string* cmds = NULL;
	cmds = Utility::getArgCmds(nrCmds, comenzi1Path);
	count_INSERTS = nrCmds;
	string* cmds2 = Utility::getArgCmds(nrCmds2, comenzi2Path);
	

	string input;
	bool ok = true;
	Database db(1);
        std::unique_ptr<Menu1> mArray[] = { std::make_unique<Menu1>(db), std::make_unique<Menu2>(db), std::make_unique<Menu3>(db) };
	FileHandling f1(db);
	f1.createExistingTables();
	if (db.getNrTables() > 0)
	{
		cout << db;
		for (int i = 0; i < 3; i++) // print 3 lines 
			cout << "\n";
	}
	while (ok == true)
	{
		if (count_CREATE < nrCmds)
		{
			input = cmds[count_CREATE];
			count_CREATE++;
		}
		else if (count_CREATE == nrCmds && count_INSERTS < nrCmds2)
		{
			input = cmds2[count_INSERTS - nrCmds];
			count_INSERTS++;
		}
		else if ( count_CREATE == nrCmds && count_INSERTS >= nrCmds2 )
		{
			getline(cin, input);
		}
		Utility::toUpper(input);
		string firstWord = input.substr(0, input.find(" "));
		getCmdType g1(input, "OBIECT AJUTATOR");
		static int count_cols = g1.getCountCols();
		if (firstWord == "CREATE")
		{
			if (g1.verifyCreateTable() == true)
			{
				string table_name = "TABEL_" + g1.res2[4];
				string path = "binaryfiles/";
				if (Utility::file_exists(path + table_name) == false)
				{
					try
					{
						Table* p = g1.createCols();
						db.addTable(p);
						if (count_CREATE >= nrCmds)
							cout << db;
						//FileHandling f1(db);
						fstream f(path + table_name, ios::out);
						f1.writeToFile(f, db.getNrTables() - 1);
						f.close();
					}
					catch (exception ex)
					{
						cout << ex.what();
					}
				}
				else cout << " Tabelul deja exista! " << endl;
			}
			else cout << " Sintaxa incorecta pentru CREATE TABLE." << endl;
		}
		else if (firstWord == "DISPLAY")
		{
			DisplayHandling d(db);
			g1.displayTable(db);
			// g1.res2[4] is the name of the table
			d.writeToFile(g1.res2[4]);
		}
		else if (firstWord == "DROP")
		{
			DropHandling d(g1);
			g1.dropTable(db);
			d.deleteTable();
		}
		else if (firstWord == "INSERT" || firstWord == "IMPORT")
		{
			InsertHandling i1(db);
			if (firstWord == "INSERT")
			{
				bool ok = Utility::checkParenthesisInsert(input);
				bool ok2 = g1.InsertVerify(db);
				if (ok == true)
				{
					if (ok2 == true)
					{

						string copy = input;
						string res = Utility::formStringInsert(copy);
						string* arr = new string[20];
						arr = Utility::regexInsert(res);
						g1.insertInto(db, arr);
						int t_POS = g1.return_pos;
						if (t_POS >= 0) // DACA TABELUL DORIT EXISTA PT INSERARE
						{ // ATUNCI SCRIE IN FISIERUL LUI 
							fstream f("TABEL_" + g1.res2[4], ios::out);
							i1.writeToFile(f, g1.res2[4]);
							f.close();
						}
					}
					else cout << "Sintaxa incorecta pentru INSERT INTO." << endl;
				}
				else cout << " Paranteze neechilibrate. Inserati din nou comanda. " << endl;
			}
			else if (firstWord == "IMPORT")
			{

				string name_tbl = g1.res2[2];
				string file_name = g1.res2[4];
				bool modifiedTable = f1.ImportFrom(file_name, name_tbl);
				if (modifiedTable == true)
				{
					fstream f("TABEL_" + g1.res2[2], ios::out);
					i1.writeToFile(f, g1.res2[2]);
					f.close();
				}
				
			}
		}


		else if (firstWord == "SELECT")
		{
			DisplayHandling d(db);
			g1.selectALL(db);
			// g1.res2[6] = numele tabelei
			d.writeToFile(g1.res2[6]);
		}
		else if (firstWord == "DELETE")
		{
			InsertHandling i1(db);
			g1.deleteFrom(db);
			int t_POS = g1.return_pos; // if the table is FOUND
			if (t_POS >= 0) 
			{ 
				fstream f("TABEL_" + g1.res2[4], ios::out);
				i1.writeToFile(f, g1.res2[4]);
				f.close();
			}
		}
		else if (firstWord == "UPDATE")
		{
			InsertHandling i1(db);
			bool updated = false;
			updated = g1.Update(db);
			if (updated == true)
			{
				fstream f("TABEL_" + g1.res2[2], ios::out);
				i1.writeToFile(f, g1.res2[2]);
				f.close();
			}
		}
		else if (firstWord == "MENU" )
		{
			input = "MENU";
			while (input == "MENU")
			{
				cout << "1) List all table names from the database.\n2)List all rows existing in the database\n3)List size of tables\n4)Exit\n";
				string inp;
				cin >> inp;
				bool ok = false;
				ok = Table::isNumber(inp);
				if (ok == true)
				{
					int x = stof(inp);
					switch (x) {
					case 1:
						mArray[0]->display();
						break;
					case 2:
						mArray[1]->display();
						break;
					case 3:
						mArray[2]->display();
						break;
					case 4:
						cin.ignore();
						break;
					}
				}
			}
		}
		else if (firstWord == "--HELP")
			readME();
		else if (firstWord == "--CLEAR")
		{
			for (int i = 0; i < 100; i++)
				cout << endl;
		}
		else if (firstWord == "--SHOWDATABASE")
			cout << db;
		else cout << "Comanda nerecunoscuta. Tastati --help pentru ajutor. Tastati --clear pentru a elibera consola" << endl;
		
	}
	
        // unique_ptr handles cleanup
}

