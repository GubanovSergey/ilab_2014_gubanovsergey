#ifndef INCL_STACK

#define INCL_STACK
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <iostream>

typedef unsigned long long int ulli;
const unsigned short int EMPTY_STACK = 0, OK = 10, DEF_SIZE = 50, ADD_SIZE = 10, SUBT_SIZE = 100;

template <typename st_type>
class Stack
{
public:
    Stack();
    ~Stack();
    void Stack_push(st_type entry);
    st_type Stack_pop();
    st_type Stack_peek();
    void Stack_print () const;

private:
    void Stack_resize(short int res_type);
    st_type * data;
    ulli counter, st_size;
};

template <typename st_type>
Stack<st_type>::Stack():
    data(new st_type[DEF_SIZE]),
    counter (0),
    st_size (DEF_SIZE)
{
    memset (data, 0, DEF_SIZE * sizeof(st_type));
}

template <typename st_type>
Stack<st_type>::~Stack()
{
    assert(this && data);
    delete[] this->data;
    this->data = nullptr;
    this->counter = 0;
}

template <typename st_type>
void Stack<st_type>::Stack_resize(short int res_type)
{
    assert(this && this->st_size && this->counter && this->data && counter <= st_size);
    ulli nsize = 0;
    if (res_type == 1)
    {
        assert(counter == st_size);
        nsize = counter + ADD_SIZE;
    }
    else if (res_type == -1)
        nsize = counter + ADD_SIZE;
    else assert(("Incorrect respond to Stack_resize", 0));
    this->data = (st_type *)realloc (data, nsize * sizeof(st_type));
    assert(this->data);
    st_size = nsize;
}

template <typename st_type>
void Stack<st_type>::Stack_push(st_type entry)
{
    assert(this && data);
    if (counter == st_size)
        Stack_resize(1);
    data[counter++] = entry;
}

template <typename st_type>
st_type Stack<st_type>::Stack_pop()
{
    assert(this && counter && st_size && data && counter <= st_size);
    st_type res = data[--counter];
    if (st_size > counter + SUBT_SIZE)
        Stack_resize(-1);
    return res;
}

template <typename st_type>
st_type Stack<st_type>::Stack_peek()
{
    assert(this && st_size && data && counter <= st_size);
    st_type res = 0;
    if (counter == 0)
    {
        printf("Empty stack, returned 0");
        return 0;
    }
    res = data[counter - 1];
    return res;
}

template <typename st_type>
void Stack<st_type>::Stack_print() const
{
    assert(this && data && st_size && counter <= st_size);
    printf("_____________________________________________________\n");
    printf("__________________________________Stack_print %i\n\n", this);
    printf("Counter = %llu, Dataptr = %i\n\n", counter, data);
    if (counter == 0)
        printf("Empty stack\n");
    else
    {
        printf("Elements:\n");
        for (ulli i = 0; i < counter; i++)                            //It's very very BAD!(
        {
            printf("\t[%4llu] element is ", i + 1);
            std::cout << data[i] << std::endl;
        }
        printf("End of stack\n\n");
        printf("=====================================================\n\n");
    }
}

#endif //INCL_STACK
