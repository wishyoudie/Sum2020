/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render primitives module.
 */

#include "RND.H"

/* Create primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Number of vertexes:
 *      - INT NoofV;
 *   - Number of indexes:
 *      - INT NoofI;
 * RETURNS: None.
 */
BOOL VI6_RndPrimCreate( vi6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(vi6PRIM));
  size = sizeof(vi6VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
} /* End of 'VI6_RndPrimCreate' function */

/* Get rid of primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 * RETURNS: None.
 */
VOID VI6_RndPrimFree( vi6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(vi6PRIM));
} /* End of 'VI6_RndPrimFree' function */

/* Draw primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Where to draw matrix:
 *      - MATR World;
 * RETURNS: None.
 */
VOID VI6_RndPrimDraw( vi6PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, VI6_RndMatrVP);
  INT i;

  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);

  /* Draw triangles */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  glEnd();
} /* End of 'VI6_RndPrimDraw' function */

/* Create sphere primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Center:
 *      - VEC C;
 *   - Radius:
 *      - DBL R;
 *   - Amount of meridians:
 *      - INT SplitW;
 *   - Amount of parallels:
 *      - INT SplitH;
 * RETURNS: None.
 */
VOID VI6_RndPrimSphereCreate( vi6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, k = 0;
  DBL phi, theta;

  VI6_RndPrimCreate(Pr, SplitW * SplitH, 3 * 2 * (SplitW - 1) * (SplitH - 1));

  for (i = 0, theta = 0; i < SplitH; i++, theta += (PI / (SplitH - 1)))
    for (j = 0, phi = 0; j < SplitW; j++, phi += (2 * PI / (SplitW - 1)))
      Pr->V[k++].P = VecSet(C.X + R * sin(theta) * sin(phi), C.Y + R * cos(theta), C.Z + R * sin(theta) * cos(phi));

  k = 0;
  for (i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Pr->I[k++] = i * SplitW + j;
      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;

      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;
      Pr->I[k++] = (i + 1) * SplitW + j + 1;
    }
} /* End of 'VI6_RndPrimSphereCreate' function */

/* Create torus primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Center:
 *      - VEC C;
 *   - Inner radius:
 *      - DBL R1;
  *   - Circle radius:
 *      - DBL R2;
 *   - Amount of meridians:
 *      - INT SplitW;
 *   - Amount of parallels:
 *      - INT SplitH;
 * RETURNS: None.
 */
VOID VI6_RndPrimTorusCreate( vi6PRIM *Pr, VEC C, DBL R1, DBL R2, INT SplitW, INT SplitH )
{
  INT i, j, k = 0;
  DBL phi, theta;

  VI6_RndPrimCreate(Pr, SplitW * SplitH, 3 * 2 * (SplitW - 1) * (SplitH - 1));

  for (i = 0, theta = -PI; i < SplitH; i++, theta += (2 * PI / (SplitH - 1)))
    for (j = 0, phi = 0; j < SplitW; j++, phi += (2 * PI / (SplitW - 1)))
      Pr->V[k++].P = VecSet(C.X + (R1 + R2 * cos(phi)) * cos(theta),
                            C.Y + (R1 + R2 * cos(phi)) * sin(theta),
                            C.Z + R2 * sin(phi));

  k = 0;
  for (i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Pr->I[k++] = i * SplitW + j;
      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;

      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;
      Pr->I[k++] = (i + 1) * SplitW + j + 1;
    }
} /* End of 'VI6_RndPrimTorusCreate' function */

/* Create cone primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Center:
 *      - VEC C;
 *   - Height:
 *      - DBL H;
  *   - Radius:
 *      - DBL R;
 *   - Amount of triangles:
 *      - INT N;
 * RETURNS: None.
 */
VOID VI6_RndPrimConeCreate( vi6PRIM *Pr, VEC C, DBL H, DBL R, INT N )
{
  INT i, k;
  DBL angle;

  VI6_RndPrimCreate(Pr, N + 1, 3 * (N - 1));

  /* Fill virticles */
  k = 0;
  Pr->V[k++].P = VecSet(C.X, C.Y + H, C.Z);

  for (i = 0, angle = 0; i < N; i++, angle += 2 * PI / (N - 1))
    Pr->V[k++].P = VecSet(C.X + R * sin(angle), C.Y, C.Z + R * cos(angle));

  /* Fill indices */
  k = 0;
  for (i = 0; i < N - 1; i++)
  {
    Pr->I[k++] = 0;
    Pr->I[k++] = i + 1;
    Pr->I[k++] = i + 2;
  }
} /* End of 'VI6_RndPrimConeCreate' function */

/* Load primitive from .OBJ file function
 * ARGUMENTS:
 *   - Pointer to primitive to be built:
 *      - vi6PRIM *pr;
 *   - File to load from:
 *      - CHAR *FileName;
 * RETURNS: 
 * (BOOL) True is successfull, FALSE is otherwise.
 */
BOOL VI6_RndPrimLoad( vi6PRIM *Pr, CHAR *FileName )
{
  INT nv, nf;
  FILE *F;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(vi6PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Vertex count */
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  /* Create primitive */
  if(!VI6_RndPrimCreate(Pr, nv, 3 * nf))
  {
    fclose(F);
    return FALSE;
  }
  /* Load geometry data */
  rewind(F);
  nv = 0;
  nf = 0;
  while(fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3;
      Pr->I[nf++] = n1 - 1;
      Pr->I[nf++] = n2 - 1;
      Pr->I[nf++] = n3 - 1;
    }

  fclose(F);
  return TRUE;
} /* End of 'VI6_RndPrimLoad' function */

/* END OF 'RNDPRIM.C' FILE */