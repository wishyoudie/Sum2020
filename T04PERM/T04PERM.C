/* FILE NAME: T04PERM.C
 * PROGRAMMER VI6
 * DATE: 02.06.2020
 * PURPOSE: Display all permutations of numbers until natural MAX
 */
#include <stdio.h>
#include <windows.h>

/* Permutation array */
#define MAX 3
INT P[MAX];
BOOL IsParity = 1;

/* Forward declaration */
VOID Store( VOID );
VOID Go( INT Pos );
VOID Swap( INT *A, INT *B );

/* Store permutation to log file function */
VOID Store( VOID )
{
  FILE *F;
  INT i;

  F = fopen("PERM.TXT", "a");
  /* if (F != NULL)
    return; */
  
  for (i = 0; i < MAX - 1; i++)
    fprintf(F, "%d, ", P[i]);
  fprintf(F, "%d – parity: %s\n", P[MAX - 1], IsParity ? "even" : "odd");

  fclose(F);
} /* End of 'Store' function */


/* Create new permutation function */
VOID Go( INT Pos )
{
  INT j;
  if (Pos == MAX)
   Store();
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

VOID main( VOID )
{
  INT i;

  /* Fill with basic permutation */
  for (i = 0; i < MAX; i++)
    P[i] = i + 1;

  Go(0);
} /* End of 'main' function */
/* END OF T04PERM.C FILE */