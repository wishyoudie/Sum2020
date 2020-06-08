/* FILE NAME: MTH.H
 * PROGRAMMER VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Mathematics module.
 */

#ifndef __MTH_H_
#define __MTH_H_

#include <math.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Base float point types */
typedef double DBL;
typedef double FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

/* Matrix 4x4 type */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

/* Set vector function.
 * ARGUMENTS:
 *   - Vector coordinates:
 *       DBL X, DBL Y, DBL Z;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v = {X, Y, Z};

  return v;
} /* End of 'VecSet' function */

/* Construct vector from equal values function 
 * ARGUMENTS:
 *   - Vector coordinates common value:
 *       DBL A;
 * RETURNS:
 *   (VEC) constructed vector.
 */
__inline VEC VecSet1( DBL A )
{
  VEC v = {A, A, A};

  return v;
} /* End of 'VecSet1' function */

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Subtract two vectors function.
 * ARGUMENTS:
 *   - vector to substract from:
 *       VEC V1;
 *   - vector to substract:
 *       VEC V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector by number function.
 * ARGUMENTS:
 *   - vector to be multiplied:
 *       VEC V1;
 *   - number to mupltiply:
 *       DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

/* Divide vector by number function.
 * ARGUMENTS:
 *   - vector to be divided:
 *       VEC V1;
 *   - number to divide:
 *       DBL N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

/* Create a vector negative to given.
 * ARGUMENTS:
 *   - vector to be negated:
 *       VEC V1;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Calculate dot product of two vectors.
 * ARGUMENTS:
 *   - vectors to be dot producted:
 *       VEC V1, VEC V2;
 * RETURNS:
 *   (DBL) Result number.
 */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* Calculate cross product of two vectors.
 * ARGUMENTS:
 *   - vectors to be cross producted:
 *       VEC V1, VEC V2;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Calculate vector length squared.
 * ARGUMENTS:
 *   - vector:
 *       VEC V1;
 * RETURNS:
 *   (DBL) Vector length squared.
 */
__inline DBL VecLen2( VEC V )
{
  return VecDotVec(V, V);
} /* End of 'VecLen2' function */

/* Calculate vector length.
 * ARGUMENTS:
 *   - vector:
 *       VEC V1;
 * RETURNS:
 *   (DBL) Vector length.
 */
__inline DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 0 || len == 1)
    return len;
  len = sqrt(len);
  return len;
} /* End of 'VecLen' function */

/* Normalize vector.
 * ARGUMENTS:
 *   - vector:
 *       VEC V1;
 * RETURNS:
 *   (VEC) Normalized vector.
 */
__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  len = sqrt(len);
  return VecDivNum(V, len);
} /* End of 'VecNormalize' function */

/* Vector by matrix multiplication (with homogenious divide) function.
 * ARGUMENTS:
 *  - source vector;
 *      VEC V;
 *  - multiplied matrix;
 *      MATR M;
 * RETURNS:
 *  - (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
 DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

 return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
               (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
               (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VelMulMatr' function */

/* Transform vector by matrix function
 * ARGUMENTS:
 *   - Vector to be transformed:
 *      - VEC V;
 *   - Matrix to transform:
 *      - MATR M;
 * RETURNS: Transformed vector.
 */
__inline VEC VectorTransform( VEC V, MATR M )
{
 return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
               V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
               V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' function */

/* Point (radius-vector) by matrix multiplication (without projection) function.
 * ARGUMENTS:
 *  - source vector;
 *      VEC V;
 *  - multiplied matrix;
 *      MATR M;
 * RETURNS:
 *  - (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* Enf of 'PointTransform' function */

/* Helpful i-times matrix multiplication macros */
#define MatrMulMatr2(A, B) MatrMulMatr(A, B)
#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr(MatrMulMatr(A, B), C), D)
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(A, B), C), D), E)

/* Matrix with 1-s on main diagonal */
static MATR UnitMatrix =
{                  
  {                
    {1, 0, 0, 0},  
    {0, 1, 0, 0},  
    {0, 0, 1, 0},  
    {0, 0, 0, 1}  
  }                
};

/* Set matrix by all elements function
 * ARGUMENTS:
 *   - All matrix elements in left-to-down order:
 *      - DBL A00, DBL A01, DBLA02...;
 * RETURNS: Filled matrix:
 *   - MATR m.
 */
__inline MATR MatrSet( DBL A00, DBL A01, DBL A02, DBL A03,
                       DBL A10, DBL A11, DBL A12, DBL A13,
                       DBL A20, DBL A21, DBL A22, DBL A23,
                       DBL A30, DBL A31, DBL A32, DBL A33 )
{
  MATR m = 
  {  
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33}
    }
  };
  return m;
} /* End of 'MatrSet' function */

/* Use matrix that changes nothing function
 * ARGUMENTS: None;
 * RETURNS: UnitMatrix.
 */
__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
} /* End of 'MatrIdentity' function */

/* Use matrix to translate function
 * ARGUMENTS:
 *   - Vector to be translated:
 *      - VEC T;
 * RETURNS: Translate matrix:
 *   - Matr m.
 */
__inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
} /* End of 'MatrTranslate' function */

/* Use matrix to scale function
 * ARGUMENTS:
 *   - Vector to be transformed:
 *      - VEC S;
 * RETURNS: Scale matrix.
 */
__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Y, 0,
                 0, 0, 0, 1);
} /* End of 'MatrScale' function */

/* Use matrix to rotate over X axis function
 * ARGUMENTS:
 *   - Angle (in degrees) to rotate by:
 *      - DBL AngleInDegree;
 * RETURNS: Rotate matrix:
 *   - MATR m.
 */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);
  MATR m =
  {
    {
      {1, 0, 0, 0},
      {0, c, s, 0},
      {0, -s, c, 0},
      {0, 0, 0, 1}
    }
  };
  
  return m;
} /* End of 'MatrRotateX' function */

/* Use matrix to rotate over Y axis function
 * ARGUMENTS:
 *   - Angle (in degrees) to rotate by:
 *      - DBL AngleInDegree;
 * RETURNS: Rotate matrix:
 *   - MATR m.
 */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);
  MATR m =
  {
    {
      {c, 0, -s, 0},
      {0, 1, 0, 0},
      {s, 0, c, 0},
      {0, 0, 0, 1}
    }
  };
  
  return m;
} /* End of 'MatrRotateY' function */

/* Use matrix to rotate over Z axis function
 * ARGUMENTS:
 *   - Angle (in degrees) to rotate by:
 *      - DBL AngleInDegree;
 * RETURNS: Rotate matrix:
 *   - MATR m.
 */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  DBL c = cos(a), s = sin(a);
  MATR m =
  {
    {
      {c, s, 0, 0},
      {-s, c, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  
  return m;
} /* End of 'MatrRotateZ' function */

/* Use matrix to rotate over vector function
 * ARGUMENTS:
 *   - Angle (in degrees) to rotate by:
 *      - DBL AngleInDegree;
 *   - Vector to rotate by:
 *      - VEC V;
 * RETURNS: Rotate matrix:
 *   - MATR m.
 */
__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  DBL
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  VEC A = VecNormalize(V);
  MATR m =
  {  
    {
      {c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s, 0},
      {A.Y * A.X * (1 - c) - A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * s, 0},
      {A.Z * A.X * (1 - c) + A.Y * s, A.Z * A.Y * (1 - c) - A.X * s, c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }
  };
  return m;
} /* End of 'MatrRotate' function */

/* Multiply matrix by matrix function
 * ARGUMENTS:
 *   - First matrix to multiply:
 *      - MATR M1;
 *   - Second matrix to multiply:
 *      - MATR M2;
 * RETURNS: Result matrix:
 *  - MATR r.
 */

/* Nota bene: multiple matrixes multiplication macros is in 'def.h' file. */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' function */

/* Calculate 3x3 matrix determiner function
 * ARGUMENTS:
 *   - All matrix elemenets:
 *      - DBL A00, DBL A01...;
 * RETURNS:
 *   - (DBL) Determiner.
 */
__inline DBL MatrDeterm3x3( DBL A00, DBL A01, DBL A02,
                            DBL A10, DBL A11, DBL A12,
                            DBL A20, DBL A21, DBL A22 )
{
  return A00 * A11 * A22 - A00 * A12 * A21 - A01 * A10 * A22 + 
         A01 * A12 * A20 + A02 * A10 * A21 - A02 * A11 * A20;
} /* End of 'MatrDeterm3x3' function */

/* Calculate matrix determiner function
 * ARGUMENTS:
 *   - Matrix:
 *      - MATR M;
 * RETURNS:
 *   - (DBL) Determiner.
 */
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Calculated inverse matrix function
 * ARGUMENTS:
 *   - Matrix:
 *      - MATR M;
 * RETURNS:
 *   - Inversed matrix:
 *      - MATR r.
 */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][1] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[1][3],
                  M.A[1][0], M.A[1][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][3] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]);
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]);

  r.A[0][0] /= det;
  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;
  r.A[0][1] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;
  r.A[0][2] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;
  r.A[0][3] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;

  return r;
} /* End of 'MatrInverse' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Perspective frustum projection matrix setup function
 * ARGUMENTS:
 *   - frustum side facets coordinates:
 *      DBL Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustum( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {2 * Near / (Right - Left), 0, 0, 0},
      {0, 2 * Near / (Top - Bottom), 0, 0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), -1},
      {0, -2 * Near * Far / (Far - Near), 0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */

/* Perspective ortho projection matrix setup function
 * ARGUMENTS:
 *   - frustum side facets coordinates:
 *      DBL Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrOrtho( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {2 / (Right - Left), 0, 0, 0},
      {0, 2 / (Top - Bottom), 0, 0},
      {0, 0, -2 / (Far - Near), 0},
      {-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1}
    }
  };

  return m;
} /* End of 'MatrOrtho' function */

#endif /* __MTH_H_ */

/* END OF 'MTH.H' FILE */