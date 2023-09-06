#include "utils.h"
#pragma warning(disable:4996)
string Utility::ws2s(const wstring& wstr)
{
	using convert_typeX = codecvt_utf8<wchar_t>;
	wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
string Utility::getPath()
{
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	wstring x(&pwd[0]);
	string res = Utility::ws2s(x);
	return res;
}
void Utility::reverseSlashes(string& x)
{
	for (int i = 0; i < x.size(); i++)
	{
		if (x[i] == '\\')
		{
			x[i] = '//';
		}
	}
}

wstring Utility::s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


void Utility::getFileNames(vector<string>* fileList)
{
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
	string path = getPath() + "\\TABEL_*";
	reverseSlashes(path);
	wstring wstrpath = s2ws(path);
	LPCWSTR pth = wstrpath.c_str();
	HANDLE handle = FindFirstFile(pth, &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		string s = ws2s(wstring(search_data.cFileName));
		//std::cout << "Found file: " << s << std::endl;

		/* Here, you append another found file to the end of the vector.
		 * However, unlike arrays, you do NOT have to worry about resizing.
		 * The vector is resized for you!
		 */
		fileList->push_back(s);

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	/* Because you are passing vector as pointer to function, there is
	 * nothing to return, thus, we don't have to:
	 *   (1) write all the file names to a vector
	 *   (2) copy that vector to a new vector
	 * You'll just end up doing number 1.
	 */
}

int Utility::getLastSelectNum()
{
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
	string path = getPath() + "\\SELECT_*";
	reverseSlashes(path);
	wstring wstrpath = s2ws(path);
	LPCWSTR pth = wstrpath.c_str();
	HANDLE handle = FindFirstFile(pth, &search_data);
	vector<string> List;
	int num = 0;
	int max = INT_MIN;
	while (handle != INVALID_HANDLE_VALUE)
	{
		string s = ws2s(wstring(search_data.cFileName));
		List.push_back(s);
		
		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}
	for (int i = 0; i < List.size(); i++)
	{
		List[i] = List[i].substr(7);
		stringstream val(List[i]);
		val >> num;
		if (num > max)
			max = num;
	}
	return max;
	
}

int Utility::getNrDigits(float num)
{
	int nr_digits = 0;
	int n = (int)num;
	while (n > 0)
	{
		nr_digits++;
		n = n / 10;
	}
	return nr_digits;
}

bool Utility::checkEqualSign(string input)
{
	// trebuie sa vedem daca dupa where se afla operator=
	// DELETE FROM STUDENT WHERE INALTIME=50
	bool step = true;
	string str;
	size_t pos = input.find("=");
	size_t poscmp = input.find("WHERE");
	size_t countEquals = count(input.begin(), input.end(), '=');
	if (countEquals > 1)
		return false;
	int count = 0;
	if (pos != string::npos && poscmp != string::npos)
	{
		if (pos > 0 && pos < input.size() && poscmp > 0 && poscmp < input.size())
		{
			if (pos < poscmp + 6)
				step = false;
		}
		else if (pos < 0 || pos > input.size())
			step = false;
		if (pos == input.size() - 1)
			step = false;
	}
	else step = false;
	// trebuie verificat ca = nu e pe ultima pozitie
	return step;
}


bool Utility::checkEqualSignUpdate(string input)
{
	// trebuie sa vedem daca dupa where si inaltime (nume_coloana) se afla operator=
	// UPDATE COMENZI SET Inaltime=50 WHERE Inaltime=100
	bool step = false;
	string str;
	size_t pos = input.find("=");
	size_t poscmp = input.find("SET");
	size_t savepos = 0;
	size_t countEquals = count(input.begin(), input.end(), '=');
	if (countEquals > 2)
		return false;
	int count = 0;
	if (pos >= 13 && pos < input.size() && poscmp > 9 && poscmp < input.size())
	{
		if (pos < poscmp)
			step = false;
		else
		{
			savepos = pos;
			count++;
		}
	}
	bool ok = false;
	size_t findWhere = input.find("WHERE");
	if (findWhere >= 6 && findWhere < input.size())
	{
		str = input.substr(findWhere);
		for (unsigned int i = 0; i < input.size(); i++)
		{
			if (input[i] == '=' && i != savepos) // daca gaseste alt "=" in afara de primul
			{
				ok = true;
			}
		}
		if (ok == true)
		{
			pos = str.find("=");
			if (pos > 0 && pos < str.size())
			{
				if (pos > 6)
					count++;
				else
					step = false;
			}
		}

	}
	if (count == 2)
		step = true;
	
	return step;
}

int Utility::countPars(string& str)
{
	int cnt = 0;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(' || str[i] == ')')
			cnt++;
	}
	return cnt;
}

bool Utility::checkParenthesis(string str)
{
	string res;
	int count_pars = 0;
	stack<char> st;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(' || str[i] == ')')
		{
			res += str[i];
			count_pars++;
		}
	}
	if (count_pars < 4)
		return false;
	
	for (unsigned int i = 0; i < res.size(); i++)
	{
		if (res[i] == '(')
		{
			st.push(res[i]);
		}
		else
		{
			if (!st.empty() && st.top() == '(')
			{
				st.pop();
			}
			else
			{
				return false;
			}
		}
	}
	return st.empty();

}

bool Utility::checkParenthesisInsert(string str)
{
	string res;
	int count_pars = 0;
	stack<char> st;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(' || str[i] == ')')
		{
			res += str[i];
			count_pars++;
		}
	}
	if (count_pars < 2)
		return false;

	for (unsigned int i = 0; i < res.size(); i++)
	{
		if (res[i] == '(')
		{
			st.push(res[i]);
		}
		else
		{
			if (!st.empty() && st.top() == '(')
			{
				st.pop();
			}
			else
			{
				return false;
			}
		}
	}
	return st.empty();

}

string Utility::formStringInsert(string str)
{
	int countchars = 0;
	size_t pos = -1;
	string in;
	string res;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (str[i] != ' ')
			in += str[i];
	}
	if ((pos = in.find("VALUES(")))
	{
		pos = in.find("VALUES(");
		//size_t end_pos = in.find(")");
		string s = in.substr(pos);
		for (size_t i = pos + 6; i < in.size(); i++)
		{
			if (in[i] != ')')
				countchars++;
		}
		if (countchars > 0)
		{
			unsigned int len = countchars - 1;
			res = in.substr(pos + 7, len);
		}
		 // DIN INSERT INTO STUDENT VALUES(5, 10, csie) SA RAMANEM DOAR CU 5,10, csie
		return res;
	}
	else throw exception("Nu se apeleaza instructiunea INSERT INTO.");
}

string* Utility::regexInsert(string str)
{
	string* arr = new string[20];
	regex words_regex("[^,;]+");
	auto words_begin = sregex_iterator(str.begin(), str.end(), words_regex);
	auto words_end = sregex_iterator();
	int j = 0;
	for (sregex_iterator i = words_begin; i != words_end; ++i)
	{
		arr[j++] = (*i).str();
	}
	return arr;
}

bool Utility::checkParsOrder(string in)
{ // vrem sa vedem daca (( sau )) sunt unele dupa altele => primele 2 si ultimele 2 
	string str;
	string str2[2];
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (in[i] != ' ')
			str += in[i];
	}
	bool ok = true;
	size_t k = 0;
	int count = 0;
	int j = 0;
	while (str[k] != '(' && k + 2 < str.size()) // Pentru primele 2 paranteze; probabil era o alta cale mai usoara
	{
		if (str[k + 1] == '(')
		{
			str2[j++] = str[k + 1];
		}
		if (str[k + 2] == '(')
		{
			str2[j] = str[k + 2];
		}
		k++;
	}
	for (int i = 0; i < 2; i++)
	{
		if (str2[i] != "(")
			ok = false;
	}

	k = str.length() - 1; // Pentru ultimele 2 paranteze
	if (str[k] == ')' && str[k - 1] != ')')
		ok = false;

	return ok;
}

bool Utility::file_exists(const string& name)
{
	if (FILE* file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else return false;
}

int Utility::calcNrCmds1(string fileName) // calculeaza cate comenzi avem intr-un fisier text 
{
	// deschidem fisierul prima data
	FILE* f = fopen(fileName.c_str(), "r");
	int nrCmds = 1;
	if (f != nullptr)
	{

		char c;
		for (c = getc(f); c != EOF; c = getc(f))
		{
			if (c == '\n')
				nrCmds++;
		}
		fclose(f);
	}
	else nrCmds = 0;
	return nrCmds;

}

string* Utility::getArgCmds(int nrCmds, string fileName)
{
	// ok, deci, eu primesc tot fisierul
	// pot sa creez doar un tabel pe rand
	// pun cate o comanda CREATE TABLE pe o pozitie intr-un vector de string
	// si mai apoi trimit vectorul ala de stringuri in main, pe fiecare pozitie 
	ifstream f(fileName, ios::in);
	string* cmds = new string[nrCmds];
	int k = 0;
	if (f.good())
	{
		while (getline(f, cmds[k]))
		{
			k++;
		}
	}
	return cmds;
}

string Utility::toLower(string& original_str)
{
	string new_str = "";
	for (int i = 0; i < original_str.size(); i++)
	{
		new_str += tolower(original_str[i]);
	}
	return new_str;
}

void Utility::toUpper(string& original_str)
{
	string new_str = "";
	for (int i = 0; i < original_str.size(); i++)
	{
		new_str += toupper(original_str[i]);
	}
	original_str = new_str;
}

bool Utility::findSep(string str)
{
	bool returnVal = false;
	string separators= ":/.;[]()*&^%$#@!`?<>-_+=";
	for (int i = 0; i < str.size(); i++)
	{
		if (separators.find(str[i]) != string::npos)
			returnVal = true;
	}
	return returnVal;
}

bool Utility::check_Nothing_After(string str)
{
	string cmpStr;
	size_t find = str.find("))");
	if(find != string::npos)
		cmpStr= str.substr(find);
	if (cmpStr == "))")
		return true;
	else return false;
}

bool Utility::check_Not_Double_Commas(string str)
{
	size_t n = count(str.begin(), str.end(), ',');
	string cmpStr = str;
	bool ok = true;
	size_t find;
	while (n > 0)
	{
		find = cmpStr.find(",");
		if (find != string::npos)
		{
			cmpStr = cmpStr.substr(find);
			if (cmpStr[0] == ',' && cmpStr[1] == ',')
			{
				ok = false;
			}
		}
		cmpStr = cmpStr.substr(1);
		n--;
	}
	return ok;
}

bool Utility::check_Nothing_After_Ins(string str)
{
	string cmpStr;
	size_t find = str.find(")");
	if (find != string::npos)
		cmpStr = str.substr(find);
	if (cmpStr == ")")
		return true;
	else return false;
}


