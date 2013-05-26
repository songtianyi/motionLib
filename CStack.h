
/**
 * CStack.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef CSTACK_H
#define CSTACK_H
/**
 *@brief stack, C++ language
 *@author songtianyi630@163.com
 */
#include <cassert>
#include <cstdlib>
using namespace std;

typedef int Type;
class CStack
{
    public:
        CStack(void);
        CStack(const int nStackSize);
        ~CStack(void);

        /*empty stack*/
        void clear(void);

        /*free stack*/
        void free(void);

        Type top(void);
        void pop(void);
        void push(Type v);
        bool empty(void);
        int  size(void);
    private:
        int m_p;
        int m_sizeLimit;
        Type *array;

};

#endif // CSTACK_H
