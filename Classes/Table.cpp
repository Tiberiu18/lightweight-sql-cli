#include "../Headers/Table.h"
#pragma warning(disable:4996)

int Table::count_tables = 0;

void Table::incrementStatic()
{
	count_tables++;
}

Table::Table(int tableID)
	: tableID(tableID)
{
	this->table_name = new char[strlen("Nume Tabel DEFAULT") + 1];
	strcpy(this->table_name, "Nume Tabel DEFAULT");
}

Table::Table()
	: tableID(++count_tables), cols(nullptr), nrCols(0)
{
	this->table_name = new char[strlen("Nume Tabel DEFAULT") + 1]; 
	strcpy(this->table_name, "Nume Tabel DEFAULT");
}

Table::Table(const char* table_name, Column** cols, int nrCols) // voi primi toate coloanele printr-un vector care contine adrese de coloanele respective
		: tableID(++count_tables) 
{
	if (cols != NULL && nrCols != 0)
	{
		this->nrCols = nrCols;
		this->cols = new Column*[this->nrCols]; // o sa am un vector care contin adrese de coloane, aici aloc [adr1][adr2][adr3]..
		for (int i = 0; i < this->nrCols; i++)
			this->cols[i] = new Column(*cols[i]); // aici aloc obiectele adr1 -> obj1.. si tot asa
	}

	if (table_name != NULL)
	{
		this->table_name = new char[strlen(table_name) + 1];
		strcpy(this->table_name, table_name);
	}
	else
	{

		this->table_name = new char[strlen("Nume Tabel Necunoscut") + 1]; 
		strcpy(this->table_name, "Nume Tabel Necunoscut");
	}

}

Table::Table(const Table& t)
	: tableID(t.tableID)
{
	if (t.cols != NULL && t.nrCols != 0)
	{
		this->nrCols = t.nrCols;
		this->cols = new Column * [t.nrCols]; // o sa am un vector care contin adrese de coloane, aici aloc [adr1][adr2][adr3]..
		for (int i = 0; i < t.nrCols; i++)
			this->cols[i] = new Column(*t.cols[i]); // aici aloc obiectele adr1 -> obj1.. si tot asa
	}

	if (t.table_name != NULL)
	{
		this->table_name = new char[strlen(t.table_name) + 1];
		strcpy(this->table_name, t.table_name);
	}
	else
	{

		this->table_name = new char[strlen("Nume Tabel Necunoscut") + 1]; 
		strcpy(this->table_name, "Nume Tabel Necunoscut");
	}
}

Table& Table::operator=(const Table& t)
{
	if (this != &t)
	{
		if (t.cols != NULL && t.nrCols != 0)
		{
			this->nrCols = t.nrCols;
			this->cols = new Column * [t.nrCols];
			for (int i = 0; i < t.nrCols; i++)
				this->cols[i] = new Column(*t.cols[i]);
		}
		else if (t.cols == nullptr || t.nrCols == 0)
		{
			this->nrCols = 0;
			this->cols = nullptr;
		}

		if (t.table_name != NULL)
		{
			this->table_name = new char[strlen(t.table_name) + 1];
			strcpy(this->table_name, t.table_name);
		}
		else
		{

			this->table_name = new char[strlen("Nume Tabel Necunoscut") + 1];
			strcpy(this->table_name, "Nume Tabel Necunoscut");
		}
	}
		return *this;
}

Column& Table::operator[](int pos)
{
	if (pos >= 0 && this->nrCols > 0)
	{
		return *this->cols[pos];
	}
	else throw exception("Index invalid pentru overload operator[] Table.");
}
// t- 1 => elimina ultima coloana a lui t
Table Table::operator-(int x)
{
	Table rez = *this;
	if (this->nrCols > 0 && this->nrCols >= x)
	{
		Column** aux_arr = rez.cols;
		rez.cols = new Column * [rez.nrCols - x];
		for (int i = 0; i < rez.nrCols - x; i++)
			rez.cols[i] = new Column(*aux_arr[i]);
		rez.nrCols -= x;
		delete[] aux_arr;
	}
	return rez;
}


Table& Table::operator--()
{
	if (this->nrCols > 0 && this->cols != nullptr)
	{
		Column** aux_arr = this->cols;
		this->cols = new Column * [this->nrCols - 1];
		for (int i = 0; i < nrCols - 1; i++)
			this->cols[i] = new Column(*aux_arr[i]);
		this->nrCols -= 1;
		delete[] aux_arr;
	}
	return *this;
}

Table Table::operator--(int)
{
	Table rez = *this;
	if (this->nrCols > 0 && this->cols != nullptr)
	{
		Column** aux_arr = rez.cols;
		rez.cols = new Column * [rez.nrCols - 1];
		for (int i = 0; i < nrCols - 1; i++)
			rez.cols[i] = new Column(*aux_arr[i]);
		rez.nrCols -= 1;
		delete[] aux_arr;
	}
	return rez;
}



Table::~Table()
{
	//for (int i = 0; i < nrCols - 1; i++)
	//	delete this->cols[i]; // DACA PUN LINIA ASTA, imi crapa
	delete[] cols;
	delete[] table_name;
	this->nrCols = 0;
	count_tables--;
}

int Table::getNrCols()
{
	return this->nrCols;
}

Column** Table::getColsArr()
{
	return this->cols;
}

int Table::getTableID()
{
	return this->tableID;
}

string Table::getTableName()
{
	string name = "";
	if( this->table_name != nullptr)
		name = this->table_name;
	return name;
}

int Table::getTablesCount()
{
	return Table::count_tables;
}

void Table::setNrcols(Column, int)
{
	throw ("undefined");
}

void Table::setTableName(const char* table_name)
{
	if (table_name != NULL)
	{
		delete[] this->table_name;
		this->table_name = new char[strlen(table_name) + 1];
		strcpy(this->table_name, table_name);
	}
	else
		cout << "Nume trimis invalid. Nu se face nicio schimbare.";
}


//3.	SELECT(cel_putin_o_coloana, ...) | ALL FROM nume_tabela[WHERE nume_coloană = valoare] - clauza WHERE este opțională
void Table::selectALL()
{
	cout << " Se vor afisa toate coloanele ale tabelului " << this->table_name << ". " << endl;
	for (int i = 0; i < this->nrCols; i++)
		cout << *this->cols[i];
}

void Table::ColUpdateNum(string col_name, float where_val, float replacement)
{
	bool found = false;
	if (col_name.size() > 0)
	{
		for (int i = 0; i < this->nrCols; i++)
		{
			if (col_name == this->cols[i]->getColName())
			{
				this->cols[i]->UpdateNum(where_val, replacement);
				found = true;
			}
		}
	}
	else cout << " Nume coloana trimis incorect. ";
	if (found == false)
		cout << endl<<" Nu exista coloana cu numele " << col_name << "." << endl;
}

void Table::ColUpdateText(string col_name, string where_val, string replacement)
{
	bool found = false;
	if (col_name.size() > 0)
	{
		for (int i = 0; i < this->nrCols; i++)
		{
			if (col_name == this->cols[i]->getColName())
			{
				this->cols[i]->UpdateText(where_val, replacement);
				found = true;
			}
		}
	}
	else cout << " Nume coloana trimis incorect. ";
	if (found == false)
		cout << endl << " Nu exista coloana cu numele " << col_name << "." << endl;
}

void Table::colDeleteFromNum(string col_name, float value)
{
	for (int i = 0; i < this->nrCols; i++)
	{

		if (this->cols[i]->getColName() == col_name)
			this->cols[i]->NumDeleteFrom(value);
	}
}

void Table::colDeleteFromText(string col_name, string value)
{
	for (int i = 0; i < this->nrCols; i++)
	{
		if (this->cols[i]->getColName() == col_name)
			this->cols[i]->TextDeleteFrom(value);
	}
}


void Table::ColInsertText(string value, int pos)
{
	if (pos >= 0)
	{
		for (int i = 0; i < this->nrCols; i++)
		{
			if (i == pos)
				this->cols[i]->insertRowText(value);
		}
	}
}

void Table::ColInsertNum(float value, int pos)
{
	for (int i = 0; i < this->nrCols; i++)
	{
		if( i == pos)
			this->cols[i]->insertRowNumber(value);
	}
}


Table::operator string()
{
	return this->table_name;
}

Table Table::operator!()
{
	Table rez = *this;
	delete[] rez.table_name;
	rez.table_name = NULL;
	return rez;
}

bool Table::operator>(int checker)
{
	if (this->tableID > checker)
		return true;
	return false;
}

bool Table::operator==(const Table& t)
{
	if (this->table_name == t.table_name)
		return true;
	return false;
}

void Table::setNrCols(Column** cols, int nrCols)
{
	if (cols != NULL && nrCols > 0)
	{
		delete[] this->cols;
		this->nrCols = nrCols;
		this->cols = new Column * [this->nrCols];
		for (int i = 0; i < this->nrCols; i++)
			this->cols[i] = new Column(*cols[i]);
	}
}




bool Table::isNumber(string myString) {
	string cp = myString;
	cp.erase(remove(cp.begin(), cp.end(), ' '), cp.end()); // should rmeove all whitespaces from string
	std::istringstream iss(cp);
	float f;
	iss >> noskipws >> f; // noskipws considers leading whitespace invalid
	// Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

//1.	INSERT INTO nume_tabela VALUES(...); valorile sunt separate prin, și au numărul și ordinea exacta ca definiția tabelului
// STUDENTI: INALTIME, VARSTA, FACULTATE
// INSERT INTO STUDENT VALUES(180, 18, "CSIE");





void Table::ColInsertInto(string* val_arr, int size)
{
	for (int i = 0; i < this->nrCols; i++)
	{
		if (this->cols[i]->getType() == "text")
		{
			if (isNumber(val_arr[i]) == 0)
			{
				string s = val_arr[i];
				if (s == "")  
					this->ColInsertText(this->cols[i]->getTextDefaultVal(), i);
				else this->ColInsertText(s, i);
			}
		}
		else if (this->cols[i]->getType() == "number")
		{
			if (isNumber(val_arr[i]) == 1)
			{
				float num = stof(val_arr[i]);
				this->ColInsertNum(num, i);
			}
			else if (isNumber(val_arr[i]) == 0 && val_arr[i] == "") // daca nu primim inregistrare, pune valoarea default
				this->ColInsertNum(this->cols[i]->getFloatDefaultVal(), i);

		}
	}
	
}

ostream& operator<<(ostream& OUT, const Table& t) {
	OUT << "ID Tabel: " << t.tableID << endl;
	if (t.table_name != nullptr)
		OUT << "Nume Tabel: " << t.table_name << "\n";
	if (t.nrCols > 0 && t.cols != nullptr)
	{
		OUT << "Lista coloane: ";
		for (int i = 0; i < t.nrCols; i++)
		{
			OUT << "\n" << i+1 << ": " << t.cols[i]->getColName();
		}

		for (int i = 0; i < t.nrCols; i++)
			OUT << *t.cols[i]; 
	}
	OUT << "********************** PRINTARE FINALIZATA ********************** ";
	return OUT;
}

istream& operator>>(istream& in, Table& t)
{
	delete[] t.table_name;
	delete[] t.cols;
	t.table_name = new char[1024];
	cout << "\nIntroduceti nume tabel: ";
	in.ignore();
	in.getline(t.table_name, 1024, '\n');
	cout << "\nIntroduceti numarul coloanelor: ";
	in >> t.nrCols;
	if (t.nrCols > 0)
	{
		Column* c = new Column[t.nrCols];  // deci e pointer la un vector de coloane
		for (int i = 0; i < t.nrCols; i++)
			cin >> c[i]; // scriu in fiecare coloana c[0] ..
		t.cols = new Column * [t.nrCols]; // deci t contine un vector de adrese de coloane 
		for (int i = 0; i < t.nrCols; i++)
			t.cols[i] = new Column(c[i]);

	}
	else
	{
		t.nrCols = 0;
		t.cols = nullptr;
	}
	return in;

}

size_t Table::calcSize()
{
	size_t total_Size = strlen(this->table_name) + sizeof(this);
	for (int i = 0; i < this->nrCols; i++)
	{
		total_Size += this->cols[i]->returnSize();
	}
	return total_Size;
}