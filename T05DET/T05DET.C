/* FILE NAME: T05DET.C
 * PROGRAMMER VI6
 * DATE: 03.06.2020
 * PURPOSE: Calculate matrix determinant
 */
#include <stdio.h>
#include <windows.h>

typedef DOUBLE DBL;
#define MAX 3

/* Global variables */
DBL A[MAX][MAX];
INT N;
INT P[MAX];
BOOL IsParity = 1;
DBL Det = 0;

/* Forward declaration */
BOOL LoadMatrix( CHAR *FileName );
VOID Go( INT Pos );
VOID Swap( INT *A, INT *B );

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

/* Create new permutation function */
VOID Go( INT Pos )
{
  INT i, j;
  DBL prod;
  if (Pos == MAX)
   {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += (IsParity ? 1 : -1) * prod;
  }
  else
  {
    for (j = Pos; j < MAX; j++)
    {
      Swap(&P[Pos], &P[j]);
      if (Pos != j)
        if (IsParity)
          IsParity = 0;
        else
          IsParity = 1;  

      Go(Pos + 1);

      Swap(&P[Pos], &P[j]);
      if (Pos != j)
        if (IsParity)
          IsParity = 0;
        else
          IsParity = 1;
    }
  }
} /* End of 'Go' function */

/* Swap 2 numbers */
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;
  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* Main function */
VOID main( VOID )
{
  INT i;

  if (LoadMatrix("m.TXT"))
  {
    /* Fill with basic permutation */
    for (i = 0; i < N; i++)
      P[i] = i;

    Go(0);

    printf("Determinant: %lf\n", Det);
  }
  else
    printf("Error: no matrix given\n");
  getchar();
} /* End of 'main' function */
/* END OF T05DET.C FILE */