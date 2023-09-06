#pragma once
#include "GetCmdType.h"
#include <windows.h>	
#include <cstdio>
#include <map>
#define intSz sizeof(int)
#define floatSz sizeof(float)
class IFile
{
	string state;
public:
	IFile(string state)
		: state(state)
	{
	}
	virtual void writeToFile(fstream& f, string ) = 0;
	virtual string readState() = 0;
};


// vreau sa generez fisiere txt cand fac SELECT SAU DISPLAY





class DropHandling
{
	
public:
	getCmdType& g;
	DropHandling(getCmdType& g)
		: g(g)
	{
	}
	void deleteTable()
	{
		string file_Name = "TABEL_"+g.res2[4];
		remove(file_Name.c_str());
	}
};









class InsertHandling : public IFile
{
public:
	Database& db;
	int pos = -1;

	InsertHandling(Database& db)
		: db(db), IFile("Inserting...")
	{
	}


	void writeToFile(fstream& f, string tableName) 
	{
		int i = 0;
		// INSERT INTO STUDENT VALUES ..
		list<Table*>::iterator holdItr;
		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
		{
			if ((**itr).table_name == tableName)
			{
				pos = i;
				holdItr = itr;
			}
			i++;
		}
		// mi-ar trebui aceeasi pozitie cu numele tabelului
		if (pos >= 0)
		{
			const Table& t = **holdItr;
			//scriu TableID
			f.write((char*)&t.tableID, intSz);
			//Scriem descrierea coloanelor si numarul de coloane
			f.write((char*)&t.nrCols, intSz);
			if (t.cols != nullptr && t.nrCols > 0)
			{
				for (int i = 0; i < t.nrCols; i++)
					t.cols[i]->writeAll(f);
			}
		}
		
	}
	string readState()
	{
		string res = "Inserting...";
		return res;
	}
};



class FHandling
{
protected:
	string type;
public:
	Database& db;
	FHandling(Database& db, string type = "FHandling")
		: db(db), type(type)
	{
	}
};



class FileHandling : public FHandling
{
protected:
	static int pos;
	string state;
public:
	/*Database& db; */
	FileHandling(Database& db, string type = "FileHandling", string state = "-")
		: FHandling(db, type), state(state)
	{
	}

	void writeToFile(fstream& f, int pos )
	{
		state = "Write new file";
		// deci, eu vreau sa merg la tabelul de pe pozitia pos din db si sa il scriu in fisier
		// apoi incrementez pos
		int aux = 0;
		if (pos < db.nrTables)
		{
			for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
			{
				if (aux == pos)
				{
					const Table& t = **itr;
					//scriu TableID
					f.write((char*)&t.tableID, intSz);
					//Scriem descrierea coloanelor si numarul de coloane
					f.write((char*)&t.nrCols, intSz);
					if (t.cols != nullptr && t.nrCols > 0)
					{
						for (int i = 0; i < t.nrCols; i++)
							t.cols[i]->writeAll(f); // writeColumnDetails
					}
				}
				aux++;
			}
		}
		
	}
	

	void createExistingTables()
	{
		state = "Creating Tables from existing Files";
		vector<string> fileList;
		Utility::getFileNames(&fileList);
		vector<string>::iterator it;
		string* tableNames = new string[fileList.size()];
		map<string, int> presentInDb;
		// trebuie sa iau de la Tabel_ in sus
		for (int i = 0; i < fileList.size(); i++)
		{
			tableNames[i] = fileList[i].substr(6); // de aici iau numele tabelelor
		}
		for (int k = 0; k < fileList.size(); k++)
		{
			string x = tableNames[k];
			presentInDb.insert(pair<string, int>(x, 0));
		}


		
		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
		{
			for (int j = 0; j < fileList.size(); j++) // fileList.size() should be the same length as db.nrTables
			{
				if ((*itr)->getTableName() == tableNames[j])
				{
					if (presentInDb.find(tableNames[j]) != presentInDb.end())
					{
						presentInDb[tableNames[j]] = 1;
					}
				}
			}
		}

		for (int i = 0; i < fileList.size(); i++) 
		{
			if (presentInDb[tableNames[i]] == 0)
			{
				fstream f(fileList[i], ios::in);
				int tableID = 0;
				f.read((char*)&tableID, intSz);
				Table* t = new Table(); // new Table(tableID)
				t->table_name = new char[tableNames[i].length() + 1]; // ii copiem numele
				strcpy(t->table_name, tableNames[i].c_str());
				f.read((char*)&t->nrCols, intSz); // ii pun numarul de coloane
				t->cols = new Column * [t->nrCols];

				if (t->nrCols > 0)
				{
					for (int j = 0; j < t->nrCols; j++)
					{
						t->cols[j] = new Column();
						t->cols[j]->readAll(f);
					}
					this->db.addTable(t);
					for (int j = 0; j < t->nrCols; j++)
						delete t->cols[j];
					delete[] t->cols;
				}
				f.close();
			}
		}
		delete[] tableNames;
	}

	//So, this function will INSERT DATA INTO the table
	// the number of rows in the csv file must be a multiple of nr_of_cols from the given table
	// If we have 3 columns => we must have 3 or 6 or 9 or .. rows
	// We will insert the data one by one, meaning, one row goes for the first column, the other row for the second and so on
	// IMPORT nume_tabela nume_fisier.csv
	bool ImportFrom(string fileName, string tableName)
	{
		bool ok = Utility::file_exists(fileName);
		bool modifiedTable = false;
		if (ok == true)
		{
			ifstream fin(fileName);

			list<Table*>::iterator itr;
			size_t foundSep;
			bool other_sep = false;
			bool foundTable = false;
			if (fin.is_open())
			{
				string line;
				vector<string> rawdata;
				while (getline(fin, line)) {
					stringstream ss(line);
					foundSep = line.find(',');
					if (foundSep == string::npos)
					{
						cout << "\nSeparatorul nu este virgula!\n";
						break;
					}
					other_sep = Utility::findSep(line);
					if (other_sep == true)
					{
						cout << "\nSeparatorul nu este virgula!\n";
						break;
					}
					char c;
					bool insideQuotes = false;
					std::string currentField;
					while (ss.get(c)) {
						if (c == '"') {
							insideQuotes = !insideQuotes;
						}
						if (!insideQuotes && c == ',') {
							rawdata.push_back(currentField);
							currentField.clear();
						}
						else {
							currentField += c;
						}
					}

				}
				if (foundSep != string::npos && other_sep == false)
				{
					for (itr = db.tables.begin(); itr != db.tables.end(); ++itr)
					{
						if ((*itr)->table_name == tableName) // find the DESIRED table
						{
							foundTable = true;
							break;
						}
					}
					if (foundTable == true)
					{
						int nrC = (*itr)->nrCols;
						int j;
						bool divisible = false;
						if (rawdata.size() % nrC == 0)
							divisible = true;


						// now, in rawdata we have our desired vals. 
						if (divisible == true)
						{
							string* values = new string[nrC];
							for (int i = 0; i < rawdata.size();)
							{
								for (j = 0; j < nrC; j++)
								{
									values[j] = rawdata[i++];
								}
								if (j == nrC)
									db.insertInto(tableName, values, nrC);
							}
							delete[] values;
							modifiedTable = true;
						}
						else cerr << "\nNumarul de valori introduse nu sunt multiplu de numarul coloanelor!\n";
					}
					else cerr << "\nTabelul nu exista!\n";
				}
			}
			fin.close();
		}
		else cerr << "\nFisierul nu exista sau comanda nu este bine formatata!\n";
		return modifiedTable;
	}
	

};

class DisplayHandling : public FHandling
{
	int count = 1; // static
public:
	DisplayHandling(Database& db, string type="DisplayHandling")
		: FHandling(db, type)
	{
	}
	void writeToFile(string tableName)
	{
		count = Utility::getLastSelectNum() + 1;
		if (count < 0)
			count = 0;
		string fileName = "SELECT_" + to_string(count) + ".txt";

		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
		{
			if ((*itr)->getTableName() == tableName)
			{
				// apelez ofstream& din Table
				fstream f(fileName, ios::out);
				operator<<(f, **itr);
				f.close();
				count++;
			}
		}

	}
};

