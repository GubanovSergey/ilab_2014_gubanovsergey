#include "stack++.h"

int main()
{
    unsigned long long i = 0;
    Stack <double> s1;
    std::cout << "Stack empty place " << s1.Stack_place() << std::endl;
    system("pause");

    for (i = 0; i < 160; i++)
        s1.Stack_push(i * 15 % 7 + 0.053 * i * i * 3);
    s1.Stack_print();
    std::cout << "Stack empty place after pushing " << s1.Stack_place() << std::endl;
    system("pause");

    for (i = 0; i < 40; i++)
        s1.Stack_pop();
    s1.Stack_print();
    std::cout << "Stack empty place after popping " << s1.Stack_place() << std::endl;
    system("pause");

    if (s1.Stack_place() >= 70)
        s1.Stack_redsize(70);
    s1.Stack_print();
    std::cout << "Stack empty place resize " << s1.Stack_place() << std::endl;
    system("pause");
    return 0;
}
