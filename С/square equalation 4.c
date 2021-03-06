//программа для решения квадратного уравнения с фильтрацией ввода. дожидается ввода строго 3 коэффициентов,\
в случае некорректности предлагает вариант исправления, также умеет перезапускаться\
в случае согласия с вопросом введите пустую строку, иначе - "."

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;


double a = 0, b = 0, c = 0, x1 = 0, x2 = 0, d = 0;                 //Объявляем переменные коэффициентов, корней и дискриминанта
char a1[16], b1[16], c1[16];                                       //массивы символов для считывания коэффициентов кв. уравнения
short int n;                                                       //количество корней
bool f = 1, flag = 1, fp = 0;                                      //переменная-флаг необходимости продолжения программы, фдаг корректности вводимого символа и флаг встречи десятичной точки

bool corsymb(const char* cc, const char* cp, const int i)          //функция проверки корректности вводимого символа
{
	bool result = 1;
	if (i == 1)
	{
		result = *cc == '-' || *cc >= '0' && *cc <= '9';   //1-й символ - цифра или минус
	}
	else if (i == 2 && *cp == '-')
	{
		result = *cc >= '0' && *cc <= '9';                 //если 1-й - минус, то 2-й цифра
	}
	else result = (*cc >= '0' && *cc <= '9') || (!fp && *cc == '.') || *cc == ' ' || *cc == 10;
	//допускаются цифры, пробел, перевод строки, десятичная точка (если встречается впервые)
	if (!result) flag = 0;
	if (*cc == '.') fp = 1;
	return result;
}

void readnum(char* arr, int i)                                     //функция чтения очередного коэффициента
{
	bool fc = 0;                                               //флаг перехода от некорректного символа к корректному
	char c;                                                    //очередной символ
	fp = 0;

	do
	{
		c = getchar();
	} while (!(corsymb(&c, arr - 1, i)));
	fc = 1;
	//"проглотить" все неверные символы в начале чтения коэффициента, запомнить, что посл. символ корректен

	while (c != ' ' && c != 10)   //чтение коэффициента должно оканчиваться пробелом или переводом строки
	{
		if (fc || corsymb(&c, arr - 1, i))   //если считанный символ корректен или уже был проверен
		{
			if (fc) fc = 0;
			*arr = c;
			c = getchar();
			arr++;
			i++;   //запомнили символ, перешли к следующему
		}
		else
		{
			do
			{
				c = getchar();
			}	while (!(corsymb(&c, arr - 1, i)));
			fc = 1;
			//"проглотить" все неверные символы, запомнить, что посл. символ корректен
		}
	}
	if (c == ' ' && (arr - i + 1 == c1))
	{
		do {
			c = getchar();
		} while (c != 10);
	}
	return;
}
void read()   //функция чтения коэффициэнтов и их подготовке к расчетам, также выводит сообщение о некорректности ввода (если так и было)
{
	char s[2];
	int i = 1;    //номер считываемого символа
	memset(a1, 0, 16); memset(b1, 0, 16); memset(c1, 0, 16);
	printf("Input 3 coefficients\n");    //программа просто будет ждать ввода всех трех коэффициентов
	readnum(a1,i);
	readnum(b1,i);
	readnum(c1,i);

	if (!flag)
	{
		printf("You've been mistaken.\nDo you want to change it to\n%s %s %s?\n", a1, b1, c1);
		gets(s);
		f = abs(strcmp(s, "."));    //возвращает 1 при согласии пользователя, 0 - при отказе
	}
	a = atof(a1);
	b = atof(b1);
	c = atof(c1);
}

int numroots()     //считаем количество корней
{
	if ((a == 0 && b == 0 && c != 0) || (d<0)) { return 0; }
	else if (a == 0 && b == 0 && c == 0) { return 3; } //Так как больше 2 корней в квадратном уравнении быть не может
	else if (a == 0 || d == 0)
	{
		if (d == 0)     //для случая нул дискриминанта приводим выражение -b/(2a) к виду -c/b
		{
			swap(a, b);
			swap(a, c);
			b = b * 2;
		}
		return 1;
	}
	else return 2;
}

void countroots()   //функция подсчета корней
{
	if (n == 1) { x1 = -c / b + 0.0; }
	else if (n == 2)
	{
		d = sqrt(d);
		x1 = (-b - d) / (2 * a) + 0.0;
		x2 = (-b + d) / (2 * a) + 0.0;
	}
}

void printroots()   //функция вывода корней и перезапуска программы по желанию пользователя
{
	switch (n)
	{
		case 0: printf("No roots\n"); break;
		case 1: printf("There is 1 root: %lg\n", x1); break;
		case 2: printf("There are 2 roots: %lg and %lg\n", x1, x2); break;
		case 3: printf("Uncountable roots amount\n");
	}
	printf("Do you want to continue?\n");
	char s[4];
	gets(s);
	f = abs(strcmp(s, "."));    //возвращает 1 при согласии пользователя, 0 - при отказе
	return;
}


int main()
{
	while (f)
	{
		read();
		if (f == 1)
		{
			d = b * b - 4 * a * c;
			n = numroots();
			countroots();
		}
		else n = -1;
		printroots();
	}
	return 0;
}
