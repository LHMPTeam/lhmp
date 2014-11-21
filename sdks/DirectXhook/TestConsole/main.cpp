#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void Test()
{
	cout << "hello";
}
int main()
{
	char daco[] = "ahoj";
	printf(daco);
	string test = "Ahoj";
	char Pismeno = '0';
	printf("Adresa funkcie: %X",&Test);
	test = test + Pismeno;
	cout << test;
	cin.get();
	cin.get();
	return 0;
}
