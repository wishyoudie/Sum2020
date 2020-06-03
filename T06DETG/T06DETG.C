/* FILE NAME: T06DETG.C
 * PROGRAMMER VI6
 * DATE: 03.06.2020
 * PURPOSE: Calculate matrix determinant using Gauss method
 */
#include <stdio.h>
#include <windows.h>
#include <math.h>

typedef DOUBLE DBL;
#define MAX 3

/* Global variables */
DBL A[MAX][MAX];
INT N;

/* Forward declaration */
BOOL LoadMatrix( CHAR *FileName );
VOID Swap( INT *A, INT *B );

/* Swap 2 numbers */
VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;
  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */
                
/* Load matrix from file */
BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
} /* End of 'LoadMatrix' function */

/* Main function */
VOID main( VOID )
{
  INT i, j, k, x, y, max_col, max_row;
  DBL coef;
  DBL Det = 1;
  if (LoadMatrix("m4.TXT"))
  {
    for (i = 0; i < N; i++)
    {
      /* look for maximum matrix element -> j row k column */
      max_row = max_col = i;
      for (y = i; y < N; y++)
        for (x = i; x < N; x++)
          if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
            max_col = x, max_row = y;
      /* max element = 0 => all elements = 0 */
      if (A[max_row][max_col] == 0)
      {
        Det = 0;
        break;
      }

      if (max_row != i)
      {
        /* Swap max_row row and i row (elements [i..N-1]) */
        for (x = i; x < N; x++)
          Swap(&A[i][x], &A[max_row][x]);
        Det = -Det;
      }
      if (max_col != i)
      {
        /* Swap max_col col and i col (elements [0..N-1]) */
        for (y = 0; y < N; y++)
          Swap(&A[y][i], &A[y][max_col]);
        Det = -Det;
      }

      /* Subtrack from every row j:[i+1..N-1] i row multiplied by A[j][i] / A[i][j] */
      for (j = i + 1; j < N; j++)
      {
        coef = A[j][i] / A[i][i];
        A[j][i] = 0;
        for (k = i + 1; k < N; k++)
          A[j][k] -= A[i][k] * coef;
      }

      Det *= A[i][i];
      
    }

    printf("Determinant: %lf\n", Det);
  }
  else
    printf("Error: no matrix given\n");
  getchar();
} /* End of 'main' function */
/* END OF T06DETG.C FILE */