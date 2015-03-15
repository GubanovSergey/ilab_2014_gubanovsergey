/*This program is a composition of 10 programs from control test 2007 and 2008 years.
Now it have some memory problems so it's strongly recommended to exit the program after hard tests of "Prime" and "Prime plural"
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define description printf("\
     Choose the program:    \n\
     0 - Exit from program  \n\
     1 - Space_delete       \n\
     2 - 4-decimal_nums     \n\
     3 - Decomposing_gen    \n\
     4 - 4_squares          \n\
     5 - unrepeating_numbers\n\
     6 - binary_good_words  \n\
     7 - binary_big_summary \n\
     8 - Decomposing w/o rep\n\
     9 - Prime              \n\
    10 - Prime_plural       \n\
    ");                       \
    scanf("%hd", &func_num);

//_________________________________________________________________________________________________
/*  Space_delete
 This programs deletes excessive spaces in input string
<=100 000 symbols - reading to EOF
Year 2008, variant 2, problem C*/
void space_del()
{
    int c = ' ';
    while (c != EOF)
    {
        while (c != ' ')
        {
            c = getchar ();
            printf ("%c",c);
        }
        while ((c = getchar()) == ' ');
        if (c != EOF) printf("%c",c);
    }
}

//_________________________________________________________________________________________________
/*  4-decimal_nums
This program finds all 4-decimal numbers, which decimals summary is equal to input variable
0<=N<=10000
Year 2008, Variant 7, Problem C*/
unsigned short int decsum(const int N)
{
    short int n1, n2;
    n1 = N/1000;
    n1 += N/100-10*n1;
    n2 = N % 10;
    n2 += (N % 100 - n2)/10;
    return n1+n2;
}

void four_dec_numbers()
{
    unsigned int sum, st = 1000, fin = 9999, counter = 0;
    unsigned short int flag = 0;
    scanf("%u", &sum);
    if (sum<1 || sum>36)
    {
        printf ("0");
        return;
    }
    else
    if (sum ==1 || sum == 36)
    {
        if (sum ==1)
        {printf("1000\n1");}
        else printf ("9999\n1");
        return;
    }
    else if (sum>=28)
    st = 8999 - (35-sum)*1000;

    if (sum<=9) fin = sum*1000;

    for (int i = st; i<=fin; i++)
    {
        if (decsum(i) == sum)
        {
            if (!flag)
            {
                printf ("%u", i);
                flag = 1;
            }
            else printf (" %u", i);
            counter++;
        }
    }
    printf ("\n%u", counter);
    return;
}

//_________________________________________________________________________________________________
/*  Decomposing_gen
This program decomposes an input number (from 1 to 40) into a summary of natural numbers. Program accepts much more number, but it takes much time.
Prints all combinations of non-raising successions of addents.
There must not be 3 equal addents in any succession.
Year 2008, Variant 2, Problem D*/

#define chunrais(a, b, c, deep) ((deep == 0) ? a : ((a < b) ? a : ((deep == 1) ? b : ((b != c) ? b : (b - 1)))))
#define last arr[deep-1]
#define llast arr[deep-2]

long counter = 0;
void global_eraze3()
{
    counter = 0;
}

void printi (short* arr, short deep)
{
    for (int i = 0; i <= deep; i++) printf ("%hd ", arr [i]);
    printf ("\n");
}

void gen (short stnum, short ost, short* arr, short deep)
{

    for (short j=(deep==0)*stnum+(deep!=0)*chunrais(ost, last, llast, deep); j>0; j--)
    {
        *(arr+deep) = j;
        ost -= j;

        if (ost != 0)
        {
            deep += 1;
            gen (chunrais(ost, last, llast, deep), ost, arr, deep);
            deep -= 1;
        }
        else
            {printi (arr,deep); counter++;}

        *(arr+deep) = 0;
        ost += j;
    }
}

void decomp_gen()
{
    global_eraze3();
    short n;
    scanf ("%hd", &n);
    short arr [n];
    gen(n, n, arr, 0);
    //printf ("%ld",counter);
    return;
}
#undef chunrais(a, b, c, deep)
#undef last
#undef llast

//_________________________________________________________________________________________________
/*  4_squares
This program finds 4 squares (including 0) which summary is an input number (0 < N <= 2 000 000).
Program accepts much more - up to 4 200 000 000.
Year 2008, Variant 5, Problem C*/
struct max2sum
{
	unsigned int maxsum;
	unsigned int maxelem;
};

unsigned short int gl_flag = 1, ifforb = 0;

void global_eraze4()
{
    gl_flag = 1;
    ifforb = 0;
}

unsigned int* create_arr(unsigned int sq_N)
{
	unsigned int* sqs = (unsigned int*)calloc(sq_N + 1, sizeof(*sqs));
	if (sqs) { //printf("Memory allocated\n");
	}
	else
	{
		gl_flag = 0;
		printf("Memory hadn't been allocated successfully(\n\
			           Try to close any applications and start the program again");
		system("pause");
		return 0;
	}
	for (int i = 0; i <= sq_N; i++) sqs[i] = i*i;
	return sqs;
}

struct max2sum* gen_max2sum(unsigned int* sqs, unsigned int sq_N, unsigned int N, unsigned int forbmin)
{
	unsigned int i, j = 0, sum2;
	struct max2sum result = {0, 0};
	for (i = sq_N; sqs[i] >= (N / 2); i--)
	{
		for (j; sqs[i] + sqs[j]<=N; j++);
		sum2 = sqs[i] + sqs[j - 1];
		if ((sum2 > result.maxsum) && (!ifforb || sum2 < forbmin))
		{
			result.maxsum = sum2;
			result.maxelem = sqs[i];
			if (sum2 == N)
			{
				gl_flag = 0;
				return &result;
			}
		}
	}
	return &result;
}

void sum4squares()
{
	unsigned int N, N1, sq_N, msqs, n1, n2, n3, n4;
	struct max2sum* pmax2sum;
	unsigned int forbmin = 0xffffffff;
	global_eraze4();

	scanf("%u", &N);
	sq_N = trunc(sqrt(N*1.00));
	unsigned int* sqs = create_arr(sq_N);
	if (!gl_flag) return;

	while (gl_flag)
	{
		sq_N = trunc(sqrt(N*1.00));
		msqs = sqs[sq_N];
		if (msqs == N)
		{
			//printf("Your number is a summary of\n");
			printf("0 0 0 %u\n", N);
			gl_flag = 0;
			//system("pause");
			return;
		}
		pmax2sum = gen_max2sum(sqs, sq_N, N, forbmin);
		n1 = pmax2sum->maxelem;
		n2 = pmax2sum->maxsum - n1;
		if (!gl_flag)
		{
			//printf("Your number is a summary of\n");
			printf("0 0 %u %u\n", n2, n1);
			//system("pause");
			return;
		}
		//printf("max unprohibited summary of 2 elements is %u\nelements are %u %u\n", n1 + n2, n1, n2);
		//system("pause");
		N1 = N - n1 - n2;
		sq_N = trunc(sqrt(N1*1.00));
		msqs = sqs[sq_N];
		if (msqs == N1)
		{
			//printf("Your number is a summary of\n");
			printf("0 %u %u %u\n", N1, n2, n1);
			gl_flag = 0;
			//system("pause");
		}
		else
		{
			pmax2sum = gen_max2sum(sqs, sq_N, N1,forbmin);
			n3 = pmax2sum->maxelem;
			n4 = pmax2sum->maxsum - n3;
			if (!gl_flag)
			{
				//printf("Your number is a summary of\n");
				printf("%u %u %u %u\n", n4, n3, n2, n1);
			}
			else
			{
				if (ifforb)
				{
					if (n1 + n2 < forbmin) forbmin = n1 + n2;
				}
				else
				{
					ifforb = 1;
					forbmin = n1+n2;
				}
				//printf("Summary of 2 is incorrect. Now %u is forbidden\nStarting again...\n", forbmin);
			}
			//system("pause");
		}
	}
	return;
}

//_________________________________________________________________________________________________
/*  unrepeating_numbers
This program reads numbers and printing unrepeating ones (in raising order)
Input: 0 < N <= 100 000. Then N numbers <= 1 000. Program accepts much more numbers and their value.
This program uses single-linking list with printing flag - ifrep.
Year 2008, Variant 6, Problem C*/
struct elem
{
    unsigned int num;
    struct elem* add;
    unsigned short int ifrep;
};

void push(const unsigned int val, struct elem** curaddadd, struct elem* nextadd)
{
    *curaddadd = (struct elem*)calloc(1, sizeof(**curaddadd));
    (*curaddadd) -> num = val;
    (*curaddadd) -> add = nextadd;
    (*curaddadd) -> ifrep = 0;
}

/*void pop(struct elem** curaddadd, struct elem* nextadd)
{
    free(*curaddadd);
    *curaddadd = nextadd;
}*/

void print_stack (const struct elem* const fadd, unsigned short int want_full)
{
    unsigned short int print_flag = 0;
    const struct elem* cadd = fadd;
    while (cadd)
    {
        if (!(cadd) -> ifrep || want_full)
        {
            if (!print_flag)
                printf("%u", (cadd -> num));
            else printf(" %u", (cadd -> num));
            print_flag = 1;
        }
        cadd = (cadd -> add);
    }
    if (!print_flag)
        printf("-1");
    if (want_full)
        printf("\n\n");
}

void unrep_num()
{
    unsigned int n,cn = 0;
    scanf ("%u%u", &n, &cn);

    struct elem* fadd = (struct elem*)calloc(1, sizeof(*fadd));
    *fadd = {cn, NULL, 0};
    struct elem** caddadd = &fadd;

    unsigned int i;

    for (i = 2; i <= n; i++)
    {
        scanf ("%u", &cn);
        caddadd = &fadd;

        while ((*caddadd)&&((*caddadd)->num < cn))      //keep raising
            caddadd = &((*caddadd) -> add);

        if (!(*caddadd))
            push(cn, caddadd, NULL);     //push(<value>, <address of current structure address>=<address of field "address" from last structure>,<next structure address>)
        else if (((*caddadd) -> num) >= cn)
		{
			if ((*caddadd)->num > cn)
                push(cn, caddadd, *caddadd);
            else if ( ((*caddadd)->num == cn) && !(((*caddadd))->ifrep) )
                (*caddadd)->ifrep = 1;
        }
    }
    unsigned short int want_full;

    /*printf("All stack info\n");
    caddadd = &fadd;
    while (*caddadd)
    {
        printf("%u repeated? %hd; next address is %x\n", (*caddadd) -> num, (*caddadd) -> ifrep, (*caddadd) -> add);

        caddadd = &((*caddadd) -> add);
    }
    printf("\n");*/

    /*printf("All stack nums\n");
    print_stack(fadd, want_full = 1);*/

//    printf("Answer\n");
    print_stack(fadd, want_full = 0);
    return;
}

//_________________________________________________________________________________________________
/*  binary_good_words
This program finds amount of binary words which include K "1" -
the row of two "1" doesn't appear anywhere. Length of binary word is N
Input: N K (0 <= N,K < 48). (But the program accepts N = 95, K = 24 - the hardest test for 95)
Year 2007, Variant 5, Problem D*/
/*
To solve this task program uses special array of unconstant row length

This is an array structure:

             0 1 2 3 ... k K+1 K+2 ... (N+1)/2 - for i elem
           0 _|
           1 _ _|
           2 _ _|
           3 _ _ _|
           4 _ _ _|
           5 _ _ _ _|
         ...
           i _ _ _ _ ... _| .   .          .|
         i+1 _ _ _ _ ... _| .   .          .       .|
         ... next rows - nothing about cut elements after "_|"
       N0-k0 _ _ _ _ ... _|
     N0-K0+1 .|_ _ _ ... _|
     N0-K0+2 . .|_ _ ... _|
     N0-K0+3 . . .|_ ... _|
     N0-K0+4 . . . .|... _|
         ...
        N0-1 . . . . ...|_|
          N0 . . . . ...|_| - out of "for"

You can also see array structure by uncommenting code part "/*Array structure demonstration"
*/
#define maxi(a,b) (a>b ? a : b)
#define mini(a,b) (a<b ? a : b)
#define us unsigned short
#define ulli unsigned long long int

us N0,K0;

ulli val (short row, short col, us nelems, const ulli* const pvalue) //col in our sense
{
    if (row >= 0 && col >= 0 && col <= nelems) { return *pvalue; }
    else return 0;
}

void GOODwords_num()
{
    short i,j;
    //printf ("Input N and K, N<=95\n");
    scanf ("%hd%hd", &N0, &K0);
    if (K0 > (N0+1)/2 || K0<=1)
    {
        if (K0 > (N0+1)/2)
        {
            printf ("0");
        }
        else if (K0 == 0)
        {
            printf ("1");
        }
        else printf ("%hd",N0);
        return;
    }
    struct strarr
    {
        ulli* addstr;
        us stelem;
        us nelem;
    };
    struct strarr arradd [N0+1];
    for (i = 0; i <= N0; i++)
        {
           arradd [i].addstr = (ulli*) calloc (arradd [i].nelem = mini((i+1)/2+1,K0+1)-(arradd [i].stelem = maxi(0, K0+1-(N0-i))),sizeof(*arradd [i].addstr));
           //address = (ulli*) give_memory(number_of_elements = k or (i+1)/2 +1 - (cut_elements = max (0, k0-(N0-i))),size_of(*address);
        }
    arradd [N0].addstr = (ulli*) calloc (arradd [N0].nelem = K0 + 1 - (arradd [N0].stelem = K0), sizeof (*arradd [N0].addstr));
    /* Array structure demonstration
    for (i = 0; i<= N0; i++)
    {
        printf("%hd row len %hd start number %hd address %x\n",\
                i, arradd [i].nelem, arradd [i].stelem, arradd [i].addstr);
    }
    */
    short delta1, delta2;
    *(arradd [0].addstr) = 1;
    *(arradd [1].addstr) = 1;
    *(arradd [1].addstr + 1) = 1;
    for (i = 2; i <= N0; i++)
        {
        delta1 = arradd [i].stelem - arradd [i-1].stelem;
        delta2 = arradd [i].stelem - arradd [i-2].stelem;
        for (j = 0; j <= arradd [i].nelem - 1; j++)
            {

                *(arradd [i].addstr + j) = \
                val(i-1,j+delta1,arradd [i-1].nelem - 1,arradd [i-1].addstr + j + delta1)+\
                val(i-2,j-1+delta2,arradd [i-2].nelem - 1,arradd [i-2].addstr + j - 1 + delta2);
            }
        }
    /* Output array
    for (i = 0; i <= N0; i++)
        {
            for (j = 0; j <= arradd [i].nelem-1; j++)
                {
                    printf("%lli ", *(arradd [i].addstr+j));
                }
            printf("\n");
        }
    */
    //printf("Answer:\n");
    printf("%lld",*(arradd [N0].addstr + (arradd [N0].nelem -1)));
    return;
}
#undef maxi(a,b)
#undef mini(a,b)
#undef us
#undef ulli

//_________________________________________________________________________________________________
/* big_summary
This program reads two binary numbers and then prints their binary summary. (<= 1000 decimals)
Problem C, Variant 7, 2007
*/
unsigned int lebigsum (const char num1[],const char* ep1, const char num2[], const char* ep2, char* sum)
{
    unsigned int ost = 0, cdig = 0;
    unsigned int sum_ep = (unsigned int)sum;
    while (ep1 >= num1)
    {
        if (ep2 >= num2)
        {
            cdig = *(ep1--) + *(ep2--) - 2*'0' + ost;
            ost = cdig / 2;
            *((sum)++) = (cdig % 2);
        }
        else
        {
            cdig = *(ep1--) - '0' + ost;
            ost = cdig / 2;
            *(sum++) = (cdig % 2);
        }

    }
    if (ost != 0) *(sum++) = ost;
    *sum = '\0';
    return (unsigned int)sum - sum_ep;


}

int bigsum()
{
    char n1[1001], n2[1001];
    char sum[1002];
    scanf("%s%s", n1, n2);
    unsigned int len1, len2;
    len1 = strlen(n1);
    len2 = strlen(n2);
    if (len1 < len2)
    {
        swap(n1,n2);
        swap(len1,len2);
    }   //first number is more than second
    int i = lebigsum (n1,n1+len1-1,n2,n2+len2-1, sum) - 1;
    for (i; i >= 0; i--) printf("%d", sum[i]);
    return 0;
}

//_________________________________________________________________________________________________
/* Decomposing_without_repeats
This program decomposes an input number (from 1 to 40) into a summary of natural numbers. Program accepts much more number, but it takes much time.
Prints all combinations of non-raising successions of addents. Also watch Decomposing_gen subprogram (also in this file)
Year 2007, Variant 9, Problem D*/
#define chunrais1(a, b, deep) ((deep == 0) ? a : ((a < b) ? a : (b-1)))
#define last arr[deep-1]

//long counter; //has already defined in this program

void global_eraze8()
{
    counter = 0;
}

/*void printi (short* arr, short deep)  //has already been defined in this program
{
    for (int i = 0; i <= deep; i++) printf ("%hd ", arr [i]);
    printf ("\n");
}
*/
void gen1 (short stnum, short ost, short* arr, short deep)
{

    for (short j=(deep==0)*stnum+(deep!=0)*chunrais1(ost, last, deep); j>0; j--)
    {
        *(arr+deep) = j;
        ost -= j;

        if (ost != 0)
        {
            deep += 1;
            gen1 (chunrais1(ost, last, deep), ost, arr, deep);
            deep -= 1;
        }
        else
        {
            printi (arr,deep);
            counter++;
        }

        *(arr+deep) = 0;
        ost += j;
    }
}

int decomp_gen_w_o_rep ()
{
    global_eraze8();
    short n;
    scanf ("%hd", &n);
    short arr [n];
    gen1(n, n, arr, 0);
    //printf ("%ld",counter);
    return 0;
}
#undef chunrais1(a, b, deep)
#undef last

//_________________________________________________________________________________________________
/* Prime
This program finds the number which № in primes succession is equal to input value (<= 10000).
Program accepts much more but it takes much time (time for 1 000 000 is around 4 sec)
Problem C, Variant 2, Year 2007
*/
struct elem1
{
	unsigned int num;
	struct elem1* add;
};

//unsigned int counter;

void global_erase9()
{
	counter = 1;
}

void push1(const unsigned int val, struct elem1** saveaddadd, struct elem1* nextadd)
{
	*saveaddadd = (struct elem1*)calloc(1, sizeof(**saveaddadd));
	(*saveaddadd)->num = val;
	(*saveaddadd)->add = nextadd;
}

unsigned short newelem(const unsigned int val, struct elem1*** saveaddaddadd, struct elem1* nextadd)
{
	push1(val, *saveaddaddadd, nextadd);
	if (!(**saveaddaddadd))
	{
		printf("Memory error");
		system("pause");
		exit(0);
		return 0;
	}
	else
	{
		(*saveaddaddadd) = &((**saveaddaddadd)->add);
		counter++;
		return 1;
	}
}

void print_stack1(const struct elem1* const fadd)
{
	//unsigned int counter = 0;
	const struct elem1* cadd = fadd;
	while (cadd->add)
	{
		/*printf("%u\n", (cadd->num));
		counter++;
		if (counter % 100 == 0) system("pause");*/
		cadd = (cadd->add);
	}
	printf("%u", cadd->num);
}

int prime()
{
	global_erase9();
	unsigned int n;
	scanf("%u", &n);

	struct elem1* fadd = (struct elem1*)calloc(1, sizeof(*fadd));
	*fadd = { 2, NULL };
	struct elem1 **caddadd = &fadd, **saveaddadd = &(fadd->add);

	unsigned int i, j;

	for (i = 3; counter != n; i++)
	{
		caddadd = &fadd;
		unsigned int compnum = (*caddadd)->num;
		for (j = 1; (j <= counter && (i % compnum) > 0 && i >= compnum*compnum); j++)
		{
			caddadd = &((*caddadd)->add);
			if (caddadd) compnum = (*caddadd)->num;
		}
		if (j > counter || i < compnum*compnum)
			if (!(newelem(i, &saveaddadd, NULL))) exit(0);    //caddadd is now reloaded
		//else if (i % (*caddadd)->num == 0) printf("%u was declined\n", i);
	}
	print_stack1(fadd);
	system("pause");
	return 0;
}
//_________________________________________________________________________________________________
/*Prime_plural
This program finds numbers which № in primes succession is equal to input values(<= 10000).
Program accepts much more but it takes much time (time for 1 000 000 is around 4 sec)
Input:  N
        N numbers in raising order
Problem B, Variant 3, Year 2007
*/
/*struct elem1
{
	unsigned int num;
	struct elem1* add;
};*/

//unsigned int counter;

void global_erase10()
{
    counter = 1;
}

/*void push1(const unsigned int val, struct elem1** saveaddadd, struct elem1* nextadd)
{
	*saveaddadd = (struct elem1*)calloc(1, sizeof(**saveaddadd));
	(*saveaddadd)->num = val;
	(*saveaddadd)->add = nextadd;
}*/
/*unsigned short newelem(const unsigned int val, struct elem1*** saveaddaddadd, struct elem1* nextadd)
{
	push1(val, *saveaddaddadd, nextadd);
	if (!(**saveaddaddadd))
	{
		printf("Memory error");
		system("pause");
		exit(0);
		return 0;
	}
	else
	{
		(*saveaddaddadd) = &((**saveaddaddadd)->add);
		counter++;
		return 1;
	}
} */

void print_stack2(const struct elem1* const fadd, const unsigned int* arr)
{
	unsigned int i = 1;
	const struct elem1* cadd = fadd;
	unsigned short printflag = 0;
	for (i = 1; i <= counter; i++)
	{
		/*printf("%u\n", (cadd->num));
		counter++;
		if (i % 100 == 0) system("pause");*/
		if (i == (*arr))
		{
            if (printflag) printf(" %u", (cadd->num));
            else
            {
                printf("%u", (cadd->num));
                printflag = 1;
            }
            arr++;
        }
		cadd = (cadd->add);
	}
    //cadd = NULL!!!
}

int prime_plu()
{
	global_erase10();
	unsigned int n, nn, i = 0, j;

	scanf("%u", &nn);
	unsigned int* arr = (unsigned int*)calloc(nn,sizeof(*arr));
	for (i = 0; i < nn - 1; i++) scanf("%u",arr+i);
	scanf("%u", &n);
	arr[i] = n;

	struct elem1* fadd = (struct elem1*)calloc(1, sizeof(*fadd));
	*fadd = { 2, NULL };
	struct elem1 **caddadd = &fadd, **saveaddadd = &(fadd->add);

	for (i = 3; counter != n; i++)
	{
		caddadd = &fadd;
		unsigned int compnum = (*caddadd)->num;
		for (j = 1; (j <= counter && (i % compnum) > 0 && i >= compnum*compnum); j++)
		{
			caddadd = &((*caddadd)->add);
			if (caddadd) compnum = (*caddadd)->num;
		}
		if (j > counter || i < compnum*compnum)
			if (!(newelem(i, &saveaddadd, NULL))) exit(0);    //caddadd is now reloaded
		//else if (i % (*caddadd)->num == 0) printf("%u was declined\n", i);
	}
	print_stack2(fadd,arr);
	system("pause");
	return 0;
}
//_________________________________________________________________________________________________

int main()
{
    unsigned short int func_num = 0;
    description
    while (func_num)
    {
        switch (func_num)
        {
            case 1: space_del();            break;
            case 2: four_dec_numbers();     break;
            case 3: decomp_gen();           break;
            case 4: sum4squares();          break;
            case 5: unrep_num();            break;
            case 6: GOODwords_num();        break;
            case 7: bigsum();               break;
            case 8: decomp_gen_w_o_rep();   break;
            case 9: prime();                break;
            case 10:prime_plu();            break;
        }
        printf("\n\nProgram %hd has successfully finished)))\n\n", func_num);
        description
    }
    return 0;
}
