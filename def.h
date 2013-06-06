
/**
 * def.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef DEF_H
#define DEF_H


#define INDEX(row,col,rowLen) (row*rowLen+col)
#define MS_BUILD_ENV
//#define DIRECTX_ENABLE
#define FLOAT_64
//#define FLOAT_32

#ifdef FLOAT_64
#define FLOAT double
#endif

#ifdef FLOAT_32
#define FLOAT float
#endif

#endif
