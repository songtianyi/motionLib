#include "Huffman.h"

CHuffman::CHuffman()
{
    //ctor
}

CHuffman::~CHuffman()
{
    //dtor
}


void CHuffman::heapsift(int i)
{
    int l=i<<1;
    int r=(i<<1)+1;
    int least=i;
    if(l<=heapCnt && heap[heap[l]]<heap[heap[least]])
    {
        least=l;
    }
    if(r<=heapCnt && heap[heap[r]]<heap[heap[least]])
    {
        least=r;
    }
    if(least!=i)
    {
		//exchange index
        heap[i]^=heap[least];
        heap[least]^=heap[i];
        heap[i]^=heap[least];
        heapsift(least);
    }
}

void CHuffman::simulate(const int *m_nFreqArray)
{

    int la,lb;
    for(int i=0; i<HUFF_S_LEN; ++i)
    {
        heap[i+HUFF_S_LEN+1] = m_nFreqArray[i];
        heap[i+1]=i+HUFF_S_LEN+1;
    }

    heapCnt=HUFF_S_LEN;
	//little heap
    for(int i=heapCnt/2; i>=1; --i)
    {
        heapsift(i);
    }
    while(heapCnt>1)
    {
        la=heap[1];
        heap[1]=heap[heapCnt];
        heapCnt--;
        heapsift(1);
        lb=heap[1];
		//la lb minium value index
        heap[heapCnt+1]=heap[la]+heap[lb];
		//
        heap[1]=heapCnt+1;
        heap[la]=heapCnt+1;
        heap[lb]=heapCnt+1;
        heapsift(1);
    }
}

void CHuffman::generate()
{
    for(int i=1; i<=HUFF_S_LEN; ++i)
    {
        int temp=heap[i+HUFF_S_LEN];
        int len=1;
        while(temp!=2)
        {
            temp=heap[temp];
            len++;
        }
        m_DicArray[i-1].length=len;
    }
    memset(len,0,sizeof(len));
    for(int i=0; i<HUFF_S_LEN; ++i)
    {
        m_DicArray[i].code=len[m_DicArray[i].length];
        len[m_DicArray[i].length]++;
    }
    //cal first
    first[HUFF_S_LEN+1]=0;
    for(int i=HUFF_S_LEN; i>0; --i)
    {
        first[i]=(first[i+1]+len[i+1]+1)/2;
    }
    //cal code
    for(int i=0; i<HUFF_S_LEN; ++i)
    {
        m_DicArray[i].code=m_DicArray[i].code+first[m_DicArray[i].length];
    }
}