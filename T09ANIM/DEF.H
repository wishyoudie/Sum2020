/* FILE NAME: DEF.H
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Common declaration module.
 */

#ifndef __DEF_H_
#define __DEF_H_

#pragma warning(disable: 4244 4305)
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

/* Primitive vertex structure */
typedef struct tagvi6VERTEX
{
  VEC P;  /* Vertex position */
  VEC2 T; /* Vertex texture */
  VEC N;  /* Vertex normal */
  VEC4 C; /* Vertex color */
} vi6VERTEX;
/* Primitive type structure */
typedef enum tagvi6PRIM_TYPE
{
  VI6_RND_PRIM_TRIMESH,    /* Triangle mesh - array of triangles */
  VI6_RND_PRIM_TRISTRIP,   /* Triangle strip - array of stripped triangles */
  VI6_RND_PRIM_LINES,      /* Line segments (by 2 points) */
  VI6_RND_PRIM_POINTS,     /* Array of points */
} vi6PRIM_TYPE;
/* Primitive structure */
typedef struct tagvi6PRIM
{
  vi6PRIM_TYPE Type;   /* Primitive type */
  INT NumOfElements;   /* Number of vertices */
  INT 
     VA,               /* Vertex array Id */
     VBuf,             /* Vertex buffer Id */
     IBuf;             /* Index buffer Id (if 0 - use only vertex buffer) */
  MATR Trans;          /* Additional transformation matrix */
  INT MtlNo;
  VEC MinBB, MaxBB;    /* Minimum and maximum bound box values */
  INT InstanceCnt;     /* Counter for instancing, 0 - not use */
} vi6PRIM;

#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */