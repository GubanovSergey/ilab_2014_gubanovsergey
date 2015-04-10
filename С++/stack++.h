#ifndef INCL_STACK

#define INCL_STACK

#define DBG_mode

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

#ifdef DBG_mode
    #define DBG if(1)
#else
    #define DBG if(0)
#endif

/*This template stack can be initialized with any basic type (such as int, double or some pointer).
There are some methods to work with it: Stack_push(), Stack_peek() - it returns top element or null
if stack is empty, also Stack_print(), Stack_redsize() - to reduce space allocated to Stack
(see desc. of this func.), Stack_place() which returns empty space in stack at the moment,
Stack_cond(), which shows condition of stack: MEMERR, DAMAGED, INC_POP, EMP_PEEK, INC_RESP, FULL, OK.
Also it generates Err_log.txt file to report about some special situations or errors*/
template <typename st_type>
class Stack
{
private:
    enum err_t {
        EMPTY = 0,
        DAMAGED,
        MEMERR,
        INC_POP,
        EMP_PEEK,
        INC_RESP,
        FULL = 9,
        OK
    };

public:
    Stack();
    ~Stack();
    void Stack_redsize(unsigned long long);
    void Stack_push(st_type);
    st_type Stack_pop();
    st_type Stack_peek();
    void Stack_print ();

    inline unsigned long long Stack_place() {
        return st_size_ - counter_;
    }
    inline typename Stack<st_type>::err_t Stack_cond() {
        return cond_;
    }

private:
    const unsigned short int DEF_SIZE = 100, ADD_SIZE = 10, SUB_SIZE = 200;            //ADD_SIZE IN %
    st_type * data_;
    unsigned long long counter_, st_size_;
    err_t cond_;                //condition of Stack, which can be got by Stack_cond()

    void err_handle (err_t) const;
    err_t Stack_verify ();
    void Stack_resize();
};

template <typename st_type>
Stack<st_type>::Stack():
    data_ (new st_type[DEF_SIZE]),
    counter_ (0),
    st_size_ (DEF_SIZE),
    cond_ (OK)
{
    DBG memset (data_, 0, DEF_SIZE * sizeof(st_type));
}

template <typename st_type>
typename Stack<st_type>::err_t Stack<st_type>::Stack_verify ()
{
    DBG assert(this); //DBG?
    if (data_ == NULL)
    {
        cond_ = MEMERR;
        err_handle(MEMERR);
    }
    else if (st_size_ == 0 || st_size_ < counter_)
    {
        cond_ = MEMERR;
        err_handle(DAMAGED);
    }
    else if (counter_ == st_size_)
        cond_ = FULL;
    else if (counter_ == 0)
        cond_ = EMPTY;
    else
        cond_ = OK;
    return cond_;
}

template <typename st_type>
void Stack<st_type>::err_handle(err_t handling_error) const
{
    static std::fstream err_log;
    err_log.open("Err_log.txt", std::fstream::out);

    if (handling_error == MEMERR)
    {
        err_log << "Stack was damaged or hasn't enough memory" << std::endl;
        exit(1);
    }
    else if (handling_error == DAMAGED)
    {
        err_log << "Stack was damaged. Size or number of elements is incorrect" << std::endl;
        exit(1);
    }
    else if (handling_error == INC_POP)
    {
        err_log << "Stack is empty. Pop will be denied. 0 will be returned" << std::endl;
        return;
    }
    else if (handling_error == EMP_PEEK)
    {
        err_log << "Stack is empty. 0 will be returned by Stack_peek()" << std::endl;
        return;
    }
    else if (handling_error == INC_RESP)
    {
        err_log << "\nI!!!--------\nIncorrect respond to stack public method. Nothing to be done\n!!!--------\n" << std::endl;
        return;
    }
}

template <typename st_type>
Stack<st_type>::~Stack()
{
    DBG assert(this && data_);
    delete[] data_;
    data_ = nullptr;
    counter_ = 0;
}

template <typename st_type>
void Stack<st_type>::Stack_resize()
{
    double calc_nsize = 0;
    unsigned long long nsize = 0;
    st_type * old_data = data_;
    calc_nsize = counter_ * (1 + 1.0*ADD_SIZE/100);
    nsize = (unsigned long long)calc_nsize;

    data_ = new st_type[nsize];
    Stack_verify();                 //Was memory allocated?

    for (unsigned long long i = 0; i < counter_; i++)
        data_[i] = old_data[i];
    delete [] old_data;
    st_size_ = nsize;
}

/*Public method which allows user to reduce space, belonging to stack.
You can only free empty space (w/o loosing any data) and you couldn't set space less
than initializing size (see description of class)*/
template <typename st_type>
void Stack<st_type>::Stack_redsize(unsigned long long red_size)
{
    if (red_size >= st_size_ - counter_ && st_size_ - red_size >= DEF_SIZE)
        err_handle(INC_RESP);            //TODO Can accept reducing size with loosing some data_
    //Warn mode could be added and give user information about
    //too little decreasing of Stack_size, which can be ineffective
    else
    {
        data_ = (st_type *)realloc(data_, (st_size_ - red_size) * sizeof(st_type));
        Stack_verify();                 //Was memory allocated?
        st_size_ -= red_size;
    }
    return;
}

template <typename st_type>
void Stack<st_type>::Stack_push(st_type entry)
{
    if (Stack_verify() == FULL)
        Stack_resize();
    data_[counter_++] = entry;
}

template <typename st_type>
st_type Stack<st_type>::Stack_pop()
{
    if (Stack_verify() == EMPTY)
    {
        err_handle(INC_POP);
        return 0;
    }
    st_type res = data_[--counter_];
    //Warn mode could be added and give user information about almost empty stack
    return res;
}

template <typename st_type>
st_type Stack<st_type>::Stack_peek()
{
    if (Stack_verify() == EMPTY)
    {
        err_handle(EMP_PEEK);
        return 0;
    }
    return data_[counter_ - 1];
}

template <typename st_type>
void Stack<st_type>::Stack_print()
{
    Stack_verify();
    std::cout << "_____________________________________________________\n";
    std::cout << "__________________________________Stack_print" << this << '\n' << std::endl;
    std::cout << "Counter = " << counter_ << ", Dataptr = " << data_ << '\n' << std::endl;
    if (counter_ == 0)
        std::cout << "Empty stack" << std::endl;
    else
    {
        std::cout << "Elements:\n";
        for (unsigned long long i = 0; i < counter_; i++)
        {
            printf("\t[%4llu] element is ", i + 1);
            std::cout << data_[i] << std::endl;
        }
        std::cout << "End of stack\n\n";
        std::cout << "=====================================================\n" << std::endl;
    }
}

#undef DBG_mode
#endif //INCL_STACK
