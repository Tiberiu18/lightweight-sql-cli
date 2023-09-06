#include "../Headers/Database.h"

const int Database:: count_DB ;

Database::Database(int dbID)
	: dbID(dbID)
{
}




Database::Database(Table** tables, int nrTables)
{
	if (tables != nullptr && nrTables > 0)
	{
		this->nrTables = nrTables;
		for (int i = 0; i < nrTables; i++)
		{
			Table* c = new Table(*tables[i]);
			this->tables.push_back(c);
			//this->tables[i] = new Table(*tables[i]);
		}
	}
}

Database::Database(const Database& db)
	: dbID(db.dbID)
{
	if ( db.tables.size() > 0 )
	{
		this->nrTables = db.tables.size();
		for(auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
		{
			Table* c = new Table(**itr);
			this->tables.push_back(c);
		}
	}

}

Database& Database::operator=(const Database& db)
{
	delete[] this->dbName;
	this->tables.clear();
	if (this != &db)
	{
		if (db.tables.size() > 0)
		{
			this->nrTables = db.tables.size();
			for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
			{
				Table* c = new Table(**itr);
				this->tables.push_back(c);
			}
		}
		else
		{
			this->nrTables = 0;
		}
	}
	return *this;
}



// overload operator cast la int, returneaza numarul total de tabele
Database::operator int()
{
	return this->nrTables;
}



// verifica daca this are mai multe tabele decat alta baza de date
bool Database::operator>=(const Database& db)
{
	return this->nrTables >= db.nrTables;
}

bool Database::operator==(const Database& db)
{
	return this->nrTables == db.nrTables;
}

int Database::getNrTables()
{
	return this->nrTables;
}

list<Table*> Database::getTables()
{
	return this->tables;
}

Database::~Database()
{
	/*for (int i = 0; i < nrTables ; i++)
		delete this->tables[i];*/ // NU MERGE ASTA
	delete[] this->dbName;
	this->tables.clear();
}

void Database::setNrTables(Table** tables, int nrTables)
{
	if (nrTables > 0)
	{
		this->nrTables = nrTables;
		for (int i = 0; i < nrTables; i++)
		{
			Table* c = new Table(*tables[i]);
			this->tables.push_back(c);
		}
	}
}


void Database::addTable(Table* table)
{
	
	if (table != nullptr)
	{
		//if (this->nrTables > 0) // daca mai avem deja tabele in DB
		Table* c = new Table(*table);
		this->tables.push_back(c);
		this->nrTables = this->tables.size();
		
	}
	else cerr << "\t TABEL TRIMIS PENTRU ADAUGARE INVALID" << endl;
}




void Database::dropTable(string tabl_name) // INSTRUCTIUNEA DROP TABLE 
{
	bool found = false;
	list<Table*>::iterator holdItr;
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ( (*itr)->getTableName() == tabl_name) 
		{
			found = true;
			this->nrTables -= 1;
			holdItr = itr;
			break;
		}
	}
	this->tables.erase(holdItr);
	if (found == false)
		cout << "Nu exista niciun tabel cu acest nume." << endl;
}


void Database::displayTable(string tabl_name)
{
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ((*itr)->getTableName() == tabl_name)
		{
			cout << **itr;
			break;
		}
	}
}

void Database::insertInto(string tabl_name, string* val_arr, int size)
{
	bool match = true;
	int i = 0;
	int pos = -1;
	int nrC = 0;
	list<Table*>::iterator holdItr;
	// primim un string de val_arr
	// VERIFICAM DACA VALORILE SE POTRIVESC CU TIPUL COLOANEI
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ((*itr)->getTableName() == tabl_name)
		{

			pos = i; // salvam pozitia tabelului in lista tables
			nrC = (*itr)->getNrCols();
			holdItr = itr; // i think I could use itr
			break;
		}
		i++;
	}

	string* types = new string[nrC]; // aici punem tipurile coloanelor in ordinea in care sunt
	// putem folosi si parametrul size primit
	int k = 0;
	if (pos != -1)
	{
		for (int i = 0; i < nrC; i++)
			//types[k++] = this->tables[pos]->getColsArr()[i]->getType();
			types[k++] = (*holdItr)->getColsArr()[i]->getType();
	}
	if (val_arr != nullptr)
	{
		for (int i = 0; i < nrC; i++)
		{
			if ((Table::isNumber(val_arr[i])) == 1 && types[i] != "number")
			{

				match = false;
			}
			else if ((Table::isNumber(val_arr[i])) == 0 && types[i] != "text")
				match = false;
				
		}
	}

	if (match == true)
	{
		/*for (int i = 0; i < this->nrTables; i++)
		{
			if (this->tables[i]->getTableName() == tabl_name)
			{
				this->tables[i]->ColInsertInto(val_arr, size);
			}
		}*/
		(*holdItr)->ColInsertInto(val_arr, size);
	}
	else cout << "Tipul coloanei cu valoarea dorita a se insera nu se potrivesc." << endl;

	delete[] types;
}


void Database::delFrom(string tabl_name, string col_name, string val) 
{
	int tabl_pos = -1;
	int col_pos = -1;
	int i = 0;
	int nrC = 0;
	list<Table*>::iterator holdItr;
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ((*itr)->getTableName() == tabl_name)
		{
			tabl_pos = i; // gasim pozitia tabelului in vectorul tables
			holdItr = itr;
			nrC = (*itr)->getNrCols();
			break;
		}
		i++;
	}
	if (tabl_pos >= 0)
	{
		for (int i = 0; i < nrC; i++)
		{
			if ((*holdItr)->getColsArr()[i]->getColName() == col_name)
				col_pos = i;
		}
		if (col_pos >= 0 && (*holdItr)->getColsArr()[col_pos]->getType() == "text") // APELAM METODELE COLOANEI IN FUNCTIE DE TIPUL ACESTORA ( TEXT SAU NUMAR )
		{
			(*holdItr)->getColsArr()[col_pos]->TextDeleteFrom(val);
		}
		else if (col_pos >= 0 && (*holdItr)->getColsArr()[col_pos]->getType() == "number")
		{
			if (Table::isNumber(val) == 1) // verificam daca valoarea trimisa pentru stergere poate fi numar sau nu
			{
				float v = stof(val); // cast string la float
				(*holdItr)->getColsArr()[col_pos]->NumDeleteFrom(v);
			}
			else cout << " Valoarea inserata pentru stergere nu se potriveste cu tipul coloanei ( text sau numar )." << endl;
		}
		else if (col_pos == -1)
			cout << endl << " Nu s-a gasit coloana dorita in tabelul " << tabl_name << "." << endl;
	}
	else cout << endl << " Nu s-a gasit tabelul cu numele " << tabl_name << "." << endl;
}


void Database::selectALL(string tabl_name)
{
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ((*itr)->getTableName() == tabl_name)
		{
			(*itr)->selectALL();
		}
	}
}



// INSTRUCTIUNEA UPDATE
bool Database::Update(string tabl_name, string col_name, string where_val, string replacement)
{
	int tabl_pos = -1;
	int col_pos = -1;
	bool return_pos = false;
	int i = 0;
	int nrC = 0;
	list<Table*>::iterator holdItr;
	for (auto itr = this->tables.begin(); itr != this->tables.end(); itr++)
	{
		if ((*itr)->getTableName() == tabl_name)
		{
			tabl_pos = i; // gasim pozitia tabelului in vectorul tables
			holdItr = itr;
			nrC = (*itr)->getNrCols();
			break;
		}
		i++;
	}
	if (tabl_pos >= 0)
	{
		for (int i = 0; i < nrC; i++)
		{
			if ((*holdItr)->getColsArr()[i]->getColName() == col_name)
				col_pos = i;
		}
	}
	else if (tabl_pos == -1)
		cout << "Tabelul din care se doreste UPDATE nu exista!" << endl;
	// Vom verifica tipul tabelei
	if (col_pos >= 0)
	{
		if ((*holdItr)->getColsArr()[col_pos]->getType() == "text")
		{
			if ((Table::isNumber(where_val) == Table::isNumber(replacement)))
			{
				(*holdItr)->getColsArr()[col_pos]->UpdateText(where_val, replacement);
				return_pos = true;
			}
			else cerr << " Valorile trimise pentru UPDATE nu se potrivesc la tip ( text sau numar)." << endl;
		}
		else if ((*holdItr)->getColsArr()[col_pos]->getType() == "number")
		{
			if ((Table::isNumber(where_val) && Table::isNumber(replacement)) == 1) // daca valorile primite sunt in regula, adica sunt numere
			{
				float where_v = stof(where_val); // le transformam in number
				float replac = stof(replacement);
				(*holdItr)->getColsArr()[col_pos]->UpdateNum(where_v, replac);
				return_pos = true;
			}
			else cerr << " Valorile trimise pentru UPDATE nu sunt de tipul corect, in sensul ca una este numar, alta string. ";
		}
	}
	else if (col_pos == -1)
		cout << "Coloana din care se doreste UPDATE nu exista!." << endl;
	return return_pos;
}


string Database::getdbName() const
{
	string name;
	name = this->dbName;
	return name;
}



ostream& operator<<(ostream& out, const Database& db)
{
	out << "****** Tabelele bazei de date sunt: ******" << endl;
	for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
	{
		out << (**itr);
	}
	return out;
}

