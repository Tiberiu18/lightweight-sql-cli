#include "../Headers/Column.h"
#pragma warning(disable:4996)
using namespace std;


Column::Column()
{
}



Column::Column(const char* col_name,string type, int max_dim, float default_val)
	: max_dim(max_dim), default_val(default_val), type(type)

{
	if (col_name != NULL)
	{
		this->col_name = new char[strlen(col_name) + 1];
		strcpy(this->col_name, col_name);
	}
	else
	{
		this->col_name = new char[strlen("Nume Coloana Necunoscut") + 1]; 
		strcpy(this->col_name, "Nume Coloana Necunoscut");
	}
	
}

Column::Column(const char* col_name, string type, int max_dim, string default_val)
	: max_dim(max_dim), text_def_val(default_val), type(type)

{
	if (col_name != NULL)
	{
		this->col_name = new char[strlen(col_name) + 1];
		strcpy(this->col_name, col_name);
	}
	else
	{
		this->col_name = new char[strlen("Nume Coloana Necunoscut") + 1]; 
		strcpy(this->col_name, "Nume Coloana Necunoscut");
	}

}


Column::Column(const Column& c)
	: max_dim(c.max_dim), type(c.type)
{
	if (c.col_name != NULL)
	{
		this->col_name = new char[strlen(c.col_name) + 1];
		strcpy(this->col_name, c.col_name);
	}
	else
	{
		this->col_name = new char[strlen("Nume Coloana Necunoscut") + 1];
		strcpy(this->col_name, "Nume Coloana Necunoscut");
	}
	if (c.type == "text")
	{
		this->text_def_val = c.text_def_val;
		if (c.rows > 0 && c.text_vals != nullptr)
		{
			this->rows = c.rows;
			this->text_vals = new string[c.rows];
			for (int i = 0; i < c.rows; i++)
				this->text_vals[i] = c.text_vals[i];
		}
	}
	else if (c.type == "number")
	{
		this->default_val = c.default_val;
		if (c.rows > 0 && c.vals != nullptr)
		{

			this->rows = c.rows;
			this->vals = new float[c.rows];
			for (int i = 0; i < c.rows; i++)
				this->vals[i] = c.vals[i];
		}
	}
}



void Column::writeColumnDetails(fstream& f)
{
	//Scriem numele coloanei
	size_t lg_name = strlen(this->col_name) + 1;
	f.write((char*)&lg_name, sizeof(int));
	f.write(col_name, lg_name);
	
	//Scriem tipul coloanei - type e text sau number
	size_t lg_type = this->type.size() + 1;
	f.write((char*)&lg_type, sizeof(int));
	f.write(this->type.data(), lg_type);

	// Scriem dimensiunea maxima
	f.write((char*)&this->max_dim, sizeof(int));

	// Scriem valoare default
	if (this->type == "text")
	{
		size_t lg_def = this->text_def_val.size() + 1;
		f.write((char*)&lg_def, sizeof(int));
		f.write(this->text_def_val.data(), lg_def);
	}
	else
	{
		f.write((char*)&this->default_val, sizeof(float));
	}
	

}

void Column::readColumnDetails(fstream& f)
{
	//Citesc numele coloanei
	int lg_name = 0;
	f.read((char*)&lg_name, sizeof(int)); // lungimea numelui coloanei
	this->col_name = new char[lg_name];
	f.read(col_name, lg_name);

	// Tipul coloanei
	int lg_type = 0;
	f.read((char*)&lg_type, sizeof(int));
	char* buffer = new char[lg_type];
	f.read(buffer, lg_type);
	this->type = buffer;
	delete[] buffer;
	
	//Dimensiunea maxima
	this->max_dim = 0;
	f.read((char*)&this->max_dim, sizeof(int));
	//Valoarea default
	if (this->type == "text")
	{
		int lg_def = 0;
		f.read((char*)&lg_def, sizeof(int));
		buffer = new char[lg_def];
		f.read(buffer, lg_def);
		this->text_def_val = buffer;
	}
	else
		f.read((char*)&this->default_val, sizeof(float));
	

	delete[] buffer;
}

void Column::writeColumnRows(fstream& f)
{
	// Scriem numarul de inregistrari, inregistrarile coloanei, daca exista
	f.write((char*)&this->rows, sizeof(int));
	if (this->rows > 0)
	{
		if (this->type == "number")
		{
			for (int i = 0; i < this->rows; i++)
				f.write((char*)&vals[i], sizeof(float));
		}
		else if (this->type == "text")
		{
			for (int i = 0; i < this->rows; i++)
			{
				size_t lg_textRow = this->text_vals[i].size() + 1;
				f.write((char*)&lg_textRow, sizeof(int));
				f.write(this->text_vals[i].data(), lg_textRow);
			}
		}
	}
}


void Column::writeAll(fstream& f)
{
	/*Column::writeColumnDetails(f);
	Column::writeColumnRows(f);*/


	
	//Scriem numele coloanei
	size_t lg_name = strlen(this->col_name) + 1;
	f.write((char*)&lg_name, sizeof(int));
	f.write(col_name, lg_name);

	//Scriem tipul coloanei - type e text sau number
	size_t lg_type = this->type.size() + 1;
	f.write((char*)&lg_type, sizeof(int));
	f.write(this->type.data(), lg_type);

	// Scriem dimensiunea maxima
	f.write((char*)&this->max_dim, sizeof(int));

	// Scriem valoare default
	if (this->type == "text")
	{
		size_t lg_def = this->text_def_val.size() + 1;
		f.write((char*)&lg_def, sizeof(int));
		f.write(this->text_def_val.data(), lg_def);
	}
	else
	{
		f.write((char*)&this->default_val, sizeof(float));
	}
	 
	// Scriem numarul de inregistrari, inregistrarile coloanei, daca exista
	f.write((char*)&this->rows, sizeof(int));
	if (this->rows > 0)
	{
		if (this->type == "number")
		{
			for (int i = 0; i < this->rows; i++)
				f.write((char*)&vals[i], sizeof(float));
		}
		else if (this->type == "text")
		{
			for (int i = 0; i < this->rows; i++)
			{
				size_t lg_textRow = this->text_vals[i].size() + 1;
				f.write((char*)&lg_textRow, sizeof(int));
				f.write(this->text_vals[i].data(), lg_textRow);
			}
		}
	}
}


void Column::readAll(fstream& f)
{
	//Column::readColumnDetails(f);
	//Citim nr. de inregistrari

	//Citesc numele coloanei
	int lg_name = 0;
	f.read((char*)&lg_name, sizeof(int)); // lungimea numelui coloanei
	this->col_name = new char[lg_name];
	f.read(col_name, lg_name);

	// Tipul coloanei
	int lg_type = 0;
	f.read((char*)&lg_type, sizeof(int));
	char* buffer = new char[lg_type];
	f.read(buffer, lg_type);
	this->type = buffer;
	delete[] buffer;

	//Dimensiunea maxima
	this->max_dim = 0;
	f.read((char*)&this->max_dim, sizeof(int));
	//Valoarea default
	if (this->type == "text")
	{
		int lg_def = 0;
		f.read((char*)&lg_def, sizeof(int));
		buffer = new char[lg_def];
		f.read(buffer, lg_def);
		this->text_def_val = buffer;
		delete[] buffer;
	}
	else
		f.read((char*)&this->default_val, sizeof(float));




	f.read((char*)&this->rows, sizeof(int));
	if (this->rows > 0)
	{
		if (this->type == "number")
		{
			delete[] vals;
			vals = new float[this->rows];
			for (int i = 0; i < this->rows; i++)
				f.read((char*)&vals[i], sizeof(float));
		}
		else 
		{
			delete[] text_vals;
			text_vals = new string[this->rows];
			for (int i = 0; i < this->rows; i++)
			{
				int lg_textRow = 0;
				f.read((char*)&lg_textRow, sizeof(int));
				buffer = new char[lg_textRow];
				f.read(buffer, lg_textRow);
				this->text_vals[i] = buffer;
				delete[] buffer;
			}
		}
	}

}


Column& Column::operator=(const Column& c)
{
	if (this != &c)
	{
		delete[] this->vals;
		delete[] this->text_vals;
		delete[] this->col_name;
		this->max_dim = c.max_dim;
		this->type = c.type;
		if (c.col_name != NULL)
		{
			this->col_name = new char[strlen(c.col_name) + 1];
			strcpy(this->col_name, c.col_name);
		}
		else
		{
			this->col_name = new char[strlen("Nume Coloana Necunoscut") + 1];
			strcpy(this->col_name, "Nume Coloana Necunoscut");
		}
		if (c.type == "text")
		{
			this->text_def_val = c.text_def_val;
			if (c.rows > 0 && c.text_vals != nullptr)
			{
				this->rows = c.rows;
				this->text_vals = new string[c.rows];
				for (int i = 0; i < c.rows; i++)
					this->text_vals[i] = c.text_vals[i];
			}
			else if (c.rows == 0 || c.text_vals == nullptr)
			{
				this->rows = 0;
				this->text_vals = nullptr;
			}
		}
		else if (c.type == "number")
		{
			this->default_val = c.default_val;
			if (c.rows > 0 && c.vals != nullptr)
			{

				this->rows = c.rows;
				this->vals = new float[c.rows];
				for (int i = 0; i < c.rows; i++)
					this->vals[i] = c.vals[i];
			}
			else if (c.rows == 0 || c.vals == nullptr)
			{
				this->rows = 0;
				this->vals = nullptr;
			}
		}
	}
	return *this;
}

float& Column::operator[](int pos)
{
	if (pos >= 0 && this->rows > 0)
	{
		if (this->vals != nullptr)
			return vals[pos];
		else throw exception("Vectorul de valori float este null.");
	}
	else throw exception("Out of range index for float operator[] overload");
}

Column Column::operator+(int val)
{
	Column rez = *this;
	rez.max_dim += val;
	return rez;
}

Column::operator int()
{
	return this->max_dim;
}


//++c1
Column& Column::operator++()
{
	this->max_dim++;
	return *this;
}
//c1++
Column Column::operator++(int)
{
	Column rez = *this;
	this->max_dim++;
	return rez;
}

Column Column::operator!()
{

	Column rez = *this;
	rez.default_val = 0;
	rez.text_def_val = "";
	return rez;
}

bool Column::operator<(const Column& c)
{
	return this->rows < c.rows;
}

bool Column::operator==(const Column& c)
{
	return this->rows == c.rows;
}

Column::~Column()
{
	delete[] col_name;
	delete[] vals;
	delete[] text_vals;
	this->rows = 0;
}

string Column::getType()
{
	return this->type;
}

string Column::getColName()
{
	string name;

	name = this->col_name;
	return name;
}

string Column::getTextDefaultVal()
{
	return this->text_def_val;
}

float Column::getFloatDefaultVal()
{
	return this->default_val;
}

int Column::getRows()
{
	return this->rows;
}

int Column::getMaxDim()
{
	return this->max_dim;
}

float* Column::getNumValuesArr()
{
	return this->vals;
}


void Column::setColName(const char* name)
{
	if (name != NULL)
	{
		delete[] this->col_name;
		this->col_name = new char[strlen(name) + 1];
		strcpy(this->col_name, name);
	}
	else
	{
		cout << " Numele trimis este invalid. Nu se face nicio modificare.";
	}
}

void Column::setMax_dim(int max_dim)
{
	if (max_dim > 0)
		this->max_dim = max_dim;
	else
		cout << "Invalid value." << endl;
}

void Column::setRowsVals(int rows, float* vals)
{
	if (rows > 0 && vals != nullptr)
	{
		delete[] this->vals;
		this->rows = rows;
		this->vals = new float[rows];
		for (int i = 0; i < rows; i++)
			this->vals[i] = vals[i];
	}
	else
	{
		delete[] this->vals;
		this->rows = 0;
		cout << "Invalid values for setRowsVals." << endl;
	}
}

void Column::setRowsText_Vals(int rows, string* text_vals)
{
	if (rows > 0 && text_vals != nullptr)
	{
		delete[] this->text_vals;
		this->rows = rows;
		this->text_vals = new string[rows];
		for (int i = 0; i < rows; i++)
			this->text_vals[i] = text_vals[i];
	}
	else
	{
		delete[] this->text_vals;
		this->rows = 0;
		cout << "Invalid values for setRowsTextVals." << endl;
	}
}

void Column::setDefault_val(float default_val)
{
	if (default_val > 0)
		this->default_val = default_val;
	else
		cout << "Invalid default value given." << endl;
}

void Column::setText_def_val(string text_def_val)
{
	if (text_def_val != "")
		this->text_def_val = text_def_val;
	else
		cout << "Invalid default value given." << endl;
}






//.1.INSERT INTO nume_tabela VALUES(...); valorile sunt separate prin, și au numărul și ordinea exacta ca definiția tabelului
void Column::insertRowNumber(float value) // ok, deci trebuie sa inseram in array-ul vals de dim rows
{
	// o sa trebuiasca sa cream alt vector mai mare 
	bool ok = true;
	int nr_digits = 0;
	int n = int(value);
	while (n > 0)
	{
		nr_digits++;
		n = n / 10;
		if (nr_digits > this->max_dim) 
			ok = false;
	}
	if (ok == true)
	{
		if (this->vals != nullptr)
		{
			float* aux_arr = this->vals;
			unsigned int len = this->rows + 1;
			this->vals = new float[len]; // facem vectorul mai mare
			for (int i = 0; i < rows; i++)
				this->vals[i] = aux_arr[i];
			this->vals[rows] = value;
			this->rows = rows + 1;
			delete[] aux_arr;
		}
		else // daca o sa fie prima inregistrare
		{
			unsigned int len = this->rows + 1;
			this->vals = new float[len];
			this->vals[rows] = value;
			this->rows = rows + 1;
		}
		cout << "Valoare de tip number a fost introdusa cu succes!\n";
	}
	else cerr << " Valoarea trimisa are numar de cifre mai mare decat capacitatea coloanei " <<this->getColName() << endl;
}


void Column::insertRowText(string value)
{
	if (this->type.size() >= 3)
	{
		if (value.size() - 1 < (unsigned)this->max_dim)
		{
			if (this->text_vals != nullptr)
			{
				string* aux_str = this->text_vals;
				unsigned int len = this->rows + 1;
				this->text_vals = new string[len];
				for (int i = 0; i < rows; i++)
					this->text_vals[i] = aux_str[i];
				this->text_vals[rows] = value;
				this->rows = rows + 1;
				delete[] aux_str;
			}
			else
			{
				unsigned int len = this->rows + 1;
				this->text_vals = new string[len];
				this->text_vals[rows] = value;
				this->rows = rows + 1;
			}
			cout << "\nValoarea introdusa de tip text a fost introdusa cu succes!\n";
		}

		else cerr << " Text trimis are dimensiune mai mare decat capacitatea coloanei " << this->getColName() << endl;
	}
	else cout << "Coloanei nu i-a fost setat tipul." << endl;
}


void Column::insertMultipleNum(float* val_arr, int size)
{
	bool ok = true;
	int nr_digits = 0;
	int n;
	for (int i = 0; i < size; i++)
	{
		n = int(val_arr[i]);
		while (n > 0)
		{
			nr_digits++;
			n = n / 10;
			if (nr_digits > this->max_dim) // check if nr_digits of any vector element is bigger than maximum capacity of column
				ok = false;
		}
	}
	if (ok == true)
	{
		if (val_arr != nullptr && size > 0)
		{
			if (this->vals != nullptr)
			{
				float* aux_arr = this->vals;
				unsigned int len = this->rows + size;
				this->vals = new float[len];
				for (int i = 0; i < rows; i++)
					this->vals[i] = aux_arr[i];
				int k = 0;
				for (int j = rows; j < rows + size; j++)
				{
					this->vals[j] = val_arr[k++];
				}
				this->rows = rows + size;
				delete[] aux_arr;
			}
			else
			{
				this->vals = new float[size];
				for (int i = 0; i < size; i++)
					this->vals[i] = val_arr[i];
				this->rows = size;
			}
		}
		else
		{
			cerr << "Nu s-a putut face modificarea. Vector trimis sau lungime a vectorului trimisa incorect." << endl;
		}
	}
	else cerr << " Dimensiunea unui element din vector depaseste capacitatea maxima a coloanei " << this->getColName() << endl;
}


void Column::insertMultipleText(string* val_arr, int size)
{
	bool ok = true;
	int nr_digits = 0;
	string n;
	for (int i = 0; i < size; i++)
	{
		n = val_arr[i];
		if (n.size() - 1 > (unsigned)this->max_dim ) // check if any string from array has a bigger size than column's max capacity
		{
			cout << " SIZE: " << n.size() << endl;
			ok = false;
		}
		
	}
	if (ok == true)
	{
		if (this->text_vals != nullptr)
		{
			string* aux_str = this->text_vals;
			unsigned int len = this->rows + size;
			this->text_vals = new string[len ];
			for (int i = 0; i < rows; i++)
				this->text_vals[i] = aux_str[i];
			int k = 0;
			for (int j = rows; j < rows + size; j++)
				this->text_vals[j] = val_arr[k++];
			this->rows = rows + size;
			delete[] aux_str;
		}
		else
		{
			this->text_vals = new string[size];
			for (int i = 0; i < size; i++)
				this->text_vals[i] = val_arr[i];
			this->rows = size;
		}
	}
	else if ( ok == false) cerr << " Un string trimis are lungimea mai mare decat maximul admis al coloanei "  << this->getColName()  << endl;
}







//2.	DELETE FROM nume_tabela WHERE nume_coloană = valoare (DELETE permite doar o coloană în clauza WHERE)


void Column::NumDeleteFrom(float value) 
{
	int count = 0;
	for (int i = 0; i < this->rows; i++)
	{
		if (this->vals[i] == value)
			count++;
	}
	if (count > 0)
	{
		unsigned int len = rows - count;
		float* aux_arr = new float[len];
		int k = 0;
		for (int i = 0; i < rows; i++)
		{
			if (this->vals[i] != value)
				aux_arr[k++] = this->vals[i];
		}
		delete[] this->vals;
		this->vals = aux_arr;
		this->rows -= count;
		cout << "Valoare stearsa cu succes!" << endl;
	}
	else cout << " Nu s-au gasit inregistrari. "<<endl;
		
}

void Column::TextDeleteFrom(string value)
{
	int count = 0;
	for (int i = 0; i < this->rows; i++)
	{
		if (this->text_vals[i] == value)
			count++;
	}
	if (count > 0)
	{
		unsigned int len = rows - count;
		string* aux_arr = new string[len];
		int k = 0;
		for (int i = 0; i < rows; i++)
		{
			if (this->text_vals[i] != value)
				aux_arr[k++] = this->text_vals[i];
		}
		delete[] this->vals;
		this->text_vals = aux_arr;
		this->rows -= count;
		cout << "Valoare stearsa cu succes!" << endl;
	}
	else cout << " Nu s-au gasit inregistrari.\n";
}





void Column::UpdateText(string where_val, string replacement)
{
	bool replaced = false;
	if (where_val != replacement)
	{
		for (int i = 0; i < this->rows; i++)
		{
			if (this->text_vals[i] == where_val)
			{
				this->text_vals[i] = replacement;
				replaced = true;
				cout << "Valorile au fost inlocuite cu succes!" << endl;
			}
		}
	}
	else {
		cout << " Where value and Replacement value are identical. No replacements made." << endl;
		replaced = true;
	}
	if (replaced == false)
	{
		cout << " No value found to replace. " << endl;
	}
}

void Column::UpdateNum(float where_val, float replacement)
{
	bool replaced = false;
	if (where_val != replacement)
	{
		for (int i = 0; i < this->rows; i++)
		{
			if (this->vals[i] == where_val)
			{
				this->vals[i] = replacement;
				replaced = true;
				cout << "Valorile au fost inlocuite cu succes!" << endl;
			}
		}
	}
	else {
		cout << " Where value and Replacement value are identical. No replacements made." << endl;
		replaced = true;
	}
	if (replaced == false)
	{
		cout << " No value found to replace. " << endl;
	}
}


ostream& operator<<(ostream& OUT, const Column& col)
{
	if (col.col_name != nullptr)
		OUT << endl << "Nume coloana: " << col.col_name << endl;
	else OUT << "Coloana nu are nume";
	OUT << "Coloana este de tipul: " << col.type << endl;
	if (col.rows > 0)
	{
		OUT << "Valorile coloanei: " << endl;
		if (col.vals != nullptr)
		{
			for (int i = 0; i < col.rows; i++)
				OUT << "  " << col.vals[i] << endl;
		}
		else if (col.text_vals != nullptr)
		{
			for (int i = 0; i < col.rows; i++)
				OUT << "  " << col.text_vals[i] << endl;
		}

	}
	else
		OUT << "Nu exista nicio inregistrare in coloana " << col.col_name << "." << "\n";
	return OUT;
}


bool Column::isNumber(string myString) {
	std::istringstream iss(myString);
	float f;
	iss >> noskipws >> f; // noskipws considers leading whitespace invalid
	// Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

istream& operator>>(istream& in, Column& c)
{
	c.col_name = new char[1024];
	cout << "\nIntroduceti numele coloanei: ";
	in.ignore();
	in.getline(c.col_name, 1024, '\n');
	cout << "\nIntroduceti de ce tip va fi coloana( text sau number): ";
	in >> c.type;
	while (c.type != "text" && c.type != "number") 
	{
		cout << "\nIntroducere invalida. Introduceti din nou (text sau number): ";
		in >> c.type;
	}
	if (c.type == "text")
	{
		delete[] c.vals;
		delete[] c.text_vals;
		c.rows = 0;
		c.text_vals = nullptr;
		cout << "\nIntroduceti lungimea maxima a cuvintelor ce vor fi inserate pe randuri: ";
		in >> c.max_dim;
		cout << "\nIntroduceti valoarea default: ";
		in >> c.text_def_val;
		while (Column::isNumber(c.text_def_val) == 1)
		{
			cout << "\nIntroduceti o valoare de tipul coloanei(text sau string): ";
			in >> c.text_def_val;
		}
		
	}
	else if (c.type == "number")
	{
		delete[] c.vals;
		delete[] c.text_vals;
		c.rows = 0;
		cout << "\nIntroduceti dimensiunea maxima admisa pe randuri(cate cifre poate avea maxim): ";
		in >> c.max_dim;
		cout << "\nIntroduceti valoarea default: ";
		string s;
		in >> s;
		while (Column::isNumber(s) == 0)
		{
			cout << "\nIntroducere incorecta.Introduceti o valoare de tipul coloanei(text sau string): ";
			in >> s;
		}
		c.default_val = stof(s);
	}
	return in;
}

size_t Column::returnSize()
{
	size_t sz = sizeof(this) + strlen(this->col_name); // on the basis that 1 byte = 1 character
	if (this->type == "text")
	{
		for (int i = 0; i < rows; i++)
			sz += this->text_vals->size();
	}
	else if (this->type == "number")
	{
		sz += (size_t)rows * 4; // 4 bytes for a float or integer
	}
	return sz;
}