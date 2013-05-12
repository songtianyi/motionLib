#include "CStack.h"


CStack::CStack()
{
    //ctor
    //you can't use this constructor
    //use CStack(int nStackSize) instead
    assert(m_p == -1 && m_sizeLimit > 0);
}


CStack::CStack(const int nStackSize)
{
    m_p = -1;
    m_sizeLimit = nStackSize;
	array = NULL;
    array = new Type[nStackSize];
	assert( array != NULL );
}

CStack::~CStack()
{
    //dtor
    assert(array == NULL);/*you need to call func free to free the memory*/
}

void CStack::clear()
{
    m_p = -1;
}
void CStack::free()
{
    if(array != NULL) delete [] array; array = NULL; clear();
}
Type CStack::top(){
        if( m_p > -1 && m_p < m_sizeLimit ){return array[m_p];}
        assert(m_p > -1 && m_p < m_sizeLimit);/*test whether it's empty or not*/
        return 0;
}
void CStack::push(Type v)
{
        if(m_p + 1 < m_sizeLimit){ array[++m_p] = v;}
        assert(m_p+1 < m_sizeLimit);/*memory exceed limit*/
}
bool CStack::empty()
{
        if(m_p < 0)return true;
        else return false;
}
void CStack::pop()
{
    m_p--;
}
int  CStack::size()
{
    return m_p + 1;
}
