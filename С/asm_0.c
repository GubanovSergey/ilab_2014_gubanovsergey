#define DBG_mode        //!!!This program works in Debug-mode
// supported commands are:
// push <=>   1 (arg is number)
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DBG_mode
    #define DBG
#else
    #define DBG if(0)
#endif

FILE * reader;
FILE * writer;

typedef enum err_codes
{
	All_good,
	Mem_err,
	Len_lim_exc,
	Eof_empstr = 10,
	Eof,
	Not_number = 20,
	Unexp_exc = 99
} err_code_t;
typedef enum expect_words
{
	Anything,
	Cmd_lbl,
	Num_or_Cmd_wo_params,
	Lbl,
	Cmd_reg = 5,
	Num_lbl,
	Num_or_reg
} exp_word_t;

typedef struct something_which_is_not_used_anywhere
	{
	    char * name;
	    unsigned long long place;
	} lblarr_el;

unsigned short int end_flag = 0;
unsigned long long lbl_size = 0, cur_lblnum = 0;

void err_handle(err_code_t err_code)
{
	//TODO check for bugs the situation of empty file
	if (err_code && err_code != 11)
	{
		if (err_code == Mem_err)
        {
            printf ("Haven't enough memory to do the operation");
            fprintf (writer, "Haven't enough memory to do the operation");
            fclose (writer);
            system ("pause");
            exit (0);
        }
        else if (err_code == Len_lim_exc)
        {
            printf ("Too long argument for command or label.\nType not more than 50 next time\n");
            fprintf (writer, "Too long argument for command or label.\nType not more than 50 next time\n");
            fclose (writer);
            system ("pause");
            exit (0);
        }
		else if (err_code == Eof_empstr)
        {
            printf ("End of file reached, but nothing was found\n");
            fprintf (writer, "End of file reached, but nothing was found\n");
            fclose (writer);
            system ("pause");
            exit (0);
        }
		else if (err_code == Not_number)
		{
			printf("Number needed to continue. Input error\nProgram will be closed\n");
			fprintf(writer, "Number expected. Input error\n");
			fclose(writer);
			system("pause");
			exit(0);
		}
		else if (err_code == Unexp_exc)
        {
            printf("An error has been detected and program will be shut down.\n");
		    fclose(writer);
		    system ("pause");
		    exit(0);
        }
        else
		{
			DBG printf("Function hasn't been realized yet. Errcode is %hd\n", err_code);
			fclose(writer);
			system("pause");
			exit(0);
		}
	}
}

lblarr_el* lbls_ptr = NULL;

err_code_t getword(char *);

err_code_t getnum(double *, char *);

void add_lbl(char * name, unsigned long long place)
{
    unsigned short symb_cnt = 0, len = strlen(name);
    if (cur_lblnum == lbl_size)
    {
        lblarr_el* newptr = (lblarr_el*)realloc(lbls_ptr, (lbl_size += 10));
        if (!newptr) err_handle (Mem_err);
        lbls_ptr = newptr;
    }
    lbls_ptr[cur_lblnum].name = (char*)malloc((len + 1)*sizeof(char));
    for (symb_cnt = 0; symb_cnt < len; symb_cnt++)
    {
        lbls_ptr[cur_lblnum].name[symb_cnt] = name [symb_cnt];
    }
    lbls_ptr[cur_lblnum].name[len] = '\0';

    lbls_ptr[cur_lblnum].place = place;
    cur_lblnum++;
}

unsigned long long find_lbl_trg(char * lbl_name)
{
    assert(lbl_name);
    lbl_name++;         //skip :

    unsigned long long lbl_cnt = 0;
    for (lbl_cnt = 0; lbl_cnt < cur_lblnum; lbl_cnt++)
        if (strcmp(lbl_name, lbls_ptr[lbl_cnt].name) == 0)
            return lbls_ptr[lbl_cnt].place;
    if (lbl_cnt == cur_lblnum)
    {
        printf("Label wasn't declared, but was used: %s\n", lbl_name);
        fprintf(writer, "Label wasn't declared, but was used");
        system("pause");
        fclose(writer);
        exit(0);
    }
}

int main()
{
	reader = fopen("asm.txt", "r");
	writer = fopen("asm_res.txt", "w");
	fprintf(writer,"                         \n");                                                   //empty place for number of commands
	char str[51] = {0};
	int c = 0, len = 0, i = 0;
	double a = 0;
	unsigned long long cmd_num = 0;

    lbls_ptr = (lblarr_el*)calloc(10, sizeof(lblarr_el));
    if (!lbls_ptr) err_handle (Mem_err);
	lbl_size = 10;

	err_code_t err_code = All_good;
	exp_word_t expected = Anything;
	unsigned short int com_flag = 1;

	//-----------------------------------------------------------------------------
	// 0 - Anything                                                               - pre-read
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
	// 1 - command or label                         = Cmd_lbl                     |
    // 2 - command without parameters or number     = Num_or_Cmd_wo_params        ~  =]-------------\
    // 3 - label                                    = Lbl                         |  /      - -      \\
	// 5 - register or command                      = Cmd_reg                     |=|  [ post-read ]  >>
	// 6 - number or label                          = Num_lbl                     |  \      - -      //
	// 7 - number or register                       = Num_or_reg                  ~  =]-------------/
	//-----------------------------------------------------------------------------

	while ( !end_flag && ((err_code = getword(str)) == All_good || err_code == Eof) )
    {
        if (str[(len = strlen (str)) - 1] == ':')
        {
            DBG
            {
                printf("New label %s", str);
                system("pause");
            }
            str[len - 1] = '\0';
            add_lbl(str, cmd_num + 1);
        }
        else cmd_num++;
    }

    DBG
    {
        for (i = 0; i < cur_lblnum; i++)
            printf("Lbl[%d]:\n\tName = %s;\n\tPlace = %llu\n", i, lbls_ptr[i].name, lbls_ptr[i].place);
    }
    system("pause");
    fseek(reader, 0, 0);
    end_flag = 0, cmd_num = 0;

	while ( expected != Anything || (!end_flag && ((err_code = getword(str)) == All_good || (err_code == Eof))) )
	{
	    com_flag = 1;
	    if (!err_code)
			expected = Cmd_lbl;
		else if (err_code == Eof)
            expected = Num_or_Cmd_wo_params;

		if (str[0] == 'p')
		{
			if (strcmp("push", str) == 0)
			{
				if (expected == Num_or_Cmd_wo_params)
				{
					printf("\nExpected command without parameters, but push was found\n");
					fprintf(writer, "Expected command without parameters, but push was found\n");
					system("pause");
					fclose(writer);
					exit(0);
				}
				cmd_num++;
				expected = Num_or_reg;
				//if push - number of cmd is positive - no arithmetic sign is needed
			}
			else if (strcmp("pop", str) == 0)
			{
			    cmd_num++;
			    fprintf(writer, "-");
			    if (expected != Num_or_Cmd_wo_params)
                    expected = Cmd_reg;
                else
                    fprintf(writer, "1");
			}
            else if (strcmp("peek", str) == 0)
                fprintf(writer, "-26"), cmd_num++;
            else if (strcmp("print", str) == 0)
                fprintf(writer, "-25"), cmd_num++;
			else com_flag = 0, expected = Lbl;
		}
		else if (str[0] == 'a')
		{
			if (strcmp("add", str) == 0)
			{
				fprintf(writer, "10"), cmd_num++;
			}
			else com_flag = 0, expected = Lbl;
		}
		else if (str[0] == 'm')
		{
			if (strcmp("mul", str) == 0)
			{
				fprintf(writer, "11"), cmd_num++;
			}
			else com_flag = 0, expected = Lbl;
		}
		else if (str[0] == 'c')
        {
            if (strcmp("call", str) == 0)
            {
                fprintf(writer, "20 "), cmd_num++;
                expected = Num_lbl;
            }
            else com_flag = 0, expected = Lbl;
        }
		else if (str[0] == 'j')
        {
            if (strcmp("jmp", str) == 0)
            {
                fprintf(writer, "15 "), cmd_num++;
                expected = Num_lbl;
            }
            else com_flag = 0, expected = Lbl;
        }
        else if (str[0] == 'r')
        {
            if (strcmp("ret",str) == 0)
            {
                fprintf(writer, "-20 "), cmd_num++;
            }
        }
		else com_flag = 0, expected = Lbl;
		DBG printf("err_code = %2d, end_flag = %d, read command = %s, expected = %d\n", err_code, end_flag, str, expected);
		//-----------------------------------------------------------------------------
		if (!com_flag && expected == Lbl)
		{
			if (str[(len = strlen(str)) - 1] != ':')
            {
			    DBG printf("This is not a command or label: %s Input error\n", str);
                fprintf(writer, "Not a command or label. Input error\n");
                fclose(writer);
                system("pause");
                exit(0);
			}
			else
                expected = Anything;
		}
		else if (expected == Num_or_reg || expected == Cmd_reg)
        {
            if (end_flag || (err_code = getnum(&a, str)) == Not_number)
            {
                DBG printf("Push/pop param isn't a number\n");
                if (str[1] == 'x' && str[0] >= 'a' && str[0] <= 'd' && str[2] == '\0')     //Register?
                {
                    fprintf(writer, "2 %hd", (unsigned short)(str[0]-'a'+1));
                    cmd_num++;
                    expected = Anything;
                    DBG printf("err_code = %2d, end_flag = %d, read command = %s, expected = %d\n", err_code, end_flag, str, expected);
                }
                else
                {
                    if (expected == Num_or_reg)
                    {
                        fprintf(writer, "Register or ");
                        printf("Register or ");
                        err_handle (Not_number);
                    }
                    else
                    {
                        fprintf(writer, "1\n");     //Pop w/o params, '-' has been already printed
                        if (end_flag)
                            expected = Num_or_Cmd_wo_params;
                        else
                            expected = Cmd_lbl;
                        DBG printf("err_code = %2d, end_flag = %d, read command = %s, expected = %d\n", err_code, end_flag, str, expected);
                        continue;
                    }
                }
            }
            else            //Number
            {
                if (expected == Num_or_reg)
                {
                    fprintf(writer, "1 %lg", a);
                    cmd_num++;
                    expected = Anything;
                }
                else         //exp = Cmd_reg
                {
                    fprintf(writer, "Error! Register was expected, but %lg was found\nProgram will be closed\n", a);
                    printf("Error! Register was expected, but %lg was found\nProgram will be closed\n", a);
                    system("pause");
                    fclose(writer);
                    exit(0);
                }
            }
        }
        else if (expected == Num_lbl)
        {
            if (end_flag || (err_code = getnum(&a, str)) == Not_number)
            {
                if (end_flag)                                  //End of file handling
                {
                    fprintf(writer, "Label or ");
                    printf("Label or ");
                    err_handle (Not_number);
                }
                if (str[0] != ':')
                {
                    DBG printf("This is not a command or label: %s Input error\n", str);
                    fprintf(writer, "Not a command or label. Input error\n");
                    fclose(writer);
                    system("pause");
                    exit(0);
                }
                else
                {
                    fprintf(writer,"%llu", find_lbl_trg(str));
                    expected = Anything;
                    cmd_num++;
                }
            }
            else
            {
                fprintf(writer, "%lg", a);
                cmd_num++;
                expected = Anything;
            }
        }
        else
            expected = Anything;
		if (!end_flag)
            fprintf(writer, "\n");
    }
	fclose(reader);
	fseek(writer, 0, 0);
	fprintf(writer, ".%llu", cmd_num);
	fclose(writer);
	printf("Program has successfully finished!\n");
	return 0;
}

err_code_t getword(char * word_ptr)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               //                                                                                                                                                                                                                                                                                     //                                                                                                                                                                                                                                   //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          //                                                                                                                                                                                                                                                     /                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        //
{
	int symb = 0;
	unsigned short int nsymb = 0;
	while ((symb = fgetc(reader)) != EOF && (symb == ' ' || symb == '\n' || symb == '\t'));
	if (symb == EOF)
	{
		if (nsymb == 0)
		{
			err_handle(Eof_empstr);
			end_flag = 1;
			return Eof_empstr;
		}
		else
		{
		    err_handle(Unexp_exc);
			return Unexp_exc;
		}
	}
	else
		ungetc(symb, reader);

	while ((symb = fgetc(reader)) != ' ' && symb != '\n' && symb != '\t' && symb != EOF && nsymb < 50)
		word_ptr[nsymb++] = symb;


	if (nsymb == 50 && !(symb == ' ' || symb == '\n' || symb == '\t'))
	{
		err_handle(Len_lim_exc);
		return Len_lim_exc;
	}

	word_ptr[nsymb] = '\0';
	while ((symb = fgetc(reader)) != EOF && (symb == ' ' || symb == '\n' || symb == '\t'));
	if (symb == EOF)
    {
        end_flag = 1;
        return Eof;
    }
	else
	{
		ungetc(symb, reader);
		return All_good;
	}
}
                                  //NULL or str
err_code_t getnum(double * num_ptr, char str_ptr[51])
{
	char* word_ptr = NULL;
	if (str_ptr)
        word_ptr = str_ptr;
    else
        word_ptr = (char*)malloc(51 * sizeof(*word_ptr));

	if (!word_ptr) err_handle(Mem_err);
	err_code_t err_code = getword(word_ptr);
	err_handle(err_code);

	char* ptr = NULL;
	double number = strtod(word_ptr, &ptr);

	if (ptr && (*ptr))
	{
		printf("Input error. Number was preferred to be read, but %c [code %d] was found\nPrint '.' to exit\n", *ptr, *ptr);
		char c = getchar();
		if (c == '.')
		{
			fprintf(writer, "Reading was aborted by user");
			fclose(writer);
			exit(0);
		}
		return Not_number;
	}
	else
	{
		(*num_ptr) = number;
		if (!str_ptr)
            free(word_ptr);
		return err_code;
	}
}
