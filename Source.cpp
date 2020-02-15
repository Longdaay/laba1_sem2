#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <fstream>

using namespace std;


int count_of_students = 30;
//int fact_count = 0;
int count_of_rates = 8;
int index = 0;
int group_count = 0;


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
	int fact_count = 0;
	int group_number;
	vector <Student> student{ count_of_students };
} gr9894, gr9893, group_all;

void info_student(Group &group);

void delete_student(Group group, int index)
{
	group.student.erase(group.student.begin() + index);
	group.fact_count--;
}

void out_width()
{
	cout << setw(2);
	cout << setfill('0');
}

void printSystemTime(const SYSTEMTIME& st)
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
	cout << st.wSecond;
}

void fill_students(Group &group, ifstream &fin)
{
	int temp;
	int count_grade = 0;

	if (fin.is_open())
	{
		int i = 0;
		while (!fin.eof())
		{
			temp = 0;
			count_grade = 0;
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
			group.student[i].edit_flag = FALSE;
			fin.ignore(1);
			if (group.student[i].grade <= 2)
				delete_student(group, i);
			else
			{
				i++;
				group.fact_count++;
			}
		}
	}
	else
	{
		cout << "Файл не удалось открыть!" << endl;
	}
	fin.close();
}

void add_group(Group &group, int gr_index)
{
	group_all.student.resize(group_count + 1);
	//group_all.student.push_back(group.student[gr_index]);
	group_all.student[index] = group.student[gr_index];
	index++;
	group_all.fact_count++;
}

void fill_group()
{
	group_count = gr9894.fact_count + gr9893.fact_count;
	group_all.student.resize(group_count);
	int i = 0;
	while (i <= gr9893.fact_count - 1)
	{
		add_group(gr9893, i);
		i++;
	}
	i = 0;
	while (i <= gr9894.fact_count - 1)
	{
		add_group(gr9894, i);
		i++;
	}
}

void print_rates(const Group& group, const int& list_number)
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << group.student[list_number].rates[i] << " ";
	}
}

void print_group_student(const Group& group)
{
	cout << "Список студентов: " << endl;
	cout << setw(3) << left << "№" << setw(13) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(8) << left << "Sex" << setw(16) << left << "Form"
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber"
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	int i = 0;
	while (i <= group.fact_count - 1)
	{
		cout << setfill(' ');
		cout << setw(3) << left << i + 1 << setw(12) << left << group.student[i].name << " "
			<< setw(12) << left << group.student[i].surname << " \t"
			<< setw(10) << left << group.student[i].father_name << " \t"
			<< setw(6) << left << group.student[i].sex << " \t"
			<< setw(5) << left << group.student[i].form_edu << " \t" << setw(5) << right;
		print_rates(group, i);
		cout << " " << setw(12) << left << group.student[i].group_number << " "
			<< setw(10) << left << group.student[i].list_number << " ";
		printSystemTime(group.student[i].sys_t);
		if (group.student[i].edit_flag)
			cout << setw(2) << "  edit data" << endl;
		else
			cout << setw(2) << "  add data" << endl;
		i++;
	}
	cout << endl;
	cout << setfill(' ');
}

void add_student(Group &group)
{
	int temp = 0;

	cout << "Name: ";
	cin >> group.student[group.fact_count].name;
	cout << "Surname: ";
	cin >> group.student[group.fact_count].surname;
	cout << "FatherName: ";
	cin >> group.student[group.fact_count].father_name;
	cout << "Sex: ";
	cin >> group.student[group.fact_count].sex;
	cout << "FormEdu: ";
	cin >> group.student[group.fact_count].form_edu;
	cout << "GroupNumber: ";
	cin >> group.student[group.fact_count].group_number;
	cout << "ListNumber: ";
	cin >> group.student[group.fact_count].list_number;
	cout << "Rating: " << endl;
	for (int j = 0; j <= count_of_rates - 1; j++)
	{
		cout << j << ": ";
		cin >> temp;
		group.student[group.fact_count].rates.push_back(temp);
	}
	GetLocalTime(&group.student[group.fact_count].sys_t);
	group.student[group.fact_count].edit_flag = FALSE;
	add_group(group, group.fact_count);
	group.fact_count++;
}

void checkgroup()
{ // функция проверки выбранного действия
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // вводим выбранное значение 
	if (strlen(value) == 1) // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
	{
		switch (value[0]) // проверям, взяв первый символ переменной value
		{
		case '1': // 9894
			info_student(gr9894);
			break;
		case '2': // 9893
			info_student(gr9893);
			break;
		default: // если число не подходит ни к одному из
			cout << "Значение введено неверно. Введите заново" << endl;
			checkgroup();
			break;
		}
	}
	else // если введено символов больше необходимого
	{
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		checkgroup();
	}
}

int check_list(const Group& group)
{
	while (true)
	{
		int value;
		// вводим число, которое хотим представить в двоичной форме
		cin >> value; // число целое
		if (cin.fail()) // ecли предыдущее извелечение оказлось неудачным,
		{
			cin.clear(); // то возвращаем cin в обычный режим работы
			cin.ignore(32767, '\n'); // и удаляем из буфера значения предыдущего ввода 
			cout << "Недопустимое заданное число. Введите число правильно" << '\n';
		}
		else
		{
			value--;
			for (int i = 0; i <= group.fact_count - 1; i++)
			{
				if(group.student[i].list_number == value)
				{
					cin.ignore(32767, '\n'); // удаляем из буфера значения предыдущего ввода 
					return value;
				}
			}
			cout << "This list number isn't exist. Try again" << endl;
			cout << "List number is ";
			check_list(group);
		}
	}
}

void print_student_info(const Group &group, int list_number)
{
	cout << setfill(' ');
	cout << setw(12) << left << group.student[list_number].name << " "
		<< setw(12) << left << group.student[list_number].surname << " \t"
		<< setw(10) << left << group.student[list_number].father_name << " \t"
		<< setw(6) << left << group.student[list_number].sex << " \t"
		<< setw(5) << left << group.student[list_number].form_edu << " \t" << setw(5) << right;
	print_rates(group, list_number);
	cout << " " << setw(12) << left << group.student[list_number].group_number << " "
		<< setw(10) << left << group.student[list_number].list_number << " ";
	printSystemTime(group.student[list_number].sys_t);
	cout << setfill(' ');
}

void change_rates(Group &group, int list_number)
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << "Current rate: " << group.student[list_number].rates[i] << " Change rate - ";
		cin >> group.student[list_number].rates[i];
	}
}

void CheckChangeInfo(Group &group, int list_number)
{
	int par;
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // вводим выбранное значение 
	if (strlen(value) == 1) // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
	{
		switch (value[0]) // проверям, взяв первый символ переменной value
		{
		case '1': //
			cout << "Enter new value: ";
			cin >> group.student[list_number].name;
			break;
		case '2': //
			cout << "Enter new value: ";
			cin >> group.student[list_number].surname;
			break;
		case '3': //
			cout << "Enter new value: ";
			cin >> group.student[list_number].father_name;
			break;
		case '4': //
			cout << "Enter new value: ";
			cin >> group.student[list_number].sex;
			break;
		case '5': //
			cout << "Enter new value: ";
			cin >> group.student[list_number].form_edu;
			break;
		case '6': //
			cout << "Enter new value: ";
			change_rates(group, list_number);
			break;
		default: // если число не подходит ни к одному из
			cout << "Значение введено неверно. Введите заново" << endl;
			CheckChangeInfo(group, list_number);
			break;
		}
	}
	else // если введено символов больше необходимого
	{
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		CheckChangeInfo(group, list_number);
	}
	GetLocalTime(&group.student[list_number].sys_t);
	group.student[list_number].edit_flag = TRUE;
}

void change_info_student()
{
	cout << "Choose the group number:" << endl;
	cout << "1. 9894" << endl;
	cout << "2. 9893" << endl;
	cout << "Your chose: ";
	checkgroup();
}

void info_student(Group &group)
{
	int list_number;
	cout << "Enter list number of student: ";
	list_number = check_list(group);
	print_student_info(group, list_number);
	cout << "What the info do you want to change?" << endl;
	cout << "1. Name" << endl;
	cout << "2. Surname" << endl;
	cout << "3. FatherName" << endl;
	cout << "4. Sex" << endl;
	cout << "5. FormEdu" << endl;
	cout << "6. Rates" << endl;
	CheckChangeInfo(group, list_number);
}

void choose_print_group()
{
	cout << "Choose the group:" << endl;
	cout << "1. 9894" << endl;
	cout << "2. 9893" << endl;
	cout << "Chose the group - ___" << endl;
// функция проверки выбранного действия
		char value[256]; // переменная, которая хранит выбранное значение
		cin >> value; // вводим выбранное значение 
		if (strlen(value) == 1) // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		{
			switch (value[0]) // проверям, взяв первый символ переменной value
			{
			case '1': // 9894
				print_group_student(gr9894);
				break;
			case '2': // 9893
				print_group_student(gr9893);
				break;
			default: // если число не подходит ни к одному из
				cout << "Значение введено неверно. Введите заново" << endl;
				checkgroup();
				break;
			}
		}
		else // если введено символов больше необходимого
		{
			cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
			checkgroup();
		}
}

void swap(Group* first_p, Group* second_p) // смена переменных между собой
{
	Group temp = *first_p;
	*first_p = *second_p;
	*second_p = temp;
}

void print_rating_students()
{
	for (int i = 0; i < group_count - 1; i++)
	{
		for (int j = 0; j < group_count - 1; j++)
		{
			if (group_all.student[j].grade < group_all.student[j + 1].grade)
			{
				swap(group_all.student[j], group_all.student[j + 1]);
			}
		}
	}
	print_group_student(group_all);
}

void print_sex_info()
{
	int count_male = 0;
	int count_female = 0;
	int count_not_money = 0;
	bool br_flag = FALSE;
	int j = 0;

	for (int i = 0; i < group_count - 1; i++)
	{
		j = 0;
		br_flag = FALSE;
		if (group_all.student[i].sex == "male")
			count_male++;
		else
			count_female++;
		while (j <= count_of_rates - 1 and br_flag == FALSE)
		{
			if (group_all.student[i].rates[j] <= 3 and group_all.student[i].form_edu == "den")
			{
				count_not_money++;
				br_flag = TRUE;
			}
			j++;
		}
	}
	cout << "Male - " << count_male << endl;
	cout << "Female - " << count_female << endl;
	cout << "Count not money - " << count_not_money << endl;
}
int main()
{
	ifstream fin;

	HANDLE  hout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD  size;
	size.X = 128;     // кол-во символов на строку
	size.Y = 1001;  // увеличиваем до 1000 - строк, можно хоть 2000
	SetConsoleScreenBufferSize(hout, size);
	setlocale(0, ""); // локализация
	SetConsoleCP(1251); // локализация ввода строки
	SetConsoleOutputCP(1251); // локализация вывода строки


	fin.open("gr9894.txt");
	fill_students(gr9894, fin);
	fin.open("gr9893.txt");
	fill_students(gr9893, fin);

	fill_group();
	//print_group_student(group_all);
	//choose_print_group();
	//print_group_student(gr9894);

	//change_info_student();
	//print_group_student(gr9894);

	//add_student(gr9894);

	//print_group_student(group_all);
	print_rating_students();
	print_sex_info();
	/*print_group_student(gr9894);

	delete_student(gr9894, 4);
	print_group_student(gr9894);*/

	system("pause");
	return 0;
}
