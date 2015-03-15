#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Windows.h>
typedef unsigned long long int ulli;
typedef double st_type;
const unsigned short int EMPTY_STACK = 0, OK = 10;
typedef struct elem
{
	st_type val;
	elem* prevadd;
	unsigned short int el_cond;
} stelem_t;
typedef struct stk
{
	ulli counter;
	stelem_t* ladd;
} stk_t;

stk_t* stack_ctor()
{
	stelem_t* fadd = (stelem_t*)calloc(1, sizeof(stelem_t));
	if (!fadd)
	{
		printf("\nHave not enough memory to create new stack. Construction will be denied\n");
		system("pause");
		return NULL;
	}
	fadd->val = 0;
	fadd->prevadd = NULL;
	fadd->el_cond = 0;
	stk_t* res = (stk_t*)calloc(1, sizeof(stk_t));
	if (!res)
	{
		printf("\nHave not enough memory to create new stack. Construction will be denied\n");
		free(fadd);
		system("pause");
		return NULL;
	}
	res->counter = 0;
	res->ladd = fadd;
	return res;
}

void print_stack(stk_t*);

unsigned short int address_check(stelem_t* t_stelem, ulli counter, unsigned short int ifpr_acc)
{
	if (!(t_stelem->el_cond) || !(t_stelem->prevadd))
	{
		if (!(t_stelem->el_cond) && !(t_stelem->prevadd) && !(t_stelem->val) && !counter)
			return EMPTY_STACK;
		else if (counter && !(t_stelem->el_cond))
		{
			printf("\nCounter error. You shouldn't change inside counter value during the program.\nProgram will be closed\n");
			printf("\ncounter = %llu but stack is empty\n", counter);
			system("pause");
			exit(0);
		}
		else
		{
			printf("\nNot a stack element!!!\n");
			system("pause");
			assert(("Trying to crack a stack!", 0));
		}
	}
	else if ((t_stelem->el_cond) > 2 || !(t_stelem->prevadd) || IsBadReadPtr((t_stelem->prevadd), sizeof(stelem_t)))
	{
		printf("\nNot a stack element!!!\n");
		system("pause");
		assert(("Trying to crack a stack!", 0));
	}
	else if (ifpr_acc && (t_stelem->el_cond) == 2)
	{
		printf("\nArray structure error.\nModifying stack is dangerous. Use functions to operate with it\nProgram will be closed\n");
		system("pause");
		exit(0);
	}
	else if ( (!counter) && (t_stelem->el_cond) )
	{
		printf("\nCounter error. You shouldn't change inside counter value during the program.\nProgram will be closed\n");
		printf("\ncounter = %llu but stack is not empty or damaged\n", counter);
		system("pause");
		exit(0);

	}
	else if ((t_stelem->el_cond) == 1 && (!ifpr_acc))
	{
		printf("\nStack access violation. You have access only to last stack element.\nProgram will be closed\n");
		system("pause");
		exit(0);
	}
	else return OK;
}

unsigned short int stack_check(stk_t* stkadd)
{
	stelem_t* t_stelem = NULL;
	if ((!stkadd) || IsBadReadPtr(stkadd, sizeof(stk_t)) || !(stkadd->ladd) || IsBadReadPtr((stkadd->ladd), sizeof(stelem_t)))
	{
		printf("\nInvalid stack or last stack element pointer\n");
		system("pause");
		assert(("Memory access error", 0));
	}
	else
	{
		t_stelem = stkadd->ladd;
		return address_check(t_stelem, stkadd->counter, 0);
	}
}

void stack_push(st_type value, stk_t* stkadd)
{
	stack_check(stkadd);
	stelem_t* lastadd = stkadd->ladd;
	stelem_t* newadd = (stelem_t*)calloc(1, sizeof(stelem_t));
	if (!newadd)
	{
		printf("\nHave not enough memory to push new element. Push will be denied\n");
		system("pause");
		return;
	}
	newadd->prevadd = lastadd;
	newadd->val = value;
	newadd->el_cond = 2;
	if (lastadd->el_cond) (lastadd->el_cond)--;
	stkadd->counter = (stkadd->counter) + 1;
	stkadd->ladd = newadd;
}

st_type stack_peek(stk_t* stkadd)
{
	if (stack_check(stkadd) == EMPTY_STACK)
    {
        printf("No number at the top (empty stack) - returned 0\n");
        return 0;
    }
    else return (stkadd->ladd->val);
}

st_type stack_pop(stk_t* stkadd)
{
	if (stack_check(stkadd) == EMPTY_STACK)
	{
		printf("You can't pop from empty stack.\nPop will be denied (returned 0)\n");
		system("pause");
		return 0;
	}
	st_type res = stkadd->ladd->val;
	stelem_t* lastadd = stkadd->ladd;
	stelem_t* deladd = lastadd;								  //проверка входных данных
	lastadd = lastadd->prevadd;
	if (lastadd->el_cond) (lastadd->el_cond++);
	free(deladd);
	stkadd->counter = (stkadd->counter) - 1;
	stkadd->ladd = lastadd;
	return res;
}

void print_stack(stk_t* stkadd)
{
	stack_check(stkadd);
	unsigned short ifpr_acc = 0;
	stelem_t* curadd = stkadd->ladd;
	ulli counter = stkadd->counter, coun = 0;
	unsigned short print_flag = 0;
	while (curadd && (curadd->el_cond) && (curadd->prevadd) && (coun < (stkadd->counter)))
	{
		if (!print_flag)
		{
			print_flag = 1;
			ifpr_acc = 1;
			printf("\n");
		}
		printf("stk[%llu] = %lg\n", coun, (curadd->val));
		address_check(curadd->prevadd, --counter, ifpr_acc);
		curadd = (curadd->prevadd);
		coun++;
	}
	if (coun != (stkadd->counter))
	{
		printf("\nUnexpected stack error.\nCounter or stack structure may be damaged.\nProgram will be closed\n");
		system("pause");
		exit(0);
	}
	else if (!print_flag) printf("Empty stack\n\n");
	else printf("Printed %4llu elements\n\n", coun);
	system("pause");
}

void stack_dtor(stk_t* stkadd)
{
	stack_check(stkadd);
	stelem_t* ladd = stkadd->ladd;
	ulli counter = (stkadd->counter), coun = 0;
	while (ladd && (ladd->el_cond) && (ladd->prevadd) && (stkadd->counter) && (coun < counter))
        stack_pop(stkadd);
	if (stack_check(stkadd) == EMPTY_STACK)
	{
		free(stkadd->ladd);
		free(stkadd);
		stkadd = NULL;
		printf("\nStack was successfully deleted. Pointer is not available anymore.\n");
		system("pause");
	}
	else
	{
		printf("\nAn error has been detected and your program will be shut down.\nPlease, contact to program developer.\n");
		system("pause");
		exit(0);
	}
}
