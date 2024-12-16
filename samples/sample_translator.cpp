#include <iostream>
#include "Translator.h"

using namespace std;

int main() {
	setlocale(2, "rus");
	try
	{
		string temp;
		cout << "������� �������������� ��������� - ";
		cin >> temp;
		Translator t(temp);
		cout << "����� = " << t.calculate();
	}
	catch (const runtime_error& error)
	{
		cout << "runtime_error: " << error.what() << endl;
	}
	catch (const logic_error& error)
	{
		cout << "logic_error: " << error.what() << endl;
	}
}