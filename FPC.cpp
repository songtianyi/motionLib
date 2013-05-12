#include "FPC.h"

FPC::FPC(void)
{
	m_bitBuffer = m_bitBufferCnt = 0;
	m_charBuffer  = 0;
	m_charBufferCnt = 0;
}
void FPC::setBuffer(std::vector<unsigned char> *charBuffer)
{
	m_charBuffer	= charBuffer;
}
void FPC::setBufferStat(const unsigned int charbufferCnt,const unsigned int bitBuffer,const unsigned int bitBufferCnt)
{
	m_charBufferCnt = charbufferCnt;
	m_bitBuffer		= bitBuffer;
	m_bitBufferCnt	= bitBufferCnt;
}
void FPC::getBufferStat(unsigned int &charBufferCnt,unsigned int &bitBuffer,unsigned int &bitBufferCnt)
{
	charBufferCnt	= m_charBufferCnt;
	bitBuffer		= m_bitBuffer;
	bitBufferCnt	= m_bitBufferCnt;
}
FPC::~FPC(void)
{
	m_charBuffer = 0;
}
void FPC::__32To16(const float x,unsigned short &res)
{
	//little endian
    assert(GABS(x)  < 1.0f);
    int  nTmp = static_cast<int>(x*32768);
    nTmp += 32768;
	res = nTmp & 65535;
}
float FPC::__16To32(const unsigned short t)
{
    int x = t; x -= 32768;
    return (float)(x/32768.0f);
}
void FPC::__64To15(const double x,unsigned short &res)
{
	assert(GABS(x)  < 1.0);
	int nTmp = static_cast<short>(x*16384);
	nTmp += 16384;
	res = nTmp & 32767; 
}
void FPC::__32To15(const float x,unsigned short &res)
{
	assert(GABS(x)  < 1.0);
	int nTmp = static_cast<short>(x*16384);
	nTmp += 16384;
	res = nTmp & 32767; 
}
double FPC::__15To64(const unsigned short t)
{
	int x = (t&32767);
	x -= 16384;
	return (double)(x/16384.0);
}
float FPC::__15To32(const unsigned short t)
{
	int x = (t&32767);
	x -= 16384;
	return (float)(x/16384.0);
}

void FPC::quantize64To15(const double *_64mat,unsigned short *_16mat,const int row,const int col,int *exceed)
{
	for(int i = 0;i < row;i++)
	{
		int index = i*col;
		for(int j = 0;j < col;j++)
		{
			int t = (int)_64mat[index+j];
			if(GABS(t) == 0)
			{
				FPC::__64To15(_64mat[index+j],_16mat[index+j]);
                _16mat[index+j] &= 32767;//
				exceed[index+j] = 0;
			}
			else
			{
				exceed[index+j] = t;
				FPC::__64To15(_64mat[index+j] - t,_16mat[index+j]);
				_16mat[index+j] |= 32768; 
			}
		}
	}
}

void FPC::predict16(const unsigned short *_16mat,unsigned short *predMat,\
					bool *signMat,int *bitsCnt,const int row,const int col)
{
    unsigned short *usJump		= new unsigned short[col];
    unsigned short *usLastValue = new unsigned short[col];
	unsigned short usTrueValue;
    memset(usJump,0,sizeof(unsigned short)*(col));
	memcpy(usLastValue,_16mat,sizeof(unsigned short)*col);
	memcpy(predMat,_16mat,sizeof(unsigned short)*col);
	//处理第一�?
	for(int j = 0;j < col;j++)
	{
		unsigned short usTmp = predMat[j];
		int nCnt = 0;
		while(usTmp)
        {
            usTmp >>= 1;
            nCnt++;
        }
        bitsCnt[j] = (nCnt+1)/2;
		signMat[j] = 0;//dist = 0
	}

    for(int i = 1; i < row; i++)								//第一行的数据保存在了 usLastValue�?这样预测效果更好
    {
		int index = i*col;
        for(int j = 0; j < col; j++)
        {
			usTrueValue = _16mat[index+j];
            int dist = usTrueValue - usLastValue[j];
            unsigned short usTmp = usJump[j] ^ GABS(dist);
            if(GABS(dist) < FPC_SENSITIVITY)
            {
                usJump[j] = GABS(dist);
            }
            usLastValue[j] = usTrueValue;

            predMat[index+j] = usTmp;

            if(dist < 0)
                signMat[index+j] = 1;
            else
                signMat[index+j] = 0;

			int nCnt = 0;
			while(usTmp)
            {
                usTmp >>= 1;
                nCnt++;
            }
            bitsCnt[index+j] = (nCnt+1)/2;
        }
    }

    delete [] usJump; usJump = 0;
    delete [] usLastValue; usLastValue = 0;
}
void FPC::predict16P(const unsigned short *_16mat,unsigned short *predMat,\
					 bool *signMat,int *bitsCnt,const int row,const int col)
{
    unsigned short *usJump		= new unsigned short[col];
    unsigned int *usLastValue = new unsigned int[col];
	unsigned short usTrueValue;
    memset(usJump,0,sizeof(unsigned short)*(col));

	for(int j = 0;j < col;j++)
	{
		usLastValue[j] = _16mat[j];
	}
	memcpy(predMat,_16mat,sizeof(unsigned short)*col);


	for(int j = 0;j < col;j++)
	{
		unsigned short usTmp = predMat[j];
		int nCnt = 0;
		while(usTmp)
        {
            usTmp >>= 1;
            nCnt++;
        }
        bitsCnt[j] = (nCnt+1)/2;
		signMat[j] = 0;//dist = 0
	}


    for(int i = 1; i < row; i++)								
    {
		int index = i*col;
        for(int j = 0; j < col; j++)
        {
			usTrueValue = _16mat[index+j];
			int dist = (usTrueValue) - (usLastValue[j]/i);
			unsigned short usTmp = usJump[j] ^ GABS(dist);
			if(GABS(dist) < FPC_SENSITIVITY)
			{
				usJump[j] = GABS(dist);
			}
			usLastValue[j] += usTrueValue;
			predMat[index+j] = usTmp;
			if(dist < 0)
				signMat[index+j] = 1;
			else
				signMat[index+j] = 0;

			int nCnt = 0;
			while(usTmp)
			{
				usTmp >>= 1;
				nCnt++;
			}
			bitsCnt[index+j] = (nCnt+1)/2;
        }
    }
    delete [] usJump; usJump = 0;
    delete [] usLastValue; usLastValue = 0;
}
void FPC::statistic(const int *bitsCnt,int *pZeroNum,int *equalZeroCnt,const int row,const int col)
{
	for(int j = 0; j < col; j++)
    {
        int nCnt=1;
        for(int i = row-1; i >= 0; --i)
        {
            while(i && bitsCnt[i*col+j] == bitsCnt[(i-1)*col+j])
            {
                nCnt++;
                equalZeroCnt[i*col+j]=0;
                --i;
                if(nCnt == 9) break;
            }
            equalZeroCnt[i*col+j] = nCnt;
            pZeroNum[ 8 - bitsCnt[i*col+j] ]++;
            nCnt=1;
        }
    }
}

void FPC::encodeHuffmanTable(const CHuffman *hf)
{
	assert(HUFF_S_LEN == 9);
	for(int i = 0; i < HUFF_S_LEN; ++i)
    {
        __saveBits(hf->m_DicArray[i].length,4); //save huffman
    }
}
void FPC::decodeHuffmanTable(CHuffman *hf)
{
	int CodeLen[HUFF_S_LEN+1];
    for(int i = 0; i < 9; i++)
    {
        CodeLen[i] = __readBits(4);
    }
	memset(hf->len,0,sizeof(hf->len));
    for(int i=0; i<HUFF_S_LEN; ++i)
    {
		hf->m_DicArray[i].length	= CodeLen[i];
        hf->m_DicArray[i].code		=hf->len[CodeLen[i]];
        hf->rec[CodeLen[i]][hf->len[CodeLen[i]]]=i;
        hf->len[CodeLen[i]]++;
    }

    hf->first[HUFF_S_LEN+1]=0;
    for(int i=HUFF_S_LEN; i>0; --i)
    {
        hf->first[i]=(hf->first[i+1]+hf->len[i+1]+1)/2;
    }

    for(int i=0; i<HUFF_S_LEN; ++i)
    {
		hf->m_DicArray[i].code += hf->first[hf->m_DicArray[i].length];
    }
}
void FPC::encodePredMat16(const unsigned short *predMat,const bool *signMat,\
		const int *bitsCnt,const int *equalZeroCnt,const CHuffman *hf,\
		const int row,const int col)
{
	for(int i = 0;i < row;i++)
	{
		int index = i*col;
		for(int j = 0;j < col;j++)
		{	
            int nCnt = bitsCnt[index + j];
            if(equalZeroCnt[index + j])
            {
                if(equalZeroCnt[index + j] == 1)
                {
					//
                    __saveBits(0,1);
                    __saveBits(hf->m_DicArray[8-nCnt].code,hf->m_DicArray[8-nCnt].length);
                }
                else
                {
                    __saveBits(1,1);
                    __saveBits(equalZeroCnt[index + j] - 2, 3);//[0,7] [2,9]
                    __saveBits(hf->m_DicArray[8-nCnt].code, hf->m_DicArray[8-nCnt].length);
                }
            }

            if(signMat[index + j])
                __saveBits(1,1);
            else
                __saveBits(0,1);

            if(nCnt > 0)
            {
                __saveBits(predMat[index + j], nCnt*2);
            }
		}
	}
}
void FPC::encodePredMat16P(const unsigned short *predMat,const bool *signMat,\
		const int *bitsCnt,const int *equalZeroCnt,\
		const CHuffman *hf,\
		const int row,const int col)
{
	for(int i = 0;i < row;i++)
	{
		int index = i*col;
		for(int j = 0;j < col;j++)
		{
            int nCnt = bitsCnt[index + j];
            if(equalZeroCnt[index + j])
            {
                if(equalZeroCnt[index + j] == 1)
                {
					//
                    __saveBits(0,1);
                    __saveBits(hf->m_DicArray[8-nCnt].code,hf->m_DicArray[8-nCnt].length);
                }
                else
                {
                    __saveBits(1,1);
                    __saveBits(equalZeroCnt[index + j] - 2, 3);//[0,7] [2,9]
                    __saveBits(hf->m_DicArray[8-nCnt].code, hf->m_DicArray[8-nCnt].length);
                }
            }

			__saveBits(signMat[index + j],1);			

            if(nCnt > 0)
            {
                __saveBits(predMat[index + j], nCnt*2);
            }
		}
	}
}
int FPC::__decodeHuffmanCode(const CHuffman *hf)
{
    int l=1;
    int v=__readBits(1);
	while(v < (hf->first[l]))
    {
        v=(v<<1)+__readBits(1);
        l++;
    }
	int ind = v - hf->first[l];
	return hf->rec[l][ind];
}
void FPC::decode16mat(unsigned short *_16mat,const CHuffman *hf,const int row,const int col)
{
	int *equalZeroCnt = new int[col];
	int *lastZeroCnt = new int[col];
	memset(equalZeroCnt,0,sizeof(int)*col);

    unsigned short *usJump		= new unsigned short[col];
	memset(usJump,0,sizeof(unsigned short)*col);
    unsigned short *usLastValue = new unsigned short[col];
	memset(usLastValue,0,sizeof(unsigned short)*col);
	unsigned short usTrueValue;

	for(int i = 0;i < row;i++)
	{
		for(int j = 0;j < col;j++)
		{
			int zeroCnt = -1;//
			if(equalZeroCnt[j] == 0)
			{
				if((__readBits(1)) & 1)
				{
					equalZeroCnt[j] = ( __readBits(3) + 2 );
				}
				else
				{
					equalZeroCnt[j] = 1;
				}
				zeroCnt = __decodeHuffmanCode(hf);
				assert(zeroCnt >= 0 && zeroCnt <= 8);
				lastZeroCnt[j] = zeroCnt;
			}
			else
			{
				zeroCnt = lastZeroCnt[j];
			}
			equalZeroCnt[j]--;
			
			bool neg = (__readBits(1) & 1);
			unsigned short usTmp = 0;
			if(8 - zeroCnt)
				usTmp = __readBits( (8 - zeroCnt)*2 );
			int dist = usTmp ^ usJump[j];
			if(neg)
				dist = -dist;

			if(i && GABS(dist)<FPC_SENSITIVITY)
            {
                usJump[j] = GABS(dist);
            }
			usTrueValue = usLastValue[j] + dist;
			_16mat[i*col+j] = usTrueValue;
			usLastValue[j] = usTrueValue;
		}
	}
    delete [] usLastValue; usLastValue = 0;
	delete [] usJump; usJump = 0;
	delete [] equalZeroCnt; equalZeroCnt = 0;
	delete [] lastZeroCnt; lastZeroCnt = 0;
}
void FPC::decode16matP(unsigned short *_16mat,const CHuffman *hf,const int row,const int col)
{
	int *equalZeroCnt = new int[col];					//successive equal zeroBitsCnt num
	int *lastZeroCnt = new int[col];					//last clip zeroCnt
	memset(equalZeroCnt,0,sizeof(int)*col);


    unsigned short *usJump	= new unsigned short[col];
	memset(usJump,0,sizeof(unsigned short)*col);
    unsigned int *usLastValue = new unsigned int[col];	//sum,exclude zero
	memset(usLastValue,0,sizeof(unsigned int)*col);
	unsigned short usTrueValue;

	for(int i = 0;i < row;i++)
	{
		int index = col*i;
		for(int j = 0;j < col;j++)
		{
			int zeroCnt = -1;//
			if(equalZeroCnt[j] == 0)
			{
				if((__readBits(1)) & 1)
				{
					equalZeroCnt[j] = ( __readBits(3) + 2 );
				}
				else
				{
					equalZeroCnt[j] = 1;
				}
				zeroCnt = __decodeHuffmanCode(hf);
				assert(zeroCnt >= 0 && zeroCnt <= 8);
				lastZeroCnt[j] = zeroCnt;
			}
			else
			{
				zeroCnt = lastZeroCnt[j];
			}
			equalZeroCnt[j]--;

			bool neg = (__readBits(1) & 1);
			unsigned short usTmp = 0;

			if(8 - zeroCnt)
				usTmp = __readBits( (8 - zeroCnt)*2 );

			int dist = usTmp ^ usJump[j];
			if(neg)
				dist = -dist;

			if(i && GABS(dist)<FPC_SENSITIVITY)
            {
                usJump[j] = GABS(dist);
            }
			if(i)
				usTrueValue = usLastValue[j]/i + dist;
			else
				usTrueValue = usLastValue[j] + dist;
			_16mat[index+j] = usTrueValue;
			usLastValue[j] += usTrueValue;
		}
	}
	for(int i = 0;i < col;i++)
		assert(equalZeroCnt[i] == 0);
    delete [] usLastValue; usLastValue = 0;
	delete [] usJump; usJump = 0;
	delete [] equalZeroCnt; equalZeroCnt = 0;
	delete [] lastZeroCnt; lastZeroCnt = 0;
}
void FPC::encodeExceed(const int *exceed,const int row,const int col)
{
	for(int i = 0;i < row;i++)
	{
		int index = col*i;
		for(int j = 0;j < col;j++)
		{
			int t = exceed[index + j];
			if(GABS(t) != 0)
			{
				assert(t>= - 4 && t <= 4);//1 2 3 4, 00 01 10 11
				if(t < 0) __saveBits(1,1);
				else	__saveBits(0,1);
				__saveBits(GABS(t)-1,2);
			}
		}
	}
}
void FPC::decode64mat(unsigned short *_16mat,double *mat,const int row,const int col)
{
	for(int i = 0;i < row;i++)
	{
		int index = col*i;
		for(int j = 0;j < col;j++)
		{
			mat[index+j] = __15To64(_16mat[index+j]);
			bool ex = (_16mat[index+j] >> 15) & 1;
			if(ex)
			{
				int neg = (__readBits(1) ) & 1 ;
				int t = (__readBits(2) + 1);
				if(neg)
				{
					mat[index+j] -= t;
				}
				else
				{
					mat[index+j] += t;
				}
			}
		}
	}
}
void FPC::__saveBits(int nCode,int nBitCnt)
{

    assert(nBitCnt <= 25);

    if(-1 == nCode)
    {
        if(m_bitBufferCnt)
        {
            m_bitBuffer >>= 24;

            m_charBuffer->push_back( (unsigned char)m_bitBuffer );
            m_charBufferCnt++;

            m_bitBufferCnt = 0;
            m_bitBuffer    = 0;
        }
        return;
    }

    m_bitBuffer |= ((unsigned int) nCode) << (32 - nBitCnt - m_bitBufferCnt);
    m_bitBufferCnt += nBitCnt;
    while (m_bitBufferCnt >= 8)
    {
        assert(m_charBufferCnt == m_charBuffer->size());
        m_charBuffer->push_back( (unsigned char)( m_bitBuffer >> 24) );
        m_charBufferCnt++;

        m_bitBuffer     <<= 8;
        m_bitBufferCnt   -= 8;
    }
}
unsigned int FPC::__readBits(int nBitCnt)
{
    assert(nBitCnt <= 25 && nBitCnt >= 1);
    int c;
    unsigned int return_value;
    while (m_bitBufferCnt <= 24)
    {
		c = m_charBuffer->at(m_charBufferCnt++);

        m_bitBuffer |= (unsigned int) c << (24-m_bitBufferCnt);
        m_bitBufferCnt += 8;
    }

    if(m_bitBufferCnt < (unsigned)nBitCnt)
    {
        return -1; /* EOF */
    }

    return_value=m_bitBuffer >> (32-nBitCnt);
    m_bitBuffer <<= nBitCnt;
    m_bitBufferCnt -= nBitCnt;

    return(return_value);
}
