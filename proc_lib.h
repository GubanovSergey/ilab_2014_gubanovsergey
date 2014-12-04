#include "stack1.h"

#ifdef DBG_mode
    #define DBG ;
#else
    #define DBG if(0)
#endif

const unsigned long long DEF_0 = 'def_0';

typedef struct processor
{
    stk_t* stkptr;
    st_type reg_ax;
    st_type reg_bx;
    st_type reg_cx;
    st_type reg_dx;
} proc_t;
static proc_t proc = {NULL, DEF_0, DEF_0, DEF_0};

void init_proc(stk_t* stkadd)
{
    proc.stkptr = stkadd;
}

void stack_add(stk_t* stkadd)
{
    st_type add1 = stack_pop(stkadd), add2 = stack_pop(stkadd);
    stack_push(add1+add2, stkadd);
}
void stack_mul(stk_t* stkadd)
{
    st_type mul1 = stack_pop(stkadd), mul2 = stack_pop(stkadd);
    stack_push(mul1*mul2, stkadd);
}
void push_reg(unsigned short int reg, stk_t* stkadd)
{
    st_type reg_val = DEF_0;
    int symb = 0;
    switch (reg)
    {
    case 1:
        reg_val = proc.reg_ax;
        break;
    case 2:
        reg_val = proc.reg_bx;
        break;
    case 3:
        reg_val = proc.reg_cx;
        break;
    case 4:
        reg_val = proc.reg_dx;
        break;
    default:
        printf("Register is wrong. May be it was an error in assembling or file has been changed");
        system("pause");
        exit(0);
        break;
    }

    if (reg_val == DEF_0)
    {
        printf("\nWarning!!!\nYou probably trying to push from register, but haven't put there any variable\n\n");
        system("pause");
        DBG
        {
            printf("\nPrint '.' to exit. Otherwise program will be continued\n");
            symb = getchar();
            if (symb == '.')
            {
                printf("Program was aborted by user");
                exit(0);
            }
        }
    }
    stack_push(reg_val, stkadd);
}
void pop_reg(unsigned short int reg, stk_t* stkadd)
{
    st_type stk_top = stack_pop(stkadd);
    switch (reg)
    {
    case 1:
        proc.reg_ax = stk_top;
        break;
    case 2:
        proc.reg_bx = stk_top;
        break;
    case 3:
        proc.reg_cx = stk_top;
        break;
    case 4:
        proc.reg_dx = stk_top;
        break;

    default:
        printf("Register is wrong. May be it was an error in assembling or file has been changed");
        system("pause");
        exit(0);
        break;
    }
}
