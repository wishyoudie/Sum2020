/* FILE NAME: DEF.H
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Common declaration module.
 */

#ifndef __DEF_H_
#define __DEF_H_

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "MTH/MTH.H"

/* 64 bit integer data type */
typedef long long INT64;
typedef unsigned long long UINT64;

/* Primitives */
typedef struct tagvi6VERTEX
{
  VEC P;  /* Vertex position */
} vi6VERTEX;

typedef struct tagvi6PRIM
{
  vi6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} vi6PRIM;

#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */