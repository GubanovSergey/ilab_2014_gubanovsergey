#ifndef INCL_STACK

#define INCL_STACK

#define DBG_mode

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

#ifdef DBG_mode
    #define DBG if(1)
#else
    #define DBG if(0)
#endif

template <typename st_type>
class Stack
{
public:
    typedef unsigned long long int ulli;
    Stack();
    ~Stack();
    void Stack_push(st_type);
    st_type Stack_pop();
    st_type Stack_peek();
    enum err_t {
        EMPTY = 0,
        DAMAGED,
        MEMERR,
        INC_POP,
        EMP_PEEK,
        FULL = 9,
        OK
    };
    err_t Stack_verify ();
    void Stack_print ();

private:
    const unsigned short int DEF_SIZE = 50, ADD_SIZE = 10, SUBT_SIZE = 100;
    void Stack_resize(short int);
    void err_handle (err_t) const;
    st_type * data_;
    ulli counter_, st_size_;
    err_t lastcond_;
};

template <typename st_type>
Stack<st_type>::Stack():
    data_ (new st_type[DEF_SIZE]),
    counter_ (0),
    st_size_ (DEF_SIZE),
    lastcond_ (OK)
{
    DBG memset (data_, 0, DEF_SIZE * sizeof(st_type));
}

template <typename st_type>
typename Stack<st_type>::err_t Stack <st_type>::Stack_verify ()
{
    DBG assert(this); //DBG?
    if (data_ == NULL)
    {
        err_handle(MEMERR);
        lastcond_ = MEMERR;
        return MEMERR;      //useless
    }
    else
        lastcond_ = OK; //because we need to determine MEMERR and any of other conditions
    if (st_size_ == 0 || st_size_ < counter_)
    {
        err_handle(DAMAGED);
        return DAMAGED;     //useless
    }
    else if (counter_ == st_size_)
        return FULL;
    else if (counter_ == 0)
        return EMPTY;
    else
        return OK;
    //return OK;      //for compiler to be quite
}

template <typename st_type>
void Stack <st_type>::err_handle(err_t handling_error) const
{
    if (handling_error == MEMERR)
    {
        std::cout << "Stack was damaged or hasn't enough memory" << std::endl;
        if (lastcond_ == MEMERR)
            exit(1);
    }
    else if (handling_error == DAMAGED)
    {
        std::cout << "Stack was damaged. Size or number of elements is incorrect" << std::endl;
        exit(1);
    }
    else if (handling_error == INC_POP)
    {
        std::cout << "Stack is empty. Pop will be denied. 0 will be returned" << std::endl;
        return;
    }
    else if (handling_error == EMP_PEEK)
    {
        std::cout << "Stack is empty. 0 will be returned by Stack_peek()" << std::endl;
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
void Stack<st_type>::Stack_resize(short int res_type)
{
    ulli nsize = 0;
    st_type * old_data = data_;
    DBG
    {
        err_t check = Stack_verify();
        if ((check == FULL && res_type == 1) || (check == OK && res_type == -1))
            nsize = counter_ + ADD_SIZE;
        else assert(("Incorrect respond to Stack_resize", 0));
    }
    else nsize = counter_ + ADD_SIZE;

    //data_ = (st_type *)realloc (data_, nsize * sizeof(st_type));  //replace realloc
    data_ = new st_type[nsize];
    Stack_verify();
    for (ulli i = 0; i < counter_; i--)
        data_[i] = old_data[i];
    delete [] old_data;
    st_size_ = nsize;
}

template <typename st_type>
void Stack<st_type>::Stack_push(st_type entry)
{
    if (Stack_verify() == FULL)
        Stack_resize(1);
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
    if (st_size_ > counter_ + SUBT_SIZE)
        Stack_resize(-1);
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
        for (ulli i = 0; i < counter_; i++)
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
