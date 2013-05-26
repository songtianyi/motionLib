
/**
 * Huffman.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Jimin Zhou <>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef CHUFFMAN_H
#define CHUFFMAN_H

#include <string.h>

#define HUFF_S_LEN 9

struct HFM_Dic
{
    int length;
    int code;
    bool operator<(HFM_Dic x)
    {
        return length < x.length;
    }
};

class CHuffman
{
public:
    CHuffman();
    ~CHuffman();
    void simulate(const int *m_nFreqArray);
    void generate();
private:
    void heapsift(int i);

public:
    HFM_Dic m_DicArray[HUFF_S_LEN];

private:
    int heap[HUFF_S_LEN*2+1];
    int heapCnt;
public:
    int len[HUFF_S_LEN+2];
    int rec[HUFF_S_LEN+1][HUFF_S_LEN];
    int first[HUFF_S_LEN+2];
};

#endif // CHUFFMAN_H
