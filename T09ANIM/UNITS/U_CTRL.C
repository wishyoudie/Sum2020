/* FILE NAME: U_CTRL.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: Control model file.
 */

#define PLSH 310
#define PLSW 300
#include "../UNITS/UNITS.H"

typedef struct tagvi6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  /* VEC CamLoc; */
  DBL
    Dist, /* Camera distance from origin */
    Elevator,
    Azimuth;

  vi6PRIM Axes;
  vi6PRIM Plane;
} vi6UNIT_CONTROL;


/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CONTROL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_CONTROL *Uni, vi6ANIM *Ani )
{
  HFONT hFnt, hFntOld;
  HDC hDC = GetDC(NULL);
  vi6VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
    {{3000, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
    {{0, 3000, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}},
    {{0, 0, 3000}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}}
  };
  INT i, j;
  static vi6VERTEX Pl[PLSH][PLSW];

  hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
    RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_SWISS | VARIABLE_PITCH, "Consolas");
  hFntOld = SelectObject(Ani->hDC, hFnt);
  wglUseFontBitmaps(Ani->hDC, 0, 256, 3030);
  SelectObject(Ani->hDC, hFntOld);
  DeleteObject(hFnt);

  VI6_RndPrimCreate(&Uni->Axes, VI6_RND_PRIM_LINES, V, 6, NULL, 0);

  for (i = 0; i < PLSH; i++)
    for (j = 0; j < PLSW; j++)
    {
      DBL
        a = j / (PLSW - 1.0), b = i / (PLSH - 1.0);
      Pl[i][j].P = VecSet(j - PLSW / 2, 0, PLSH / 2 - i);
      Pl[i][j].C = Vec4Set(0, 1, 0, 1);
    }
  VI6_RndPrimCreateFromGrid(&Uni->Plane, Pl[0], PLSW, PLSH, TRUE);
  Uni->Azimuth = 45;
  Uni->Elevator = 45;
  Uni->Dist = 8;
  wglUseFontBitmaps(hDC, 0, 256, 102);
  ReleaseDC(NULL, hDC);
} /* End of 'VI6_UnitInit' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CONTROL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_CONTROL *Uni, vi6ANIM *Ani )
{
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['S'])
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Uni->Dist += Ani->GlobalDeltaTime * (2 * Ani->Mdz + 80 * (1 + Ani->Keys[VK_SHIFT] * 300) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
  Uni->Azimuth += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 150 * -(Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->Elevator += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 47 * -(Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

  if (Uni->Elevator > 89.99)
    Uni->Elevator = 89.99;
  else if (Uni->Elevator < -89.99)
    Uni->Elevator = -89.99;

  if (Uni->Dist < 0.0002)
    Uni->Dist = 0.0002;

  VI6_RndCamSet(PointTransform(VecSet(0, 0, Uni->Dist),
                  MatrMulMatr(MatrRotateX(Uni->Elevator),
                              MatrRotateY(Uni->Azimuth))),
                VecSet(0, 0, 0),
                VecSet(0, 1, 0));


} /* End of 'VI6_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CONTROL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_CONTROL *Uni, vi6ANIM *Ani )
{
  INT n;
  MATR m = MatrOrtho(0, Ani->W - 1, Ani->H - 1, 0, -1, 1);
  static CHAR Buf[100];
  
  glLineWidth(8);
  VI6_RndPrimDraw(&Uni->Axes, MatrIdentity());
  //VI6_RndPrimDraw(&Uni->Plane, MatrIdentity());
  glLineWidth(1);

  n = sprintf(Buf, "FPS: %.3f", Ani->FPS);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glLoadMatrixf(m.A[0]);
  glRasterPos2d(10, 30);

  glListBase(3030);
  glCallLists(n, GL_UNSIGNED_BYTE, Buf);
  glEnable(GL_DEPTH_TEST);
  glPopAttrib();
  
} /* End of 'VI6_UnitRender' function */

/* Unit ctrl creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateCtrl( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_CONTROL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateCtrl' function */

/* END OF U_CTRL.C FILE */