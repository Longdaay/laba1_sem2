#include <iostream> // ��� �����/������
#include <vector> //��� �������� �������
#include <string> // ��� �������� �����
#include <Windows.h> // ��� ��������� �����
#include <iomanip> // ��� ��������� ���������� ������ 
#include <fstream> // ��� ����������/������ ��/� ����(�)
#include <conio.h> // ��� ���� (���������� �������� �������� �� ������� ��� ������������� ������ � echo-������ (getch(void))

using namespace std;

void SetColor(int text, int bg) //������� ����� �����, ������ �� ���������
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

const int NotUsed = system("color 70"); // ��������� ����� ������� � ����� 

int count_of_students = 50; // ���������� ��������� � "�����" ������
int count_of_rates = 8; // ���������� ������ � ���������
int index = 0; 
int group_count = 0; // ���������� ����������
int count_of_players = 30; // ���������� ���������� � ����� �������������
int m_count = 0; // �������� ��� ���� - ��� �������� ���������� ��� ������� ������� ����� ��� ����

struct Student
{
	string name; //���
	string surname; //�������
	string father_name; //��������
	string sex; //���
	int group_number; // ����� ������
	int list_number;
	vector<int> rates{ count_of_rates }; //������
	string form_edu; //����� ��������
	SYSTEMTIME sys_t; //������� ������� � �������� ��� ��������� ������
	bool edit_flag; // 1 - ���� ������ ����������
	int grade; // ������� ����
};

struct Group
{
	int fact_count = 0; // ����������� ���������� ���������� � ������
	vector <Student> student{ count_of_students }; // ������ ��������� 
} gr9894, gr9893, group_all;

void info_student(Group &group); // �������� ������ ���������� � ��������

void menu_choice(); // �������� ����

void menu(); // �������� ����

void delete_student(Group group, int index) // ������� �������� ���������
{
	group.student.erase(group.student.begin() + index);
	group.fact_count--;
}

void out_width() // ������� ������������� ������ �������
{
	cout << setw(2); // ������
	cout << setfill('0'); // ���������� ������
}

void printSystemTime(const SYSTEMTIME& st) // ����� ������� �� ��������/���������
{
	//cout << "���� ��������: ";
	out_width();
	cout << right << st.wDay << "."; // ����
	out_width();
	cout << right << st.wMonth << "."; // �����
	out_width();
	cout << st.wYear << " "; // ���
	out_width();
	cout << right << st.wHour << ":"; // ���
	out_width();
	cout << right << st.wMinute << ":"; // ������
	out_width();
	cout << right << st.wSecond; // �������
}

void fill_students(Group &group, ifstream &fin) // ���������� ������ ��������� � ������
{
	int temp; // ��������� ����������
	int count_grade = 0; // ����� ������ ���������

	if (fin.is_open()) // ���� ���� ������ ������
	{
		int i = 0;
		while (!fin.eof()) // ���� �� ����� �� ����� �����
		{
			temp = 0; // �������� ����������
			count_grade = 0; // �������� �����
			group.student[i].rates.resize(count_of_rates); // ��������� ������ ������ � ��������
			getline(fin, group.student[i].name, ' '); // ������� ���
			getline(fin, group.student[i].surname, ' '); // ������� ������� 
			getline(fin, group.student[i].father_name, ' '); // ������� ��������
			getline(fin, group.student[i].sex, ' '); // ������� ���
			getline(fin, group.student[i].form_edu, ' '); // ������� ����� ��������
			fin >> group.student[i].group_number; // ������� ����� ������
			fin >> group.student[i].list_number; // ������� ����� �� ������
			for (int j = 0; j <= count_of_rates - 1; j++) // ������� ������ ��������
			{
				fin >> temp;
				count_grade += temp;
				group.student[i].rates[j] = temp;
			}
			group.student[i].grade = count_grade / count_of_rates; // ������� ������� ���� �� �������
			GetLocalTime(&group.student[i].sys_t); // ������� ����� �������� 
			group.student[i].edit_flag = FALSE; // ������������� ���� ��������� ������� � �������� 
			fin.ignore(1); // ���������� ������� ������
			if (group.student[i].grade <= 2) // ���� ������� ���� �������� ���� 2
				delete_student(group, i); // ������� 
			else
			{
				i++; // ����������� �������
				group.fact_count++; // ����������� ����������� ���������� ��������� � ������
			}
		}
	}
	else // ���� ���� ������� �� �������
	{
		cout << "���� �� ������� �������!" << endl;
	}
	fin.close(); // ��������� ����
}

void change_group(Group& group, int gr_index, char* value) // ������� �������� ��������� ���������� � �������� � ����� ������
{
	for (int i = 0; i < group_all.fact_count - 1; i++)
	{
		switch (*value)
		{
		case '1': // ���������� ���
			if(group_all.student[i].surname == group.student[gr_index].surname and group_all.student[i].father_name == group.student[gr_index].father_name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		case '2': // ���������� �������
			if (group_all.student[i].name == group.student[gr_index].name and group_all.student[i].father_name == group.student[gr_index].father_name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		case '3': // ���������� ��������
			if (group_all.student[i].surname == group.student[gr_index].surname and group_all.student[i].name == group.student[gr_index].name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		case '4': // ��������� ���
			if (group_all.student[i].surname == group.student[gr_index].surname and group_all.student[i].father_name == group.student[gr_index].father_name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		case '5': // ���������� ����� ��������
			if (group_all.student[i].surname == group.student[gr_index].surname and group_all.student[i].father_name == group.student[gr_index].father_name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		case '6': // ���������� ������
			if (group_all.student[i].surname == group.student[gr_index].surname and group_all.student[i].father_name == group.student[gr_index].father_name)
				group_all.student[gr_index] = group.student[gr_index];
			break;
		default:
			break;
		}
	}
}

void add_group(Group &group, int gr_index) // ������� ���������� ��������� � ����� ������
{
	group_all.student.resize(group_count + 1); // �������� ������ ��� ������ ��������
	group_all.student[index] = group.student[gr_index];
	index++;
	group_all.fact_count++;
}

void fill_group() // ������� ����������� ���� ����� � ����
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

void print_rates(const Group& group, const int& list_number) // ����� ������ ��������
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << group.student[list_number].rates[i] << " ";
	}
}

void print_group_student(const Group& group) // ����� ������ ���������
{
	system("cls"); // ������� �������
	cout << "������ ���������: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "Name"
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

	system("pause");
	menu();
}

void add_student(Group &group) // ���������� ����� ������ � ��������
{
	group.student.resize(group.fact_count + 1);
	int temp = 0;
	system("cls"); // ������� �������
	cout << "Name: ";
	cin >> group.student[group.fact_count].name;
	cout << "Surname: ";
	cin >> group.student[group.fact_count].surname;
	cout << "FatherName: ";
	cin >> group.student[group.fact_count].father_name;
	cout << "Sex(Male/Female): ";
	cin >> group.student[group.fact_count].sex;
	cout << "FormEdu(den/vecher): ";
	cin >> group.student[group.fact_count].form_edu;
	cout << "GroupNumber(9894/9893): ";
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
	group_count++;
	add_group(group, group.fact_count);
	
	system("pause");
	menu();
}

void checkgroup() // ������� ������ ������ ��� ������ ���������
{ // ������� �������� ���������� ��������
	char value[256]; // ����������, ������� ������ ��������� ��������
	cin >> value; // ������ ��������� �������� 
	if (strlen(value) == 1) // ��������� ���������� ��������� ��������. ���� �����, �� ������ ������ ��� ���, ����� �������� ������
	{
		switch (value[0]) // ��������, ���� ������ ������ ���������� value
		{
		case '1': // 9894
			info_student(gr9894);
			break;
		case '2': // 9893
			info_student(gr9893);
			break;
		default: // ���� ����� �� �������� �� � ������ ��
			cout << "�������� ������� �������. ������� ������" << endl;
			checkgroup();
			break;
		}
	}
	else // ���� ������� �������� ������ ������������
	{
		cout << "���������� ������ ���� ������. ���������� ������ ������" << endl;
		checkgroup();
	}
}

int check_list(const Group& group) // �������� ������ �������� �� ������
{
	while (true)
	{
		int value;
		// ������ �����, ������� ����� ����������� � �������� �����
		cin >> value; // ����� �����
		if (cin.fail()) // ec�� ���������� ����������� �������� ���������,
		{
			cin.clear(); // �� ���������� cin � ������� ����� ������
			cin.ignore(32767, '\n'); // � ������� �� ������ �������� ����������� ����� 
			cout << "������������ �������� �����. ������� ����� ���������" << '\n';
		}
		else
		{
			value--;
			for (int i = 0; i <= group.fact_count - 1; i++)
			{
				if(group.student[i].list_number == value)
				{
					cin.ignore(32767, '\n'); // ������� �� ������ �������� ����������� ����� 
					return value;
				}
			}
			cout << "This list number isn't exist. Try again" << endl;
			cout << "List number is ";
			check_list(group);
		}
	}
}

void print_student_info(const Group &group, int list_number) // ����� ������ � ��������
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

void change_rates(Group &group, int list_number) // ��������� ������� ������ � ��������
{
	for (int i = 0; i <= count_of_rates - 1; i++)
	{
		cout << "Current rate: " << group.student[list_number].rates[i] << " Change rate - ";
		cin >> group.student[list_number].rates[i];
	}
}

void CheckChangeInfo(Group &group, int list_number) // ��������� � ������������ ������ ��������
{
	bool par = FALSE;

	while (!par)
	{
		char value[256]; // ����������, ������� ������ ��������� ��������
		cin >> value; // ������ ��������� �������� 
		if (strlen(value) == 1) // ��������� ���������� ��������� ��������. ���� �����, �� ������ ������ ��� ���, ����� �������� ������
		{
			switch (value[0]) // ��������, ���� ������ ������ ���������� value
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
			default: // ���� ����� �� �������� �� � ������ ��
				cout << "�������� ������� �������. ������� ������" << endl;
				CheckChangeInfo(group, list_number);
				break;
			}
		}
		else // ���� ������� �������� ������ ������������
		{
			cout << "���������� ������ ���� ������. ���������� ������ ������" << endl;
			CheckChangeInfo(group, list_number);
		}
		GetLocalTime(&group.student[list_number].sys_t);
		group.student[list_number].edit_flag = TRUE; // ��������������� ���� ��������� ���������� � ��������
		change_group(group, list_number, value);
		cout << "Do you want change another one info about student?(1 - yes/0 - no)  ___";
		cin >> par;
		if (par)
			info_student(group);
		else
			par = TRUE;
	}
}

void change_info_student() // ����� ������ ��� ��������� ������ � ��������
{
	system("cls"); // ������� �������
	cout << "Choose the group number:" << endl;
	cout << "1. 9894" << endl;
	cout << "2. 9893" << endl;
	cout << "Your chose: ";
	checkgroup();

	system("pause");
	menu();
}

void info_student(Group &group) // ��������� ������ � ��������
{
	int list_number;
	cout << "Enter list number of student: ";
	list_number = check_list(group);
	print_student_info(group, list_number);
	cout << endl << "What the info do you want to change?" << endl;
	cout << "1. Name" << endl;
	cout << "2. Surname" << endl;
	cout << "3. FatherName" << endl;
	cout << "4. Sex" << endl;
	cout << "5. FormEdu" << endl;
	cout << "6. Rates" << endl;
	cout << "Your chose: ";
	CheckChangeInfo(group, list_number);
}

void choose_print_group() // ����� ������ ��� ������
{
	system("cls"); // ������� �������
	cout << "Choose the group:" << endl;
	cout << "1. 9894" << endl;
	cout << "2. 9893" << endl;
	cout << "Chose the group - ___";
	// ������� �������� ���������� ��������
	char value[256]; // ����������, ������� ������ ��������� ��������
	cin >> value; // ������ ��������� �������� 
	if (strlen(value) == 1) // ��������� ���������� ��������� ��������. ���� �����, �� ������ ������ ��� ���, ����� �������� ������
	{
		switch (value[0]) // ��������, ���� ������ ������ ���������� value
		{
		case '1': // 9894
			print_group_student(gr9894);
			break;
		case '2': // 9893
			print_group_student(gr9893);
			break;
		default: // ���� ����� �� �������� �� � ������ ��
			cout << "�������� ������� �������. ������� ������" << endl;
			checkgroup();
			break;
		}
	}
	else // ���� ������� �������� ������ ������������
	{
		cout << "���������� ������ ���� ������. ���������� ������ ������" << endl;
		checkgroup();
	}
	system("pause");
	menu();
}

void swap(Group* first_p, Group* second_p) // ����� ���������� ����� �����
{
	Group temp = *first_p;
	*first_p = *second_p;
	*second_p = temp;
}

void print_rating_students() // ���������� ��������� �� �������� ����� �� ��������
{
	system("cls"); // ������� �������
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

void print_info() // ����� �������������� ���������� � ���������
{
	system("cls"); // ������� �������

	int count_male = 0; // ���������� �
	int count_female = 0; // ���������� �
	int count_money = 0; // ���������� ��������� �� ����������
	bool br_flag = FALSE; // ���� ��� ����������� �����
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
				//count_not_money++;
				br_flag = TRUE;
			}
			j++;
		}
		if (br_flag == FALSE and group_all.student[i].form_edu == "den")
			count_money++;
	}
	cout << "Male - " << count_male << endl;
	cout << "Female - " << count_female << endl;
	cout << "���������� ���������, ������� ������� ��������� - " << count_money << endl;

	system("pause");
	menu();
}

void print_student_Scholarship() // ����� ��������� ��� ��������� - �������� �� 4 - �������� �� 5
{
	system("cls"); // ������� �������

	bool br_flag = FALSE;
	int j = 0;

	cout << "������ ��������� ��� ���������: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(8) << left << "Sex" << setw(16) << left << "Form"
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber"
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	for (int i = 0; i < group_count - 1; i++)
	{
		j = 0;
		br_flag = FALSE;
		while (j <= count_of_rates - 1 and br_flag == FALSE)
		{
			if (group_all.student[i].rates[j] <= 3)
			{
				cout << setw(3) << left << i + 1;
				br_flag = TRUE;
				print_student_info(group_all, i);
				cout << endl;
			}
			j++;
		}
	}

	cout << endl << endl;
	cout << "������ ���������, ������� ������ ������ �� 4: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(8) << left << "Sex" << setw(16) << left << "Form"
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber"
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	for (int i = 0; i < group_count - 1; i++)
	{
		if (group_all.student[i].grade == 4)
		{
			cout << setw(3) << left << i + 1;
			print_student_info(group_all, i);
			cout << endl;
		}
	}

	cout << endl << endl;
	cout << "������ ���������, ������� ������ ������ �� 5: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(8) << left << "Sex" << setw(16) << left << "Form"
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber"
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	for (int i = 0; i < group_count - 1; i++)
	{
		if (group_all.student[i].grade == 5)
		{
			cout << setw(3) << left << i + 1;
			print_student_info(group_all, i);
			cout << endl;
		}
	}

	system("pause");
	menu();
}

void print_student_listnumber() // ����� ���������, � ����������� �� �� ������ �� ������, ����������� �������������
{
	system("cls"); // ������� �������
	int list_number;
	bool br_flag = FALSE;

	cout << "Enter the listnumber student: ";
	cin >> list_number;
	for (int i = 0; i < group_count - 1; i++)
	{
		if (group_all.student[i].list_number == list_number)
		{
			cout << setw(3) << left << i + 1;
			print_student_info(group_all, i);
			cout << endl;
			br_flag = TRUE;
		}
	}
	if (!br_flag)
		cout << "ListNumber is not exist" << endl;
	system("pause");
	menu();
}

void print_group_in_file() // ����� ���� ������ � ��������� � ���� Student
{
	system("cls"); // ������� �������
	ofstream fout;
	fout.open("students.txt");

	fout << "������ ���������: " << endl;
	fout << setw(3) << left << "�" << setw(13) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(8) << left << "Sex" << setw(16) << left << "Form"
		<< setw(13) << left << "Rating" << setw(13) << left << "GroupNumber"
		<< setw(15) << left << "ListNumber" << setw(10) << left << "Time" << endl;

	int i = 0;
	while (i <= group_all.fact_count - 1)
	{
		fout << setfill(' ');
		fout << setw(3) << left << i + 1 << setw(12) << left << group_all.student[i].name << " "
			<< setw(12) << left << group_all.student[i].surname << " \t"
			<< setw(10) << left << group_all.student[i].father_name << " \t"
			<< setw(6) << left << group_all.student[i].sex << " \t"
			<< setw(5) << left << group_all.student[i].form_edu << " \t" << setw(5) << right;
		for (int j = 0; j <= count_of_rates - 1; j++)
		{
			fout << group_all.student[i].rates[j] << " ";
		}
		fout << " " << setw(12) << left << group_all.student[i].group_number << " "
			<< setw(10) << left << group_all.student[i].list_number << " ";

		fout << setw(2);
		fout << setfill('0');
		fout << right << group_all.student[i].sys_t.wDay << ".";
		fout << setw(2);
		fout << right << group_all.student[i].sys_t.wMonth << ".";
		fout << setw(2);
		fout << group_all.student[i].sys_t.wYear << " ";
		fout << setw(2);
		fout << right << group_all.student[i].sys_t.wHour << ":";
		fout << setw(2);
		fout << right << group_all.student[i].sys_t.wMinute << ":";
		fout << setw(2);
		fout << right << group_all.student[i].sys_t.wSecond;
		fout << setfill(' ');
		if (group_all.student[i].edit_flag)
			fout << setw(2) << "  edit data" << endl;
		else
			fout << setw(2) << "  add data" << endl;
		i++;
	}
	fout << endl;
	fout << setfill(' ');
	fout.close();

	cout << "������ � ��������� �������� � ����� Student. ����������!" << endl;
	system("pause");
	menu();
}

/// �������������� �������

struct team {
	string TeamName;
	string Surname;
	string Name;
	string FatherName;
	bool gto_order;
	bool captain_flag;
	int year_of_birth;
};

struct kubok {
	int fact_count = 0;
	vector<team> player{ count_of_players };
} Kubok_team;

void kubok_fill_student(kubok &team, ifstream &fin) // ���������� ������� ������� �� ����������
{
	if (fin.is_open())
	{
		int i = 0;
		while (!fin.eof())
		{
			//team.player.resize(count_of_players);
			getline(fin, team.player[i].TeamName, ' ');
			getline(fin, team.player[i].Surname, ' ');
			getline(fin, team.player[i].Name, ' ');
			getline(fin, team.player[i].FatherName, ' ');
			fin >> team.player[i].gto_order;
			fin >> team.player[i].captain_flag;
			fin >> team.player[i].year_of_birth;
			fin.ignore(1);
			i++;
			team.fact_count++;
		}
	}
	else
	{
		cout << "���� �� ������� �������!" << endl;
	}
	fin.close();
}

void kubok_print_student(const kubok &team) // ����� ���� ���������� ����� �������������
{
	system("cls"); // ������� �������
	int i = 0;

	cout << "������ ���������� � ����� �������������: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "TeamName" << setw(16) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(15) << left << "YearOfBirth" << setw(10) << left << "Captain"
		<< setw(13) << left << "GTO" << endl;
	while (i <= team.fact_count - 1)
	{
		cout << setfill(' ');
		cout << setw(3) << left << i + 1 << setw(12) << left << team.player[i].TeamName << " "
			<< setw(12) << left << team.player[i].Name << " \t"
			<< setw(10) << left << team.player[i].Surname << " \t"
			<< setw(8) << left << team.player[i].FatherName << " \t"
			<< setw(8) << left << team.player[i].year_of_birth << " \t";
		if (team.player[i].captain_flag)
			cout << setw(8) << left << "YES" << " ";
		else
			cout << setw(8) << left << "NO" << " ";
		if (team.player[i].gto_order)
			cout << setw(2) << "YES" << endl;
		else
			cout << setw(2) << "NO" << endl;
		i++;
	}

	system("pause");
	menu();
}

void kubok_print_student_info(const kubok& team, int index) // ����� ��������� ����� ������������� �� ������� � �������
{
	cout << endl << endl;
	cout << "������ ��������� ������ 18 ��� �� ������ ���: " << endl;
	cout << setw(3) << left << "�" << setw(13) << left << "TeamName" << setw(16) << left << "Name"
		<< setw(16) << left << "Surname" << setw(16) << left << "FatherName"
		<< setw(15) << left << "YearOfBirth" << setw(10) << left << "Captain"
		<< setw(13) << left << "GTO" << endl;

	cout << setfill(' ');
	cout << setw(3) << left << index << setw(12) << left << team.player[index].TeamName << " "
		<< setw(12) << left << team.player[index].Name << " \t"
		<< setw(10) << left << team.player[index].Surname << " \t"
		<< setw(8) << left << team.player[index].FatherName << " \t"
		<< setw(8) << left << team.player[index].year_of_birth << " \t";
	if (team.player[index].captain_flag)
		cout << setw(8) << left << "YES" << " ";
	else
		cout << setw(8) << left << "NO" << " ";
	if (team.player[index].gto_order)
		cout << setw(2) << "YES" << endl;
	else
		cout << setw(2) << "NO" << endl;
}

void kubok_indidid_zad(const kubok &team) // ����� ��������� ������ 18 ��� �� ������� ���
{
	system("cls");
	int i = 0;
	while (i <= team.fact_count - 1)
	{
		if (team.player[i].captain_flag == TRUE and team.player[i].gto_order == TRUE and (2020 - team.player[i].year_of_birth) < 18)
		{
			kubok_print_student_info(team, i);
		}
		i++;
	}
	system("pause");
	menu();
}

void Check_kubok() // ����� �������� ����� ������� �� "�������������� �������"
{
	system("cls");
	cout << "����� ���������� �� ������ ��������?" << endl;
	cout << "1. ������ ���������� ����� �������������" << endl;
	cout << "2. ������ ��������� ������ 18 ��� �� ������� ���" << endl;
	cout << "3. ����� � ����" << endl;
	
	char value[256]; // ����������, ������� ������ ��������� ��������
	cin >> value; // ������ ��������� �������� 
	if (strlen(value) == 1) // ��������� ���������� ��������� ��������. ���� �����, �� ������ ������ ��� ���, ����� �������� ������
	{
		switch (value[0]) // ��������, ���� ������ ������ ���������� value
		{
		case '1': // ���� �� ����������
			kubok_print_student(Kubok_team);
			break;
		case '2': // ��������, ������ 18 ��� �� ������� ���
			kubok_indidid_zad(Kubok_team);
			break;
		case '3':
			menu();
		default: // ���� ����� �� �������� �� � ������ ��
			cout << "�������� ������� �������. ������� ������" << endl;
			Check_kubok();
			break;
		}
	}
	else // ���� ������� �������� ������ ������������
	{
		cout << "���������� ������ ���� ������. ���������� ������ ������" << endl;
		Check_kubok();
	}
}

void conf_val() // ����� ����� ������������� � ����������� �� �������� ��������
{
	switch (m_count)
	{
	case 0:
		add_student(group_all);
		break;
	case 1:
		change_info_student();
		break;
	case 2:
		print_group_student(group_all);
		break;
	case 3:
		choose_print_group();
		break;
	case 4:
		print_rating_students();
		break;
	case 5:
		print_info();
		break;
	case 6:
		print_student_Scholarship();
		break;
	case 7:
		print_student_listnumber();
		break;
	case 8:
		print_group_in_file();
		break;
	case 9:
		Check_kubok();
		break;
	case 10:
		SetColor(7, 7);
		exit(0);
		break;
	default:
		break;
	}
}

void menu() // ����
{
	system("cls"); // ������� �������

	if (m_count == 0)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		SetColor(5, 7);
		cout << "������� ������ � ��������" << endl;
		SetColor(0, 7);
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 1)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		SetColor(5, 7);
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		SetColor(0, 7);
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 2)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		SetColor(5, 7);
		cout << "��� ������ � ���������" << endl;
		SetColor(0, 7);
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 3)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		SetColor(5, 7);
		cout << "��� ������ � ������" << endl;
		SetColor(0, 7);
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 4)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		SetColor(5, 7);
		cout << "��� ����� ��������� �� �������� �����" << endl;
		SetColor(0, 7);
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 5)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		SetColor(5, 7);
		cout << "���������� �� ���������" << endl;
		SetColor(0, 7);
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 6)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		SetColor(5, 7);
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		SetColor(0, 7);
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 7)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		SetColor(5, 7);
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		SetColor(0, 7);
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 8)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		SetColor(5, 7);
		cout << "����� ������� � ����" << endl;
		SetColor(0, 7);
		cout << "�������������� ������� - ����� �������������" << endl;
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 9)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		SetColor(5, 7);
		cout << "�������������� ������� - ����� �������������" << endl;
		SetColor(0, 7);
		cout << "�����" << endl;
		menu_choice();
	}
	if (m_count == 10)
	{
		cout.width(15);
		cout << right << "__����__" << endl;
		cout << "������� ������ � ��������" << endl;
		cout << "������ ��������� � ������������ ������ ��������" << endl;
		cout << "��� ������ � ���������" << endl;
		cout << "��� ������ � ������" << endl;
		cout << "��� ����� ��������� �� �������� �����" << endl;
		cout << "���������� �� ���������" << endl;
		cout << "�������� ��� ���������; ������ �� 4; ������ �� 5" << endl;
		cout << "������(�) ��������(��) �� ������ � �������" << endl;
		cout << "����� ������� � ����" << endl;
		cout << "�������������� ������� - ����� �������������" << endl;
		SetColor(5, 7);
		cout << "�����" << endl;
		SetColor(0, 7);
		menu_choice();
	}
}

void menu_choice() // � ����������� �� ������� �������� ����
{
	int k1;
	k1 = _getch(); // �������� ������ ������� ��� ������ �����
	if (k1 == 0xE0) // ���� �������
	{
		switch (k1)
		{
		case 0x48: // ������� �����
			m_count--;
			if (m_count < 0) m_count = 0;
			menu();
			break;

		case 0x50: // ������� ����
			m_count++;
			if (m_count > 10) m_count = 10;
			menu();
			break;
		case 0xD: // �����������
			conf_val();
			break;
		default:
			menu_choice();
		}
	}
	switch (k1)
	{
	case 0x48: // ������� �����
		m_count--;
		if (m_count < 0) m_count = 0;
		menu();
		break;

	case 0x50: // ������� ����
		m_count++;
		if (m_count > 10) m_count = 10;
		menu();
		break;
	case 0xD: // �����������
		conf_val();
		break;
	default:
		menu_choice();
	}
}

int main()
{
	ifstream fin;

	HANDLE  hout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD  size;
	size.X = 200;     // ���-�� �������� �� ������
	size.Y = 1001;  // ����������� �� 1000 - �����, ����� ���� 2000
	SetConsoleScreenBufferSize(hout, size);
	setlocale(0, ""); // �����������
	SetConsoleCP(1251); // ����������� ����� ������
	SetConsoleOutputCP(1251); // ����������� ������ ������


	fin.open("gr9894.txt");
	fill_students(gr9894, fin);
	fin.open("gr9893.txt");
	fill_students(gr9893, fin);
	fin.open("team.txt");
	kubok_fill_student(Kubok_team, fin);

	fill_group();
	menu();

	system("pause");
	return 0;
}
