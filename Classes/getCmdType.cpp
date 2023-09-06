#include "../Headers/GetCmdType.h"
#pragma warning(disable:4996)

getCmdType::getCmdType(string in, const char* obj_name)
	: input(in)
{
	string copy = in;
	regex words_regex("[^\\s(())!?=,]+");
	auto words_begin = sregex_iterator(in.begin(), in.end(), words_regex);
	auto words_end = sregex_iterator();
	int j = 0;
	int k = 0;
	for (sregex_iterator i = words_begin; i != words_end; ++i)
	{
		if ((*i).str() == "TEXT" || (*i).str() == "FLOAT" || (*i).str() == "INTEGER")
		{
			this->count_cols++;
		}
		res2[j++] = (*i).str();
		j++; // am gresit ca am pus de doua ori..am mers cu vectorul din 2 in 2 pozitii
	}

	/*this->cols_order = new string[count_cols];
	for (sregex_iterator i = words_begin; i != words_end; ++i)
	{
		if ((*i).str() == "text" || (*i).str() == "float" || (*i).str() == "integer")
		{
			this->cols_order[k++] = (*i).str();
		}
	}*/

	if (obj_name != nullptr)
	{
		this->obj_name = new char[strlen(obj_name) + 1];
		strcpy(this->obj_name, obj_name);
	}
}

getCmdType::getCmdType(string in)
	: input(in)
{
	string copy = in;
	regex words_regex("[^\\s(())!?=,]+");
	auto words_begin = sregex_iterator(in.begin(), in.end(), words_regex);
	auto words_end = sregex_iterator();
	int j = 0;
	int k = 0;
	for (sregex_iterator i = words_begin; i != words_end; ++i)
	{
		if ((*i).str() == "text" || (*i).str() == "float" || (*i).str() == "integer")
		{
			this->count_cols++;

		}
		res2[j++] = (*i).str();
		j++; // am gresit ca am pus de doua ori..am mers cu vectorul din 2 in 2 pozitii
	}
}

getCmdType::getCmdType(const getCmdType& g)
	: input(g.input), syntaxCreateT(g.syntaxCreateT), count_cols(g.count_cols)
{
	for (int i = 0; i < 200; i++)
		this->res2[i] = g.res2[i];
	if (g.obj_name != nullptr)
	{
		this->obj_name = new char[strlen(g.obj_name) + 1];
		strcpy(this->obj_name, g.obj_name);
	}
	else
	{
		this->obj_name = nullptr;
	}
}

getCmdType getCmdType::operator=(const getCmdType & g)
{
	this->input = g.input;
	this->syntaxCreateT = g.syntaxCreateT;
	this->count_cols = g.count_cols;
	for (int i = 0; i < 200; i++)
		this->res2[i] = g.res2[i];
	if (g.obj_name != nullptr)
	{
		delete[] this->obj_name;
		this->obj_name = new char[strlen(g.obj_name) + 1];
		strcpy(this->obj_name, g.obj_name);
	}
	else
	{
		this->obj_name = nullptr;
	}
	return *this;
}

getCmdType::~getCmdType()
{
	delete[] this->obj_name;
	//delete[] this->cols_order;
}

string getCmdType::operator[](int pos)
{
	if (pos > 0 && pos < 200)
		return this->res2[pos];
	else throw exception("Invalid index.");
}

getCmdType getCmdType:: operator+(string x)
{
	getCmdType rez = *this;
	rez.input += x;
	return rez;
}

getCmdType& getCmdType::operator++() 
{
	int k = 0;
	while (res2[k] != "")
	{
		k = k + 2;
	}
	if (k >= 2)
	{
		res2[k] = res2[k - 2];
	}
	return *this;
}

getCmdType getCmdType::operator++(int)
{
	getCmdType rez = *this;
	int k = 0;
	while (rez.res2[k] != "")
	{
		k = k + 2;
	}
	if (k >= 2)
	{
		rez.res2[k] = rez.res2[k - 2];
	}
	return rez;
}

getCmdType::operator string()
{
	return this->input;
}

getCmdType getCmdType::operator!()
{
	getCmdType rez = *this;
	for (int i = 0; i < 200; i++)
		rez.res2[i] = "";
	rez.input = "";
	return rez;
}

bool getCmdType::operator<(const getCmdType& g)
{
	return this->input.size() < g.input.size();
}

bool getCmdType::operator==(const getCmdType& g)
{
	return this->input == g.input;
}

ostream& operator<<(ostream& out, const getCmdType& g)
{
	cout << "\nInput: " << g.input;
	cout << "Cols: " << g.count_cols;
	if (g.obj_name != nullptr)
	{
		cout << "Name: " << g.obj_name;
	}
	else cout << "Name: - ";
	return out;
}

istream& operator>>(istream& in, getCmdType& g)
{
	g.obj_name = new char[1024];
	cout << "\nInput: ";
	in.ignore();
	getline(in, g.input);
	cout << "Name: ";
	in.getline(g.obj_name, 1024, '\n');
	return in;
}

string getCmdType::getInput()
{
	return this->input;
}

int getCmdType::getNrCols()
{
	return this->count_cols;
}

string getCmdType::getName()
{
	string res;
	if (obj_name != nullptr)
		res = obj_name;
	else res = "No name.";
	return res;
}

void getCmdType::setInput(string input)
{
	this->input = input;
}

void getCmdType::setNrCols(int nrC, string input)
{
	if (nrC >= 0)
		this->count_cols = nrC;
	this->input = input;
}

void getCmdType::setName(const char* name)
{
	if (name != nullptr)
	{
		delete[] this->obj_name;
		this->obj_name = new char[strlen(name) + 1];
		strcpy(this->obj_name, name);
	}
}



bool getCmdType::verifyCreateTable()
{
	if (res2[0] == "CREATE" && res2[2] == "TABLE")
	{
		syntaxCreateT = true;
	}
	else if (res2[0] != "CREATE" || res2[2] != "TABLE")
	{
		syntaxCreateT = false;
	}
	bool check_Nothing_After = Utility::check_Nothing_After(this->input);
	if (check_Nothing_After == false)
	{
		syntaxCreateT = false;
	}
	bool ok = Utility::checkParenthesis(this->input);
	if (ok == false)
	{
		syntaxCreateT = false;
		cout << " Parantezele nu sunt puse cum trebuie." << endl;
	}
	if (count_cols == 0)
	{
		syntaxCreateT = false;
	}
	int l = 6;
	for (int i = 0; i < Utility::countPars(input) / 2 - 1; i++)
	{
		if (res2[l + 6] == "")
			syntaxCreateT = false;
	}
	if (Utility::checkParsOrder(input) == false)
	{
		syntaxCreateT = false;
	}

	return syntaxCreateT;
}

Table* getCmdType::createCols()
{
	string col_name;
	string type;
	int max_dim = 0;
	string text_def_val;
	float def_val = 0;
	int z = Utility::countPars(input) / 2 - 1;
	int x = z;
	int t = z;
	int i = 6;
	int k = 0;
	bool ok2 = false;
	bool ok4 = true;
	bool IdenticalNames = false;
	int n = 0;
	set<string> col_names; // in ideea ca nu se pot crea doua coloane cu acelasi nume
	
	Column** cols = new Column * [this->count_cols];
	int l = 6;
	while (z > 0)
	{
		if (res2[l + 2] != "FLOAT" && res2[l + 2] != "INTEGER" && res2[l + 2] != "TEXT")
			ok4 = false;
		l = l + 8;
		z--;
	}
	// aici, daca col_name se afla deja in set, sa nu se mai creeze tabela
	if (ok4 == true)
	{
		while (x > 0)
		{
			col_name = res2[i];
			type = res2[i + 2];

			max_dim = stoi(res2[i + 4]);
			if (type == "FLOAT" || type == "INTEGER")
			{
				if ((Table::isNumber(res2[i + 6]) == 0))
					ok2 = false;
				else if ((Table::isNumber(res2[i + 6]) == 1))
				{
					def_val = stof(res2[i + 6]);
					ok2 = true;
					n = Utility::getNrDigits(def_val);
					if (n > max_dim) // daca valoarea default are dim mai mare decat maximul admis
						ok2 = false;
				}
				if (ok2 == false)
					break;
			}
			else if (type == "TEXT")
			{
				if ((Table::isNumber(res2[i + 6]) == 1))
					ok2 = false;
				else if ((Table::isNumber(res2[i + 6]) == 0))
				{
					ok2 = true;
					text_def_val = res2[i + 6];
					if (text_def_val.length() > (unsigned)max_dim) // daca valoarea default are dim mai mare decat maximul admis
					{
						ok2 = false;
					}
				}
				if (ok2 == false)
					break;
			}

			if (col_names.find(col_name) != col_names.end())
			{
				IdenticalNames = true;
			}
			else
			{
				col_names.insert(col_name);
			}


			if (ok2 == true)
			{
				if (type == "TEXT")
				{
					type = "text";
					cols[k++] = new Column(col_name.c_str(), type, max_dim, text_def_val);
				}
				else if (type == "FLOAT" || type == "INTEGER")
				{
					type = "number";
					cols[k++] = new Column(col_name.c_str(), type, max_dim, def_val);
				}

			}
			else cout << "Exista erori.(Sugestii: valoarea si tipul coloanei nu se potrivesc; valoarea default are dimensiunea mai mare decat maximul admis)." << endl;
			x--;
			i = i + 8;
		}
	}


	if (ok2 == true && IdenticalNames == false)
	{
		Table* t = new Table(res2[4].c_str(), cols, this->count_cols);
		return t;
	}
	else
	{
		delete[] cols;
		throw exception("Reintroduceti comanda.\n");
	}

}

void getCmdType::displayTable(Database& db)
{
	string table_name;
	bool ok = false;
	bool found = false;
	if (this->res2[0] == "DISPLAY" && this->res2[2] == "TABLE")
	{
		ok = true;
	}
	// IF res2[4] se regaseste in vreun tabel din DB, found = true
	// daca nu, found = false si apare mesaj de eroare
	if (ok == true)
	{
		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
			if ((*itr)->getTableName() == res2[4])
			{
				db.displayTable(this->res2[4]);
				found = true;
				break;
			}
		if (found == false)
			cout << "Tabelul ce se doreste a fi afisat nu exista." << endl;
	}
	else cout << "Sintaxa incorecta pentru DISPLAY TABLE." << endl;

}

void getCmdType::dropTable(Database& db)
{
	bool ok = false;
	bool found = false;
	if (this->res2[0] == "DROP" && this->res2[2] == "TABLE")
	{
		ok = true;
	}
	if (ok == true)
	{
		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
			if ((*itr)->getTableName() == res2[4])
			{
				db.dropTable(this->res2[4]);
				found = true;
				cout << "Tabel sters cu succes!" << endl;
				break;
			}
		if (found == false)
			cout << "Tabelul ce se doreste a fi sters nu exista." << endl;
	}
	else cout << "Sintaxa incorecta pentru DROP TABLE." << endl;

}

bool getCmdType::InsertVerify(Database& db)
{
	bool ok = false;
	if (res2[0] == "INSERT" && res2[2] == "INTO" && res2[6] == "VALUES")
	{
		ok = true;
	}
	size_t cnt_pars = Utility::countPars(this->input);
	if (cnt_pars > 2)
		ok = false;
	bool good_commas = Utility::check_Not_Double_Commas(this->input);
	if (good_commas == false)
		ok = false;
	size_t nr_commas = count(this->input.begin(), this->input.end(), ',');
	list<Table*>::iterator itr;
	for (itr = db.tables.begin(); itr != db.tables.end(); itr++)
	{
		string tabN = (*itr)->getTableName();
		Utility::toUpper(tabN);
		if (tabN == res2[4])
			break;
	}
	if (nr_commas + 1 != (*itr)->getNrCols()) // we know that the number of commas in insert is number of columns - 1
		ok = false;
	if (Utility::check_Nothing_After_Ins(this->input) == false)
		ok = false;
	return ok;
}

void getCmdType::insertInto(Database& db, string* arr)
{
	bool ok = false;
	int no_of_cols = 0;
	bool found = false;
	bool more_args = false;
	int k = 0;
	int j = 0;
	// pentru tabelul X din DB trebuie sa ii iau numarul de coloane
	for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
	{
		if ((*itr)->getTableName() == res2[4]) // Gasim tabelul unde vrem sa inseram valori
		{
			found = true;
			this->return_pos = j;
			no_of_cols = (*itr)->getNrCols(); // ii stocam numarul de coloane
			break;
		}
		j++;
	}
	if (found == true)
	{
		string* str_vals = new string[no_of_cols];
		if (arr[no_of_cols] != "")
			more_args = true;
		if (more_args == false)
		{
			if (no_of_cols > 0)
			{
				for (int i = 0; i < no_of_cols; i++)
				{
					str_vals[i] = arr[k];
					k += 1; // valorile din Values(..) vor aparea din 1 in 1 in stringul arr
				}
				db.insertInto(res2[4], str_vals, db.getNrTables()); // res2[4] este nume tabel 
			}
		}
		else cout << "Se doreste inserarea a mai multor argumente decat coloane." << endl;
	}
	else cout << "Tabelul pentru care se doreste inserarea nu exista." << endl;

}

// SELECT ALL FROM NUME_TABEL
void getCmdType::selectALL(Database& db)
{
	bool found = false;
	if (res2[0] == "SELECT" && res2[2] == "ALL" && res2[4] == "FROM")
	{
		for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
			if ((*itr)->getTableName() == res2[6])
			{
				db.selectALL(res2[6]);
				found = true;
			}
		if (found == false)
			cout << "Tabelul ale calui coloane se doreste afisarea nu exista." << endl;
	}
	else cout << "Sintaxa gresita pentru selectAll. " << endl;
}

void getCmdType::deleteFrom(Database& db)
{

	bool found = false;
	int i = 0;
	if (this->res2[0] == "DELETE" && this->res2[2] == "FROM" && this->res2[6] == "WHERE")
	{
		if (Utility::checkEqualSign(input) == true)
		{
			for (auto itr = db.tables.begin(); itr != db.tables.end(); itr++)
				if ((*itr)->getTableName() == res2[4])
				{
					return_pos = i;
					db.delFrom(res2[4], res2[8], res2[10]);
					found = true;
				}
			if (found == false)
				cout << "Tabelul pentru care se doreste stergerea unei valori nu exista." << endl;
			i++;
		}
		else cout << "Sintaxa gresita pentru deleteFrom. Asigurati-va ca ati pus spatii peste tot." << endl;

	}
	else cout << "Sintaxa gresita pentru deleteFrom." << endl;

}

//4.	UPDATE nume_tabela SET nume_coloană = valoare WHERE nume_coloană = valoare (coloana SET poate fi diferită de cea WHERE; UPDATE merge doar pentru o coloana)
	// UPDATE STUDENT SET INALTIME = 50 WHERE INALTIME = 10
bool getCmdType::Update(Database& db)
{
	bool updated = false;
	//res2[2] = TABEL; res2[6] = nume-col res[8] = replacement res[12] = col res[14] = where_val
	if (res2[0] == "UPDATE" && res2[4] == "SET" && res2[10] == "WHERE")
	{
		if (res2[6] == res2[12])
		{
			if (Utility::checkEqualSignUpdate(input) == true)
			{

				updated = db.Update(res2[2], res2[6], res2[14], res2[8]);
			}
			else cout << "Sintaxa incorecta pentru Update." << endl;
		}
		else if (res2[6] != res2[12])
			cout << "Coloana SET nu se potriveste cu coloana Where!." << endl;
	}
	else cout << "Sintaxa incorecta pentru Update." << endl;
	return updated;
}

int getCmdType::getCountCols()
{
	return this->count_cols;
}
