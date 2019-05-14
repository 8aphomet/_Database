#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <time.h>
#include <algorithm>
#include <windows.h>
using namespace std;

string PL(const char * text)
{
	std::string result;
	for (; *text; text++)
		switch (*text)
		{
		case 'π': result += static_cast <char>(0xa5); break;
		case 'Ê': result += static_cast <char>(0x86); break;
		case 'Í': result += static_cast <char>(0xa9); break;
		case '≥': result += static_cast <char>(0x88); break;
		case 'Ò': result += static_cast <char>(0xe4); break;
		case 'Û': result += static_cast <char>(0xa2); break;
		case 'ú': result += static_cast <char>(0x98); break;
		case 'ø': result += static_cast <char>(0xbe); break;
		case 'ü': result += static_cast <char>(0xab); break;
		case '•': result += static_cast <char>(0xa4); break;
		case '∆': result += static_cast <char>(0x8f); break;
		case ' ': result += static_cast <char>(0xa8); break;
		case '£': result += static_cast <char>(0x9d); break;
		case '—': result += static_cast <char>(0xe3); break;
		case '”': result += static_cast <char>(0xe0); break;
		case 'å': result += static_cast <char>(0x97); break;
		case 'Ø': result += static_cast <char>(0xbd); break;
		case 'è': result += static_cast <char>(0x8d); break;
		default: result += *text; break;
		}
	return result;
}

//Struktura z danymi o osobie
struct person
{
	string name;

	string surname;

	string pesel;

	string bplace;
};

//Struktura z danymi o urodzinach
struct bdate
{
	int day;
	
	int month;
	
	int year;
};

bdate bdate_from_pesel(string pesel)
{
	//81010200131
	//530.9.2809r.
	bdate date;

	int day = pesel[5] - '0' + 10 * (pesel[4] - '0');

	int month = pesel[3] - '0' + 10 * (pesel[2] - '0');
	if (month > 12)
		month %= 20;

	int year = pesel[1] - '0' + 10 * (pesel[0] - '0');
	if (pesel[2] == '8' || pesel[2] == '9')
		year += 1800;
	else if (pesel[2] == '0' || pesel[2] == '1')
		year += 1900;
	else if (pesel[2] == '2' || pesel[2] == '3')
		year += 2000;
	else if (pesel[2] == '4' || pesel[2] == '5')
		year += 2100;
	else
		year += 2200;

	date.day = day;
	date.month = month;
	date.year = year;

	return date;
}

void disp_under_10(int num)
{
	if (num <= 9 && num >= 0)
		cout << "0" << num;
	else
		cout << num;
}

void display_bdate(bdate date)
{
	disp_under_10(date.day);
	cout << ".";
	disp_under_10(date.month);
	cout << "." << date.year << "r." << endl;
}

//Wypisz konkretnπ osobÍ
void display_person(person p1)
{
	cout << PL("ImiÍ: ") << p1.name << endl << "Nazwisko: " << p1.surname << endl << "Data urodzenia: ";
	display_bdate(bdate_from_pesel(p1.pesel));
	cout << "Numer PESEL: " << p1.pesel << endl << "Miejsce urodzenia: " << p1.bplace << endl;
}

//Funkcja wspomagajπca do generatora numeru pesel, podobna do disp_under_10
void add_0_if_under_10(string &num)
{
	if (stoi(num) <= 9)
	{
		string newnum = "0";
		newnum += num;
		num = newnum;
	}
}

//Funkcja wspomagajaca do generatora numeru pesel, sprawdza czy rok jest przestepny
bool if_leap_year(int iyear, int imonth)
{
	int year = iyear;
	if (imonth > 80)
		year += 1800;
	else if (imonth > 60 && imonth < 80)
		year += 2200;
	else if (imonth > 40 && imonth < 60)
		year += 2100;
	else if (imonth > 20 && imonth < 40)
		year += 2000;
	else
		year += 1900;

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	else
		return false;
}

//Generator imienia
void names_load(vector<string> &names)
{
	ifstream plikgenname("Imiona.txt");
	string input = " ";
	char * ptr = &input[0];

	while (!plikgenname.eof())
	{
		plikgenname >> input;
		names.push_back(PL(ptr));
	}
}

string name_gen(vector<string> names)
{
	return names[rand() % names.size()];
}

//Generator nazwiska
void surnames_load(vector<string> &surnames)
{
	ifstream plikgensurname("Nazwiska.txt");
	string input = " ";
	char * ptr = &input[0];

	while (!plikgensurname.eof())
	{
		plikgensurname >> input;
		surnames.push_back(PL(ptr));
	}
}

string surname_gen(string name, vector<string> surnames)
{
	string surname = surnames[rand() % surnames.size()];

	if (name[name.length() - 1] == 'a' && surname[surname.length() - 1] == 'i')
		surname[surname.length() - 1] = 'a';

	return surname;
}

//Generator miejsca urodzenia
void bplaces_load(vector<string> &bplaces)
{
	ifstream plikgenbplace("Miasta.txt");
	string input = " ";
	char * ptr = &input[0];

	while (!plikgenbplace.eof())
	{
		getline(plikgenbplace, input);
		if (input != "")
		{
			bplaces.push_back(PL(ptr));
			ptr = &input[0];
		}
	}
}

string bplace_gen(vector<string> bplaces)
{
	return bplaces[rand() % bplaces.size()];
}

//Generator numeru pesel
string pesel_gen(string imie)
{
	//pesel ktory bedzie zwrocony
	string pesel = "";

	int iyear = 0;
	int imonth = 0;

	do
	{
		iyear = rand() % 99 + 1;
		imonth = rand() % 12 + 1 + (rand() % 2) * 20;
	} while ((imonth > 20 && iyear > 18));

	//dzien
	int iday;
	if (imonth % 20 <= 7)
	{
		if (imonth % 20 == 2)
		{
			if (if_leap_year(iyear, imonth))
				iday = rand() % 29 + 1;
			else
				iday = rand() % 28 + 1;
		}
		else if ((imonth % 20) % 2 == 1)
			iday = rand() % 31 + 1;
		else
			iday = rand() % 30 + 1;
	}
	else
	{
		if ((imonth % 20) % 2 == 1)
			iday = rand() % 30 + 1;
		else
			iday = rand() % 31 + 1;
	}

	//numer serii
	string nr_serii;
	for (int i = 0; i < 3; i++)
		nr_serii += to_string(rand() % 10);

	if (imie[imie.length() - 1] == 'a')
		nr_serii += to_string((rand() % 5) * 2);
	else
		nr_serii += to_string((rand() % 5) * 2 + 1);

	string year = to_string(iyear);
	string month = to_string(imonth);
	string day = to_string(iday);

	add_0_if_under_10(year);
	add_0_if_under_10(month);
	add_0_if_under_10(day);

	pesel += year;
	pesel += month;
	pesel += day;
	pesel += nr_serii;

	int ctrl_num = (9 * (pesel[0] - '0') + 7 * (pesel[1] - '0') + 3 * (pesel[2] - '0') + 1 * (pesel[3] - '0') + 9 * (pesel[4] - '0') + 7 * (pesel[5] - '0') + 3 * (pesel[6] - '0') + 1 * (pesel[7] - '0') + 9 * (pesel[8] - '0') + 7 * (pesel[9] - '0')) % 10;
	pesel += to_string(ctrl_num);

	return pesel;
}

//1: DODAWANIE POJEDYNCZEJ OSOBY
string keep_reading()
{
	string str;

	while (str.length() == 0)
		getline(cin, str);

	return str;
}

void add_person(vector<person> &people, bool settings[6], vector<string> names, vector<string> surnames, vector<string> bplaces)
{
	system("CLS");
	int b4size = people.size();

	person p1;
	
	cout << PL("Podaj informacje zgodnie z poniøszymi instrukcjami. Zatwierdü informacje enterem.") << endl << PL("Aby powrociÊ do menu bez dodawania nowej osoby, wpisz 0 i kliknij enter.") << endl << endl;

	//cin.ignore();
	
	//IMI 
	if (settings[0] == 0)
	{
		cout << PL("IMI : ");
		p1.name = keep_reading();
		//getline(cin, p1.name);

		if (p1.name == "0")
			return;
	}
	else
		p1.name = name_gen(names);
	
	//NAZWISKO
	if (settings[1] == 0)
	{
		cout << endl << "NAZWISKO: ";
		p1.surname = keep_reading();

		if (p1.surname == "0")
			return;
	}
	else
		p1.surname = surname_gen(p1.name, surnames);

	//PESEL
	if (settings[2] == 0)
	{
		cout << endl << "PESEL: ";
		while (p1.pesel.length() != 11 && p1.pesel != "0")
			p1.pesel = keep_reading();

		if (p1.pesel == "0")
			return;
	}
	else
		p1.pesel = pesel_gen(p1.name);

	//MIEJSCE URODZENIA
	if (settings[3] == 0)
	{
		cout << endl << "MIEJSCE URODZENIA: ";
		p1.bplace = keep_reading();
		//getline(cin, p1.bplace);

		if (p1.bplace == "0")
			return;
	}
	else
		p1.bplace = bplace_gen(bplaces);

	people.push_back(p1);

	if (people.size() == b4size + 1)
	{
		system("CLS");
		cout << PL("Pomyúlnie dodano osobÍ. Aby zapisaÊ dane do pliku, wybierz opcjÍ 7) z menu.") << endl << PL("Naciúnij dowolny przycisk aby kontynuowaÊ");
	}

	_getch();
}

void add_people(vector<person> &people, vector<string> names, vector<string> surnames, vector<string> bplaces)
{
	cout << PL("Ile osÛb chcesz wygenerowaÊ?") << endl;
	int odp;
	int b4size = people.size();
	cin >> odp;
	person input;

	for (int i = 0; i < odp; i++)
	{
		try
		{
			input.name = name_gen(names);
			input.surname = surname_gen(input.name, surnames);
			input.bplace = bplace_gen(bplaces);
			input.pesel = pesel_gen(input.name);
			people.push_back(input);
		}
		catch (bad_alloc& err)
		{
			cout << PL("Za ma≥o pamiÍci") << err.what() << endl;
		}
	}

	if (people.size() == b4size + odp)
	{
		system("CLS");
		cout << PL("Pomyúlnie dodano osoby. Aby zapisaÊ dane do pliku, wybierz opcjÍ 7) z menu.") << endl << PL("Naciúnij dowolny przycisk aby kontynuowaÊ");
	}

	_getch();
}

void adding_menu(vector<person> &people, vector<string> names, vector<string> surnames, vector<string> bplaces, bool settings[6])
{
	system("CLS");
	int b4size = people.size();

	if (settings[4] == 0)
	{
		add_person(people, settings, names, surnames, bplaces);
	}
	else if (settings[4] == 1)
	{
		cout << "1. Dodanie pojedynczej osoby" << endl;
		cout << PL("2. Wygenerowanie grupy osÛb") << endl;
		char odp = _getch();

		switch (odp)
		{
		case '1':
			add_person(people, settings, names, surnames, bplaces);
			break;
		case '2':
			add_people(people, names, surnames, bplaces);
			break;
		default:
			break;
		}
	}
}

//2: USUN KONKRETNA OSOBE
void range_err()
{
	cout << PL("B≥Ídny przedzia≥. Naciúnij dowolny przycisk aby kontynuowaÊ.");
	_getch();
}

void delete_person(vector<person> &people)
{
	system("CLS");
	int b4size = people.size();
	int person_id;
	cout << PL("Podaj ID osoby, ktÛrπ chcesz usunπÊ. Moøesz sprawdziÊ ID wybierajπc opcjÍ 6) w menu.") << endl;
	cout << PL("Aby powrÛciÊ do menu bez usuwania osoby, wpisz liczbÍ 0") << endl;

	cin >> person_id;

	if (person_id == 0)
		return;

	if (person_id >= 1 && person_id <= people.size())
	{
		people.erase(people.begin() + (person_id - 1));
	}
	else
		range_err();

	if (people.size() == b4size - 1)
	{
		system("CLS");
		cout << PL("Pomyúlnie usuniÍto osobÍ.") << endl << PL("Naciúnij dowolny przycisk aby kontynuowaÊ");
		_getch();
	}
}

void delete_people(vector<person> &people)
{
	system("CLS");
	int b4size = people.size();
	int min, max;
	cout << PL("Aby powrÛciÊ do menu bez usuwania osoby, wpisz liczbÍ 0") << endl << endl;
	cout << PL("Podaj ID osoby, od ktÛrej chcesz zaczπÊ usuwanie (w≥πcznie z tπ osobπ): ");
	cin >> min;

	if (min == 0)
		return;

	cout << PL("Podaj ID osoby, na ktÛrej chcesz zakoÒczyÊ usuwanie (w≥πcznie z tπ osobπ): ");
	cin >> max;

	if (min >= 1 && min <= people.size() && max >= 1 && max <= people.size() && max >= min)
	{
		for(int i = 0; i <= (max - min); i++)
			people.erase(people.begin() + (min - 1));
	}
	else
		range_err();

	if (people.size() == b4size - (max - min + 1))
	{
		system("CLS");
		cout << PL("Pomyúlnie usuniÍto osoby.") << endl << PL("Naciúnij dowolny przycisk aby kontynuowaÊ");
		_getch();
	}
}

void delete_menu(bool setting[6], vector<person> &people)
{
	system("CLS");

	if (setting[5] == 0)
	{
		delete_person(people);
	}
	else
	{
		cout << PL("1. UsuniÍcie konkretnej osoby") << endl << PL("2. UsuniÍcie przedzia≥u osÛb") << endl;
		char odp = _getch();

		switch (odp)
		{
		case '1':
			delete_person(people);
			break;
		case '2':
			delete_people(people);
			break;
		default:
			break;
		}
	}
}

//3: SORTUJ BAZE DANYCH
//PorÛwnanie alfabetyczne imienia
bool name_comparison(person p1, person p2)
{
	return p1.name < p2.name;
}

//PorÛwnanie alfabetyczne nazwiska
bool surname_comparison(person p1, person p2)
{
	return p1.surname < p2.surname;
}

//PorÛwnanie numeru pesel
bool pesel_comparison(person p1, person p2)
{
	return p1.pesel < p2.pesel;
}

//PorÛwnanie p≥ci
int sex_check(person p)
{
	//0: Kobiety; 1: MÍøczyüni
	if (int(p.pesel[9]) % 2 == 0)
		return 0;
	else
		return 1;
}

bool sex_comparison(person p1, person p2)
{
	return sex_check(p1) < sex_check(p2);
}

//PorÛwnanie alfabetyczne miejsca pochodzenia
bool bplace_comparison(person p1, person p2)
{
	return p1.bplace < p2.bplace;
}

//Wiekiem
bool age_comparison(person p1, person p2)
{
	return bdate_from_pesel(p1.pesel).year < bdate_from_pesel(p2.pesel).year;
}

void sort_data(vector<person> &people)
{
	system("CLS");

	cout << PL("Wybierz, jak chcesz posortowaÊ dane: ") << endl;
	cout << "(Wybierz numer na klawiaturze)" << endl << endl;
	cout << PL("0. PowrÛt do menu.") << endl;
	cout << PL("1. Alfabetycznie wed≥ug imienia.") << endl;
	cout << PL("2. Alfabetycznie wed≥ug nazwiska.") << endl;
	cout << PL("3. Wed≥ug p≥ci.") << endl;
	cout << PL("4. Alfabetycznie wed≥ug miejsca urodzenia") << endl;
	cout << "5. Wiekiem od najstarszego" << endl;

	char answer = _getch();

	switch (answer)
	{
	case '0':
		break;
	case '1':
	{
		sort(people.begin(), people.end(), name_comparison);
		system("CLS");
		cout << PL("Sortowanie powiod≥o siÍ.");
		_getch();
		break;
	}
	case '2':
	{
		sort(people.begin(), people.end(), surname_comparison);
		system("CLS");
		cout << PL("Sortowanie powiod≥o siÍ.");
		_getch();
		break;
	}
	/*
	case '3':
	{
		sort(people.begin(), people.end(), pesel_comparison);
		system("CLS");
		cout << PL("Sortowanie powiod≥o siÍ.");
		_getch();
		break;
	}
	*/
	case '3':
	{
		sort(people.begin(), people.end(), sex_comparison);
		system("CLS");
		cout << "Sortowanie powiodlo sie.";
		_getch();
		break;
	}
	case '4':
	{
		sort(people.begin(), people.end(), bplace_comparison);
		system("CLS");
		cout << PL("Sortowanie powiod≥o siÍ.");
		_getch();
		break;
	}
	case '5':
	{
		sort(people.begin(), people.end(), age_comparison);
		system("CLS");
		cout << PL("Sortowanie powiod≥o siÍ.");
		_getch();
		break;
	}
	default:
	{
		cout << PL("Niepoprawna liczba. Naciúnij dowolny przycisk, aby powrÛciÊ do menu.");
		_getch();
	}
	}
}

//4: Edycja danych
int data_edition(vector<person> &people)
{
	system("CLS");
	int person_id;
	cout << PL("Podaj ID osoby, ktÛrej dane chcesz zmieniÊ. Moøesz sprawdziÊ ID wybierajπc opcjÍ 6) w menu.") << endl;
	cout << PL("Aby powrÛciÊ do menu wpisz liczbÍ 0") << endl;

	cin >> person_id;

	if (person_id == 0)
		return 0;
	else
	{
		cout << "NUMER ID #" << person_id << endl;
		display_person(people[person_id - 1]);
	}

	cout << PL("KtÛre dane chcesz zmienic ?") << endl;
	cout << PL("1. ImiÍ") << endl;
	cout << "2. Nazwisko" << endl;
	cout << "3. Pesel" << endl;
	cout << "4. Miejsce urodzenia" << endl;

	char answer;
	answer = _getch();
	cout << endl;

	switch (answer)
	{
	case '1':
	{
		cout << PL("Nowe imiÍ: ");

		cin >> people[person_id - 1].name;
		break;
	}
	case '2':
	{
		cout << PL("Nowe nazwisko: ");
		cin >> people[person_id - 1].surname;
		break;
	}
	case '3':
	{
		cout << "Nowy numer pesel: ";
		cin >> people[person_id - 1].pesel;
		while (people[person_id - 1].pesel.length() != 11)
		{
			cout << PL("Niepoprawna dlugoúÊ numeru pesel. SprÛbuj ponownie.");
			cin >> people[person_id - 1].pesel;
		}
		break;
	}
	case '4':
	{
		cout << "Nowe miejsce urodzenia: ";
		cin >> people[person_id - 1].bplace;
		break;
	}

	cout << PL("Pomyúlnie zedytowano dane.") << endl;

	return 1;
	}
}

//5: Wyszukiwanie osoby
void cont()
{
	cout << endl << PL("Naciúnij cokolwiek, aby kontynuowaÊ");
}

void iff_found(bool iff)
{
	if (!iff)
		cout << "Brak osoby w bazie" << endl;
}

void find_by_name(vector<person> people, string name)
{
	system("CLS");
	bool if_found = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (people[i].name == name)
		{
			cout << "NUMER ID #" << i + 1 << endl;
			if_found = 1;
			display_person(people[i]);
			cout << endl;
		}
	}

	iff_found(if_found);
	cont();
	_getch();
}

void find_by_surname(vector<person> people, string surname)
{
	system("CLS");
	bool if_found = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (people[i].surname == surname)
		{
			cout << "NUMER ID #" << i + 1 << endl;
			if_found = 1;
			display_person(people[i]);
			cout << endl;
		}
	}

	iff_found(if_found);
	cont();
	_getch();
}

void find_by_pesel(vector<person> people, string pesel)
{
	system("CLS");
	bool if_found = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (people[i].pesel == pesel)
		{
			cout << "NUMER ID #" << i + 1 << endl;
			if_found = 1;
			display_person(people[i]);
			cout << endl;
		}
	}

	iff_found(if_found);
	cont();
	_getch();
}

void find_by_bplace(vector<person> people, string bplace)
{
	system("CLS");
	bool if_found = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (people[i].bplace == bplace)
		{
			cout << "NUMER ID #" << i + 1 << endl;
			if_found = 1;
			display_person(people[i]);
			cout << endl;
		}
	}

	iff_found(if_found);
	cont();
	_getch();
}

void find_menu(vector<person> people)
{
	system("CLS");

	char command;
	cout << PL("W jaki sposÛb chcesz rozpoczπÊ wyszukiwanie ?") << endl;
	cout << PL("1. ImiÍ") << endl;
	cout << "2. Nazwisko" << endl;
	cout << "3. Pesel" << endl;
	cout << "4. Miejsce urodzenia" << endl;
	cout << PL("5. PowrÛt") << endl;

	command = _getch();

	switch (command)
	{
	case '1':
	{
		string name;
		cout << endl << PL("Podaj imiÍ:") << endl;
		cin >> name;
		find_by_name(people, name);
		break;
	}
	case '2':
	{
		string surname;
		cout << endl << "Podaj nazwisko:" << endl;
		cin >> surname;
		find_by_surname(people, surname);
		break;
	}
	case '3':
	{
		string pesel;
		cout << endl << "Podaj pesel:" << endl;
		cin >> pesel;
		find_by_pesel(people, pesel);
		break;
	}
	case '4':
	{
		string bplace;
		cout << endl << "Podaj miejsce urodzenia:" << endl;
		cin >> bplace;
		find_by_bplace(people, bplace);
		break;
	}
	default:
		break;
	}
}

//6: Pokaz wszystkie osoby
void display_data(vector<person> people)
{
	system("CLS");
	cout << PL("Informacje wyúwietlane sπ w formacie #[NUMER ID] [ImiÍ] [Nazwisko] [Numer pesel] [Miejsce zamieszkania] [Data urodzenia]") << endl << endl;
	for (int i = 0; i < people.size(); i++)
	{
		cout << "NUMER ID #" << i + 1 << endl;
		display_person(people[i]);
		cout << endl;
	}
	cont();
	_getch();
}

//7: Zapisywanie danych do pliku
bool if_space(string bplace)
{
	for (int i = 0; i < bplace.length(); i++)
	{
		if (bplace[i] = ' ')
			return true;
	}

	return false;
}

string formatted_space(string bplace)
{
	if (if_space(bplace))
	{
		bplace.erase(remove(bplace.begin(), bplace.end(), ' '), bplace.end());
	}
	return bplace;
}

void save_to_file(vector<person> &people, ofstream &plik)
{
	for (int i = 0; i < people.size(); i++)
	{
		plik << people[i].name + " " << people[i].surname + " " << people[i].pesel + " " << formatted_space(people[i].bplace);
		if (i != people.size() - 1)
			plik << endl;			//Bez tej linijki przy zczytywaniu danych kopiuje sie ostatnia osoba (prawdopodobnie eof czyta endl jako znak)
	}

	system("CLS");
}

//10: Statystyki
//Srednia wieku, najstarszy, najmlodszy, ile z kazdej miejscowosci, ile kobiet, mezczyzn, dzieci
void err_empty_ppl(vector<person> people)
{
	if (people.size() == 0)
	{
		cout << PL("B£•D! Brak osÛb w bazie") << endl;
		cout << PL("Naciúnij dowolny przycisk, aby kontynuowaÊ") << endl;
		_getch();
	}
}

void err_empty_age(vector<int> age)
{
	if (age.size() == 0)
	{
		cout << PL("B£•D! Brak osÛb w bazie") << endl;
		cout << PL("Naciúnij dowolny przycisk, aby kontynuowaÊ") << endl;
		_getch();
	}
}

bool if_alrd_bday(bdate bday)
{
	if (bday.month < 4 || (bday.month == 4 && bday.day < 17))
		return true;
	else
		return false;
}

void avg_age(vector<int> age)
{
	system("CLS");

	err_empty_age(age);
	if (age.size() == 0)
		return;

	double sum = 0;
	double avg;

	for (int i = 0; i < age.size(); i++)
		sum += age[i];

	avg = sum / age.size();
	cout << PL("åREDNIA WIEKU: ") << setprecision(2) << fixed << avg << " lat";
	_getch();
}

void oldest(vector<int> age, vector<person> people)
{
	int oldest = 0;
	int id = 0;
	for (int i = 0; i < age.size(); i++)
	{
		if (age[i] > oldest)
		{
			oldest = age[i];
			id = i;
		}
	}

	cout << "Najstarsza osoba w systemie: " << endl;
	display_person(people[id]);
	cout << "WIEK: " << oldest << endl << endl;
}

void youngest(vector<int> age, vector<person> people)
{
	int youngest = 100000;
	int id = 0;
	for (int i = 0; i < age.size(); i++)
	{
		if (age[i] < youngest)
		{
			youngest = age[i];
			id = i;
		}
	}

	cout << PL("Najm≥odsza osoba w systemie: ") << endl;
	display_person(people[id]);
	cout << "WIEK: " << youngest << endl << endl;
}

void oldest_n_youngest(vector<int> age, vector<person> people)
{
	system("CLS");
	err_empty_age(age);
	if (age.size() == 0)
		return;
	oldest(age, people);
	youngest(age, people);
	_getch();
}

void bplace_table(vector<person> people)
{
	system("CLS");
	err_empty_ppl(people);
	if (people.size() == 0)
		return;

	vector<string> bplace;
	for (int i = 0; i < people.size(); i++)
	{
		bplace.push_back(people[i].bplace);
	}

	sort(bplace.begin(), bplace.end());
	int howmany = 0;
	int i = 0;
	string cityname = bplace[0];

	while (i < bplace.size())
	{
		if (bplace[i] == cityname)
		{
			howmany += 1;
			i++;
		}
		else
		{
			cout << cityname << ": " << howmany << endl;
			howmany = 1;
			cityname = bplace[i];
			i++;
		}


		if (i == bplace.size())
		{
			cout << cityname << ": " << howmany << endl;
		}
	}

	_getch();
}

void fill_age(vector<int> &age, vector<person> people)
{
	vector<bdate> bdays;
	for (int i = 0; i < people.size(); i++)
	{
		bdays.push_back(bdate_from_pesel(people[i].pesel));
	}

	for (int i = 0; i < bdays.size(); i++)
	{
		if (!if_alrd_bday(bdays[i]))
			age.push_back(2019 - bdays[i].year - 1);
		else
			age.push_back(2019 - bdays[i].year);
	}
}

void count_sex(vector<person> people)
{
	system("CLS");
	err_empty_ppl(people);
	if (people.size() == 0)
		return;

	int mencounter = 0;
	int womencounter = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (sex_check(people[i]))
			mencounter++;
		else
			womencounter++;
	}

	cout << "Kobiety: " << womencounter << endl;
	cout << PL("MÍøczyüni: ") << mencounter << endl;

	_getch();
}

void age_table(vector<int> age)
{
	system("CLS");

	int tab[5] = { 0,0,0,0,0 };
	//0: 0-18
	//1: 19-29
	//2: 30-49
	//3: 50-69
	//4: 70+

	for (int i = 0; i < age.size(); i++)
	{
		if (age[i] >= 0 && age[i] <= 18)
			tab[0]++;
		else if (age[i] >= 19 && age[i] <= 29)
			tab[1]++;
		else if (age[i] >= 30 && age[i] <= 49)
			tab[2]++;
		else if (age[i] >= 50 && age[i] <= 69)
			tab[3]++;
		else
			tab[4]++;
	}

	cout << "0 - 18: " << tab[0] << endl;
	cout << "19 - 29: " << tab[1] << endl;
	cout << "30 - 49: " << tab[2] << endl;
	cout << "50 - 69: " << tab[3] << endl;
	cout << "70+: " << tab[4] << endl;

	_getch();
}

//8: Usun baze danych
void delete_data(vector<person> &people)
{
	system("CLS");

	cout << PL("Czy na pewno chcesz usunπÊ wszystkie dane z systemu?") << endl;
	cout << "1. TAK" << endl << "2. NIE" << endl;
	char odp = _getch();

	switch (odp)
	{
	case '1':
	{
		people.clear();
		cout << PL("UsuniÍto bazÍ danych. Aby ostatecznie zatwierdziÊ usuniÍcie danych wybierz opcjÍ 7 z menu.") << endl;
		break;
	}
	case '2':
		break;
	default:
		break;
	}
}

//9: Archiwizacja
void archive(vector<person> &people)
{
	system("CLS");

	ofstream plikarchive("archive.txt");
	save_to_file(people, plikarchive);
	ofstream plikout("data.txt");
	people.clear();
	save_to_file(people, plikout);

	cout << PL("Dane usuniÍto z pliku 'data.txt' i przeniesiono do pliku 'archive.txt'. ") << endl;
	_getch();
}

void stats_menu(vector<person> people)
{
	system("CLS");

	vector<int> age;
	fill_age(age, people);

	cout << PL("Wybierz statystykÍ, ktÛrπ chcesz wyúwietliÊ:") << endl;
	cout << PL("1. årednia wieku") << endl;
	cout << PL("2. Najstarsza i najm≥odsza osoba w systemie") << endl;
	cout << PL("3. IloúÊ osÛb z poszczegÛlnych miejscowoúci") << endl;
	cout << PL("4. IloúÊ kobiet i mÍøczyzn") << endl;
	cout << PL("5. Podzia≥ na grupy wiekowe") << endl;
	cout << PL("6. PowrÛt do menu") << endl;

	char command;
	command = _getch();

	switch (command)
	{
	case '1':
	{
		avg_age(age);
		break;
	}
	case '2':
	{
		oldest_n_youngest(age, people);
		break;
	}
	case '3':
	{
		bplace_table(people);
		break;
	}
	case '4':
	{
		count_sex(people);
		break;
	}
	case '5':
	{
		age_table(age);
		break;
	}
	default:
		break;

	}
}

//10: Ustawienia
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void get_settings(bool settings[6])
{
	ifstream pliksettings("Settings.txt");
	string input = " ";
	int iff;
	int i = 0;

	while (!pliksettings.eof())
	{
		getline(pliksettings, input);
		settings[i] = input[input.length() - 1] - '0';
		i++;
	}
}

void settings_display(int curopt, bool settings[6])
{
	string settingsstring[6] = { PL("ImiÍ"), "Nazwisko", "Pesel", "Miejsce zamieszkania", PL("Generowanie zbioru osÛb"), PL("Usuwanie zbioru osÛb") };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("CLS");
	cout << PL("Sterowanie: klawisze W,S,Enter") << endl << endl;
	ShowConsoleCursor(false);

	for (int i = 0; i < 6; i++)
	{
		if (i + 1 == curopt)
			cout << "--->";
		else
			cout << "    ";

		cout << settingsstring[i];

		if (i >= 0 && i <= 3)
		{
			SetConsoleTextAttribute(hConsole, 14);

			if (settings[i] == 1)
				cout << " >Generuj<" << endl;
			else if (settings[i] == 0)
				cout << PL(" >RÍcznie<") << endl;
		}
		else if (i >= 4 && i <= 5)
		{
			SetConsoleTextAttribute(hConsole, 12);

			if (settings[i] == 1)
				cout << PL(" >W≥πcz<") << endl;
			else if (settings[i] == 0)
				cout << PL(" >Wy≥πcz<") << endl;
		}

		SetConsoleTextAttribute(hConsole, 7);
	}

	if (curopt == 7)
		cout << "--->";
	else
		cout << "    ";

	cout << PL("WrÛÊ") << endl;
}


void set_settings(bool settings[6])
{
	ofstream plikout("Settings.txt");
	string tab[6] = { "GenNames", "GenSurnames", "GenPesel", "GenBPlace", "GenManyPpl", "DltManyPpl" };

	for (int i = 0; i < 6; i++)
	{
		plikout << i << "." << tab[i] << ":" << settings[i];

		if (i != 5)
			plikout << endl;
	}
}

void settings_menu(bool settings[6])
{
	int curopt = 1;
	int command = 0;

	while (curopt != 7 || command != 13)
	{
		if (curopt < 1)
			curopt = 7;

		if (curopt > 7)
			curopt = 1;

		settings_display(curopt, settings);
		command = 0;

		while (command != 13 && command != 115 && command != 119)
			command = _getch();

		if (command == 13 && curopt != 7)
			settings[curopt - 1] = !settings[curopt - 1];
		else if (command == 115)
			curopt += 1;
		else if(command == 119)
			curopt -= 1;
	}

	set_settings(settings);
}

void menu(vector<person> &people, bool &if_saved, bool settings[6])
{
	int command;
	system("CLS");

	vector<string> names;
	names_load(names);

	vector<string> surnames;
	surnames_load(surnames);

	vector<string> bplaces;
	bplaces_load(bplaces);

	cout << "---BAZA DANYCH---" << endl;
	cout << PL("1) Dodaj osoby") << endl;
	cout << PL("2) UsuÒ osoby") << endl;
	cout << PL("3) Sortuj bazÍ danych") << endl;
	cout << "4) Edycja danych" << endl;
	cout << "5) Wyszukiwanie osoby" << endl;
	cout << PL("6) Pokaø wszystkie osoby") << endl;
	cout << "7) Zapisz dane do pliku" << endl;
	cout << PL("8) UsuÒ ca≥π bazÍ danych") << endl;
	cout << PL("9) Archiwizuj bazÍ danych") << endl;
	cout << "10) Statystyki" << endl;   //Srednia wieku, najstarszy, najmlodszy, ile z kazdej miejscowosci, ile kobiet, mezczyzn, dzieci
	cout << "11) Ustawienia" << endl;
	cout << "12) Koniec" << endl << endl;
	
	cin >> command;

	switch (command)
	{
	case 1:
	{
		adding_menu(people, names, surnames, bplaces, settings);

		if_saved = 0;
		break;
	}
	case 2:
	{
		delete_menu(settings, people);

		if_saved = 0;
		break;
	}
	case 3:
	{
		sort_data(people);
		if_saved = 0;
		break;
	}
	case 4:
	{
		data_edition(people);
		if_saved = 0;
		break;
	}
	case 5:
	{
		find_menu(people);
		break;
	}
	case 6:
	{
		display_data(people);
		break;
	}
	case 7:
	{
		ofstream plikout("database.txt");
		save_to_file(people, plikout);
		if_saved = 1;
		cout << PL("Dane zapisano pomyúlnie.");
		cont();
		_getch();
		break;
	}
	case 8:
	{
		delete_data(people);
		break;
	}
	case 9:
	{
		archive(people);
		break;
	}
	case 10:
	{
		stats_menu(people);
		break;
	}
	case 11:
	{
		settings_menu(settings);
		break;
	}
	case 12:
	{
		if (if_saved)
		{
			exit(-1);
		}
		else
		{
			cout << PL("Czy na pewno chcesz zamknπÊ program ? Nie zapisano zmian w pliku.") << endl;
			cout << "1. TAK" << endl;
			cout << "2. NIE" << endl;
			char answer = _getch();

			while (answer != '1' && answer != '2')
			{
				cout << PL("Niepoprawna cyfra. SprÛbuj ponownie.") << endl;
				answer = _getch();
			}

			if (answer == '1')
				exit(-1);
		}

		break;
	}	
	default:
	{
		system("CLS");
		cout << PL("Niepoprawna wartoúÊ. Naciúnij dowolny przycisk, aby powrÛciÊ do menu.");
		_getch();
	}		
	}
}

//Reading from file
bool if_cap(string word)
{
	for (int i = 1; i < word.length(); i++)
	{
		if ((int)word[i] >= 65 && (int)word[i] <= 90)
			return true;
	}

	return false;
}

string formatted_input(string word)
{
	if (if_cap(word))
	{
		for (int i = 1; i < word.length(); i++)
		{
			if ((int)word[i] >= 65 && (int)word[i] <= 90)
			{
				word.insert(i, " ");
				i++;
			}
		}
	}

	return word;
}

void read_from_file(vector<person> &people)
{
	//Deklaracja pliku, z ktÛrego zczytywane bÍdπ dane
	ifstream plikin("Database.txt");
	person input;

	while (!plikin.eof())
	{
		plikin >> input.name >> input.surname >> input.pesel >> input.bplace;
		input.name = formatted_input(input.name);
		input.surname = formatted_input(input.surname);
		input.bplace = formatted_input(input.bplace);

		people.push_back(input);
	}
}

bool fexists(const char *filename) 
{
	std::ifstream ifile(filename);
	return !(bool)ifile;
}

int main()
{
	//srand
	srand(time(NULL));
	bool settings[6];
	get_settings(settings);

	//Zczytanie danych z pliku do wektora
	if(fexists("Database.txt"))
		ofstream plikout("Database.txt");
	ifstream plikin("Database.txt");
	vector<person> people;
	person input;

	while (!plikin.eof())
	{
		plikin >> input.name >> input.surname >> input.pesel >> input.bplace;
		input.name = formatted_input(input.name);
		input.surname = formatted_input(input.surname);
		input.bplace = formatted_input(input.bplace);

		people.push_back(input);
	}

	//Eof fsr spisuje dane z pustego pliku wiec ta linijka jest wymagana w przypadku utworzonego, ale niezawierajacego danych pliku database.txt
	if (people.size() == 1 && people[0].name == "")
		people.clear();

	//Zmienna wskazujaca, czy dokonane zmiany zostaly zapisane w pliku
	bool if_saved = 1;

	//Wyswietlanie menu
	while (1)
	{
		menu(people, if_saved, settings);
	}
}

//Stan: mozna skutecznie zczytac osoby z pliku i dodac nowa osobe do pliku