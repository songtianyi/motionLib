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
