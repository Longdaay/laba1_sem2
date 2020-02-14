#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <fstream>

using namespace std;
int count_of_students = 30;
int fact_count = 0;
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
	int grade; // средний балл
};

struct Group
{
	int group_number;
	vector <Student> student{count_of_students};
} gr9894, gr9893;

void delete_student(int index);

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

void fill_students(Group group)
{
	ifstream fin;
	int temp;
	int count_grade = 0;

	fin.open("input.txt");
	if (fin.is_open())
	{
		int i = 0;
		while(!fin.eof())
		{
			temp = 0;
			getline(fin, group.student[i].name, ' ');
			getline(fin, group.student[i].surname, ' ');
			getline(fin, group.student[i].father_name, ' ');
			getline(fin, group.student[i].sex, ' ');
			getline(fin, group.student[i].form_edu, ' ');
			fin >> group.student[i].group_number;
			fin >> group.student[i].list_number;
			for (int j = 0; j <= count_of_rates - 1; j++)
			{	
				fin >> temp;
				count_grade += temp;
				group.student[i].rates.push_back(temp);
			}
			group.student[i].grade = count_grade / count_of_rates;
			GetLocalTime(&group.student[i].sys_t);
			fin.ignore(1);
			if (group.student[i].grade <= 2)
				delete_student(i);
			else
			{
				i++;
				fact_count++;
			}
		}
	}
	else 
	{
		cout << "Файл не удалось открыть!" << endl;
	}
	cout << endl;
	fin.close();
}

void print_rates(const Group& group, const int& list_number)
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << group.student[list_number].rates[i] << " ";
	}
}

void print_group_student(const Group &group)
{
	cout << "Список студентов: " << endl;
	cout << setw(3) << left << "№" << setw(13) << left << "Name" 
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName" 
		<< setw(8) << left << "Sex" << setw(16) << left << "Form" 
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber" 
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	int i = 0;
	while (i <= fact_count - 1)
	{
		cout << setfill(' ');
		cout << setw(3) << left << i + 1 << setw(12) << left << group.student[i].name << " "
			<< setw(12) << left << group.student[i].surname << " \t"
			<< setw(10) << left << group.student[i].father_name << " \t"
			<< setw(6) << left << group.student[i].sex << " \t"
			<< setw(5) << left << group.student[i].form_edu << " \t" << setw(5) <<right;
		print_rates(group, i);
		cout << " " << setw(12) << left << group.student[i].group_number << " "
			<< setw(10) << left << group.student[i].list_number << " ";
		printSystemTime(group.student[i].sys_t);
		i++;
	}
	cout << endl;
	cout << setfill(' ');
}

void add_student(Group group)
{
	int temp = 0;
	
	cout << "Name: ";
	cin >> group.student[fact_count].name;
	cout << "Surname: ";
	cin >> group.student[fact_count].surname;
	cout << "FatherName: ";
	cin >> group.student[fact_count].father_name;
	cout << "Sex: ";
	cin >> group.student[fact_count].sex;
	cout << "FormEdu: ";
	cin >> group.student[fact_count].form_edu;
	cout << "GroupNumber: ";
	cin >> group.student[fact_count].group_number;
	cout << "ListNumber: ";
	cin >> group.student[fact_count].list_number;
	cout << "Rating: ";
	for (int j = 0; j <= count_of_rates - 1; j++)
	{
		cout << j << ": ";
		cin >> temp;
		group.student[fact_count].rates.push_back(temp);
	}
	GetLocalTime(&group.student[fact_count].sys_t);
	fact_count++;
}

void delete_student(Group group, int index)
{
	group.student.erase(group.student.begin() + index);
	fact_count--;
}

void change_info_student()
{
	cout << "What the info do you want to change?" << endl;
	cout << "1. GroupNumber" << endl;
	cout << "2. Info about student" << endl;

	cout << "" << endl;


}

int main()
{
	setlocale(0, ""); // локализация
	SetConsoleCP(1251); // локализация ввода строки
	SetConsoleOutputCP(1251); // локализация вывода строки

	fill_students(gr9894);
	print_group_student(gr9894);

	add_student(gr9894);
	print_group_student(gr9894);
	
	delete_student(4);
	print_group_student(gr9894);
	
	system("pause");
	return 0;
}
