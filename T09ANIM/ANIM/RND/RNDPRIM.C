/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render primitives module.
 */

//#include "RND.H"
#include "../ANIM.H"

/* Create primitive function
 * ARGUMENTS:
 *   - Primitive:
 *      - vi6PRIM *pr;
 *   - Primitive type:
 *      - vi6PRIM_TYPE Type;
 *   - Number of vertexes:
 *      - INT NoofV;
 *   - Number of indexes:
 *      - INT NoofI;
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_RndPrimCreate( vi6PRIM *Pr, vi6PRIM_TYPE Type, vi6VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(vi6PRIM));

  if (V != NULL)
  {
    /* Generate buffers */
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    /* Fill vertex array and buffer */
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vi6VERTEX) * NumOfV, V, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vi6VERTEX), (VOID *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vi6VERTEX), (VOID *)sizeof(VEC));
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vi6VERTEX), (VOID *)(sizeof(VEC) + sizeof(VEC2)));
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vi6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
  }

  if (I != NULL)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Pr->NumOfElements = NumOfI;
  }
  else
    Pr->NumOfElements = NumOfV;
  Pr->Type = Type;
  Pr->Trans = MatrIdentity();

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
  if (Pr->VA != 0)
  {
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);
  }
  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);
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
  MATR wvp = MatrMulMatr(World, MatrMulMatr(VI6_RndMatrView, VI6_RndMatrProj));
  INT loc;
  INT gl_prim_type = Pr->Type == VI6_RND_PRIM_LINES ? GL_LINES:
                     Pr->Type == VI6_RND_PRIM_TRIMESH ? GL_TRIANGLES:
                     Pr->Type == VI6_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP:
                     GL_POINTS;

  glLoadMatrixf(wvp.A[0]);

  glUseProgram(VI6_RndProgId);

  if ((loc = glGetUniformLocation(VI6_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(VI6_RndProgId, "Time")) != -1)
    glUniform1f(loc, VI6_Anim.Time);

  if (Pr->IBuf != 0)
  {
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  else
  {
    glBindVertexArray(Pr->VA);
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }

  glUseProgram(0);
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
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_RndPrimSphereCreate( vi6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, k = 0, noofv, size;
  DBL phi, theta;
  vi6VERTEX *V;

  memset(Pr, 0, sizeof(vi6PRIM));

  noofv = SplitW * SplitH;
  size = sizeof(vi6VERTEX) * noofv;
  if ((V = malloc(size)) == NULL)
    return FALSE;

  for (i = 0, theta = 0; i < SplitH; i++, theta += (PI / (SplitH - 1)))
    for (j = 0, phi = 0; j < SplitW; j++, phi += (2 * PI / (SplitW - 1)))
    {
      FLT
        x = R * sin(theta) * sin(phi),
        y = R * cos(theta),
        z = R * sin(theta) * cos(phi);
      V[k].P = VecSet(C.X + x, C.Y + y, C.Z + z);

      V[k].C = Vec4Set(0.8, 0.3, 0.47, 1);
      V[k].N = VecSet(x, y, z);
      k++;
    }
    VI6_RndPrimCreateFromGrid(Pr, V, SplitW, SplitH, FALSE);
    free(V);
  return TRUE;
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
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_RndPrimTorusCreate( vi6PRIM *Pr, VEC C, DBL R1, DBL R2, INT SplitW, INT SplitH )
{
  INT i, j, k = 0, noofv, noofi, size;
  DBL phi, theta;
  vi6VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(vi6PRIM));

  noofv = SplitW * SplitH;
  noofi = 3 * 2 * (SplitW - 1) * (SplitH - 1);
  size = sizeof(vi6VERTEX) * noofv + sizeof(INT) * noofi;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  for (i = 0, theta = -PI; i < SplitH; i++, theta += (2 * PI / (SplitH - 1)))
    for (j = 0, phi = 0; j < SplitW; j++, phi += (2 * PI / (SplitW - 1)))
      V[k++].P = VecSet(C.X + (R1 + R2 * cos(phi)) * cos(theta),
                            C.Y + (R1 + R2 * cos(phi)) * sin(theta),
                            C.Z + R2 * sin(phi));

  k = 0;
  for (i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Ind[k++] = i * SplitW + j;
      Ind[k++] = (i + 1) * SplitW + j;
      Ind[k++] = i * SplitW + j + 1;

      Ind[k++] = (i + 1) * SplitW + j;
      Ind[k++] = i * SplitW + j + 1;
      Ind[k++] = (i + 1) * SplitW + j + 1;
    }
    VI6_RndPrimCreate(Pr, VI6_RND_PRIM_TRIMESH, V, noofv, Ind, noofi);
    free(V);
  return TRUE;
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
/*VOID VI6_RndPrimConeCreate( vi6PRIM *Pr, VEC C, DBL H, DBL R, INT N )
{
  INT i, k;
  DBL angle;

  VI6_RndPrimCreate(Pr, N + 1, 3 * (N - 1));

  
  k = 0;
  Pr->V[k++].P = VecSet(C.X, C.Y + H, C.Z);

  for (i = 0, angle = 0; i < N; i++, angle += 2 * PI / (N - 1))
    Pr->V[k++].P = VecSet(C.X + R * sin(angle), C.Y, C.Z + R * cos(angle));

  
  k = 0;
  for (i = 0; i < N - 1; i++)
  {
    Pr->I[k++] = 0;
    Pr->I[k++] = i + 1;
    Pr->I[k++] = i + 2;
  }
} End of 'VI6_RndPrimConeCreate' function */

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
  INT nv = 0, nf = 0, size, i;
  vi6VERTEX *V;
  INT *Ind;
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
  
  size = sizeof(vi6VERTEX) * nv + sizeof(INT) * nf * 3;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  /* Load geometry data */
  rewind(F);
  nv = 0;
  nf = 0;
  while(fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv].P = VecSet(x, y, z);
      V[nv].C = Vec4Set(1, 1, 1, 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3;
      Ind[nf++] = n1 - 1;
      Ind[nf++] = n2 - 1;
      Ind[nf++] = n3 - 1;
    }

  /* Calculate primitive vertex normals */

    /* Set all normals to (0, 0, 0) */
    for (i = 0; i < nv; i++)
      V[i].N = VecSet(0, 0, 0);

    /* Enumerate all triangles */
    for (i = 0; i < nf; i += 3)
    {
      VEC
        p0 = V[Ind[i]].P,
        p1 = V[Ind[i + 1]].P,
        p2 = V[Ind[i + 2]].P,
        N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

      V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
      V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
      V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
    }

    /* Normalize all normals */
    for (i = 0; i < nv; i++)
    {
      V[i].N = VecNormalize(V[i].N);
      V[i].C = Vec4Set(0.8, 0.47, 0.26, 1);
    }

  VI6_RndPrimCreate(Pr, VI6_RND_PRIM_TRIMESH, V, nv, Ind, nf);
  free(V);
  fclose(F);
  return TRUE;
} /* End of 'VI6_RndPrimLoad' function */

/* Primitive creation from vertex grid function
 * ARGUMENTS:
 *   - Pointer to primitive to be built:
 *      - vi6PRIM *Pr;
 *   - Vertex array:
 *        - vi6VERTEX *V;
 *   - Grid size:
 *      - Width: INT W, Height: INT H;
 *   - Normal evaluation flag:
 *      - (BOOL) IsNormalsNeed;
 * RETURNS: TRUE/FAlSE.
 */
BOOL VI6_RndPrimCreateFromGrid( vi6PRIM *Pr, vi6VERTEX *V, INT W, INT H, BOOL IsNormalsNeed )
{
  INT i, j, k;
  INT *Ind;

  memset(Pr, 0, sizeof(vi6PRIM));
  if ((Ind = malloc(((2 * W + 1) * (H - 1) - 1) * sizeof(INT))) == NULL)
    return FALSE;

  for (i = 0, k = 0; i < H - 1; i++)
  {
    for (j = 0; j < W; j++)
    {
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j;
    }
    if (i != H - 2)
      Ind[k++] = -1;
  }

  if (IsNormalsNeed)
  {
    /* Set all normals to (0, 0, 0) */
    for (i = 0; i < W * H; i++)
      V[i].N = VecSet(0, 0, 0);
    /* Enumerate all triangles */
    for (i = 0; i < H - 1; i++)
      for (j = 0; j < W - 1; j++)
      {
        vi6VERTEX
          *P00 = V + i * W + j,
          *P01 = V + i * W + j + 1,
          *P10 = V + (i + 1) * W + j,
          *P11 = V + (i + 1) * W + j + 1;
        VEC N;

        N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                     VecSubVec(P11->P, P10->P)));
        P00->N = VecAddVec(P00->N, N);
        P10->N = VecAddVec(P10->N, N);
        P11->N = VecAddVec(P11->N, N);

        N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                     VecSubVec(P00->P, P01->P)));
        P00->N = VecAddVec(P00->N, N);
        P01->N = VecAddVec(P01->N, N);
        P11->N = VecAddVec(P11->N, N);
      }
    /* Normalize all normals */
    for (i = 0; i < W * H; i++)
      V[i].N = VecNormalize(V[i].N);
  }

  VI6_RndPrimCreate(Pr, VI6_RND_PRIM_TRISTRIP, V, W * H, Ind,
    (2 * W + 1) * (H - 1) - 1);
  free(Ind);
  return TRUE;
} /* End of 'VI6_RndPrimCreateFromGrid' function */

/* END OF 'RNDPRIM.C' FILE */