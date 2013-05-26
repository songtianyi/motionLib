
/**
 * FPC.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef FPC_H
#define FPC_H


#include <assert.h>
#include <string.h>
#include <vector>
#include "Huffman.h"


#define GABS(x) (x<0?(-x):x)
#define FPC_SENSITIVITY (1<<13)

/*
*@brief floating point compression routines
*/
class FPC
{
public:
	FPC(void);
	~FPC(void);
	//initiallize buffer
	void setBuffer(std::vector<unsigned char> *charBuffer);
	void setBufferStat(const unsigned int charbufferCnt,const unsigned int bitBuffer,const unsigned int bitBufferCnt);
	void getBufferStat(unsigned int &charbufferCnt,unsigned int &bitBuffer,unsigned int &bitBufferCnt);

	//quantize double to unsigned short, highest bit to remark whether the double data is bigger than 1 or not
	void quantize64To15(const double *_64mat,unsigned short *_16mat,const int row,const int col,int *exceed);

	//be care of the first frame
	//get matrix of predicted value ,matrix to indicate the dist is positive or negative, bit num of predicted value
	void predict16(const unsigned short *_16mat,unsigned short *predMat,bool *signMat,int *bitsCnt,const int row,const int col);
	void predict16P(const unsigned short *_16mat,unsigned short *predMat,bool *signMat,int *bitsCnt,const int row,const int col);

	//stastic the frequency of pre-zero and successive-zero length[2,9]
	void statistic(const int *bitsCnt,int *pZeroNum,int *equalZeroCnt,const int row,const int col);

	//
	//output huffman to buffer
	void encodeHuffmanTable(const CHuffman *hf);
	void decodeHuffmanTable(CHuffman *hf);

	//output predicted value to buffer
	void encodePredMat16(const unsigned short *predMat,const bool *signMat,\
		const int *bitsCnt,const int *equalZeroCnt,\
		const CHuffman *hf,\
		const int row,const int col);
	//decode quantized matrix
	void decode16mat(unsigned short *_16mat,const CHuffman *hf,const int row,const int col);

	void encodePredMat16P(const unsigned short *predMat,const bool *signMat,\
		const int *bitsCnt,const int *equalZeroCnt,\
		const CHuffman *hf,\
		const int row,const int col);
	void decode16matP(unsigned short *_16mat,const CHuffman *hf,const int row,const int col);

	//output the exceeded part to buffer
	void encodeExceed(const int *exceed,const int row,const int col);

	//decode
	void decode64mat(unsigned short *_16mat,double *mat,const int row,const int col);




	//remember to initialize bitBuffer, bitBufferCnt
	void __saveBits(int nCode,int nBitCnt);
	unsigned int __readBits(int nBitCnt);
	int __decodeHuffmanCode(const CHuffman *hf);

	static void __32To16(const float x,unsigned short &res);
	static float __16To32(const unsigned short t);
	static void __64To15(const double x,unsigned short &res);//not deal with the high-bit yet
	static void __32To15(const float x,unsigned short &res);//not deal with the high-bit yet
	static float __15To32(const unsigned short t);
	static double __15To64(const unsigned short t);

private:
	unsigned int m_bitBuffer;
	unsigned int m_bitBufferCnt;

	std::vector<unsigned char> *m_charBuffer;
	unsigned int m_charBufferCnt;
};

#endif
