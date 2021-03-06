#define DBG_mode   //!!!This program works in Debug-mode - description is in stack_proc.h
// supported commands are:
// push <=>   1 (arg is a number)
// pop  <=>  -1 (no arg)
// push <=>   2 (arg is register)
// pop  <=>  -2 (arg is register)
// add  <=>  10
// mul  <=>  11
// jmp  <=>  15
// call <=>  20
// ret  <=> -20
// peek <=> -26
// print<=> -25

#include "stack_proc.h"  //proc is a structure which includes stack pointer (initialized NULL) and registers (ax,bx,cx,dx)
extern proc_t proc;         //definition is in stack_proc.h
#define proc_t  //using processor type is not allowed
#include <math.h>

/* included libs are
stdlib, stdio, assert, Windows.h, math
*/

/*#define END_MESSAGE \
system("pause");\
printf("\n"); */

FILE * reader = fopen("asm_res.txt", "r");

void savecmd_arr(double* cmd_ptr, unsigned long long cmd_num)
{
    assert(reader);
    unsigned long long int i = 0;
    int symb;
    while (!feof(reader))
    {
        if (i == cmd_num)                           //Handling the end of file
        {
            while ((symb = fgetc(reader)) != EOF && (symb == ' ' || symb == '\n' || symb == '\t'));
            if (symb != EOF)
            {
                printf("Number of arguments not as expected. May be there was an error in assembling or file has been changed. Program will be closed\n");
                system("pause");
                exit(0);
            }
            else
                break;
        }

        if (fscanf(reader, "%lg", &(cmd_ptr[i])) == 0)
        {
            printf("Input error. File can be damaged or changed\n");
            DBG printf("Number of read commands = %llu\n", i);

            system("pause");
            exit(0);
        }
        i++;
    }
    cmd_ptr[i++] = 0;

    if (i != cmd_num + 1)
    {
        printf("Number of arguments not as expected. May be there was an error in assembling or file has been changed. Program will be closed\n");
        system("pause");
        exit(0);
    }

    DBG
    {
        printf("[DBG] Commands written to array:\n");
        for (i = 0; i <= cmd_num; i++)
            printf("%lg\n",cmd_ptr[i]);
        system("pause");
    }
}

int main()
{
    //const DEF_0 initialized in stack_proc.h
    /*
    // ...
    transit.i = 3;
    stack_t hren ;
    push(hren, transit.d);
    // ...
    transit.d = pop(hren);
    (transit.i == 3) = true!!!
    */
    unsigned long long int cmd_arrsize = DEF_0, cmd_cnt = 0;
    unsigned short int st_succ = fscanf(reader, ".%llu", &cmd_arrsize);
    DBG printf("cmd_num was found = %hd, number of cmds = %llu\n",st_succ, cmd_arrsize);
    if (!st_succ || cmd_arrsize == DEF_0 || !reader)
    {
        printf("File is damaged, or assembling wasn't successful\n");
        system("pause");
        exit(0);
    }

    double* cmd_arr_ptr = (double*)calloc(cmd_arrsize + 1, sizeof(*cmd_arr_ptr));
    if (!cmd_arr_ptr)
    {
        printf("Haven't enough memory to read commands");
        system("pause");
        exit(0);
    }
    savecmd_arr(cmd_arr_ptr, cmd_arrsize++);
    fclose(reader);

    DBG
    {
        printf("Commands list:\n");
    // push <=>   1 (arg is a number)
    // pop  <=>  -1 (no arg)
    // push <=>   2 (arg is register)
    // pop  <=>  -2 (arg is register)
    // add  <=>  10
    // mul  <=>  11
    // peek <=> -26
    // print<=> -25
        printf("Push  (number)  code is   1\n");
        printf("Push (register) code is   2\n");
        printf("Pop             code is  -1\n");
        printf("Pop  (register) code is  -2\n");
        printf("Add             code is  10\n");
        printf("Mul             code is  11\n");
        printf("Peek            code is -26\n");
        printf("Print           code is -25\n");
        system("pause");
    }

    double* cur_cmdptr = cmd_arr_ptr;
    int cur_cmd = lrint(*(cur_cmdptr));

    stk_t* stkadd = stack_ctor();
    stk_t* call_stack = stack_ctor();
    union poor_double
    {
        double* ret_ind;
        double d;
    } transit = {0};

    init_proc(stkadd);

    while (cur_cmd != 0 && (assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize), *cur_cmdptr))
    {
        cmd_cnt++;
        switch ((cur_cmd = (int)(*(cur_cmdptr++)), cur_cmd))
        {
            case 1:
                assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize);
                stack_push(*(cur_cmdptr++), stkadd);
                break;
            case -1: stack_pop(stkadd); break;
            case 2:
                assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize);
                push_reg (lrint(*(cur_cmdptr++)), stkadd);
                break;
            case -2:
                assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize);
                pop_reg (lrint(*(cur_cmdptr++)), stkadd);
                break;
            case 10: stack_add(stkadd); break;
            case 11: stack_mul(stkadd); break;
            case -26:
                printf("Requested element = %lg\n", stack_peek(stkadd));
                break;
            case -25: print_stack(stkadd); break;
            case 15:
                assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize);
                cur_cmdptr = cmd_arr_ptr + (llrint(*cur_cmdptr) - 1);
                break;
            case 20:
                assert (cur_cmdptr >= cmd_arr_ptr && cur_cmdptr < cmd_arr_ptr + cmd_arrsize);
                transit.ret_ind = cur_cmdptr + 1;
                stack_push(transit.d, call_stack);
                cur_cmdptr = cmd_arr_ptr + (llrint(*cur_cmdptr) - 1);
                break;
            case -20:
                if (stack_check(call_stack) == EMPTY_STACK)
                {
                    printf("\nYou tryed to return, but haven't gone into the called function\n");
                    printf("Check your commands. Program will be closed\n");
                    system("pause");
                    exit (0);
                }
                transit.d = stack_pop(call_stack);
                cur_cmdptr = transit.ret_ind;
                break;
            case 0: break;

            default:
                printf("No such operation! Program will be shut down.\n");
                system("pause");
                exit(0);
                break;
        }
        DBG
           {
               printf("Cmd = %d, stack_top = %lg", cur_cmd, stack_peek(stkadd));
               system("pause");
           }
    }
    printf("\nProgram has successfully finished\nStack in finish condition:\n");
    print_stack(stkadd);
    return 0;
}
