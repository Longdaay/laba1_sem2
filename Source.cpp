#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <fstream>

using namespace std;
const int count_of_students = 30;
int count_of_rates = 8;

struct Student
{
	string name; //Имя
	string surname; //Фамилия
	string father_name; //Отчество
	string sex; //Пол
	int group_number;
	int list_number;
	vector<int> rates; //Оценки
	string form_edu; //Форма обучения
	SYSTEMTIME sys_t; //Отметка времени о создании или изменении данных
	bool edit_flag; // 1 - если данные изменялись
};

struct Group
{
	int group_number;
	Student student[count_of_students];
} gr9894, gr9893;

void out_width()
{
	cout << setw(2);
	cout << setfill('0');
}

void printSystemTime(const SYSTEMTIME &st)
{
	//cout << "Дата создания: ";
	out_width();
	cout << st.wDay << ".";
	out_width();
	cout << st.wMonth << ".";
	out_width();
	cout << st.wYear << " ";
	out_width();
	cout << st.wHour << ":";
	out_width();
	cout << st.wMinute << ":";
	out_width();
	cout << st.wSecond << endl;
}

void fill_students()
{
	ifstream fin;
	int temp;

	fin.open("input.txt");
	if (fin.is_open())
	{
		for (int i = 0; i <= count_of_students - 1; i++)
		{
			temp = 0;
			getline(fin, gr9894.student[i].name, ' ');
			getline(fin, gr9894.student[i].surname, ' ');
			getline(fin, gr9894.student[i].father_name, ' ');
			getline(fin, gr9894.student[i].sex, ' ');
			getline(fin, gr9894.student[i].form_edu, ' ');
			fin >> gr9894.student[i].group_number;
			fin >> gr9894.student[i].list_number;
			for (int j = 0; j <= count_of_rates - 1; j++)
			{	
				fin >> temp;
				gr9894.student[i].rates.push_back(temp);
			}
			GetLocalTime(&gr9894.student[i].sys_t);
			fin.ignore(1);
		}
	}
	else 
	{
		cout << "Файл не удалось открыть!" << endl;
	}
	cout << endl;
	fin.close();
}

void print_rates(const int &list_number)
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << gr9894.student[list_number].rates[i] << " ";
	}
}

void print_group_student()
{
	cout << "Список студентов: " << endl;
	cout << setw(3) << left << "№" << setw(13) << left << "Name" 
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName" 
		<< setw(8) << left << "Sex" << setw(16) << left << "Form" 
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber" 
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	for (int i = 0; i <= count_of_students - 1; i++)
	{
		cout << setfill(' ');
		cout << setw(3) << left << i + 1 << setw(12) << left << gr9894.student[i].name << " "
			<< setw(12) << left << gr9894.student[i].surname << " \t"
			<< setw(10) << left << gr9894.student[i].father_name << " \t"
			<< setw(6) << left << gr9894.student[i].sex << " \t"
			<< setw(5) << left << gr9894.student[i].form_edu << " \t" << setw(5) <<right;
		print_rates(i);
		cout << " " << setw(12) << left << gr9894.student[i].group_number << " "
			<< setw(10) << left << gr9894.student[i].list_number << " ";
		printSystemTime(gr9894.student[i].sys_t);
	}
	cout << endl;
}


int main()
{
	setlocale(0, ""); // локализация
	SetConsoleCP(1251); // локализация ввода строки
	SetConsoleOutputCP(1251); // локализация вывода строки

	fill_students();
	print_group_student();
	
	system("pause");
	return 0;
}
