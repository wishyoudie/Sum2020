/* FILE NAME: U_CTRL.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: Control model file.
 */
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
} vi6UNIT_CONTROL;

/* Project scene indicators */
BOOL IsWireFrame = FALSE;
BOOL DrawAxes = FALSE;

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
  //HDC hDC = GetDC(NULL);
  vi6VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
    {{3000, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
    {{0, 3000, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}},
    {{0, 0, 3000}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}}
  };
  vi6MATERIAL mtl;

  VI6_RndPrimCreate(&Uni->Axes, VI6_RND_PRIM_LINES, V, 6, NULL, 0);
  mtl = VI6_RndMtlGetDef();
  strncpy(mtl.Name, "AXES", VI6_STR_MAX);
  mtl.ShdNo = VI6_RndShdAdd("AXES");
  Uni->Axes.MtlNo = VI6_RndMtlAdd(&mtl);
  Uni->Azimuth = 45;
  Uni->Elevator = -30;
  Uni->Dist = 1000;
  Ani->IsPause = TRUE;
  //wglUseFontBitmaps(hDC, 0, 256, 102);
  //ReleaseDC(NULL, hDC);
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
  VEC Loc, At, Up;

  /***
   Keyboard project control
  ***/

  /* WireFrame ('1') */
  if (IsWireFrame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   if (Ani->KeysClick['1'])
    IsWireFrame = TRUE;
  if (Ani->KeysClick['1'] && Ani->KeysClick[VK_SHIFT])
    IsWireFrame = FALSE;

  /* Pause ('2') */
  if (Ani->KeysClick['2'])
  {
    Ani->IsPause = TRUE;
  }
  if (Ani->KeysClick['2'] && Ani->KeysClick[VK_SHIFT])
  {
    Ani->IsPause = FALSE;
  }

  /* Draw axes ('3') */
  if (Ani->KeysClick['3'])
    DrawAxes = TRUE;
  if (Ani->KeysClick['3'] && Ani->KeysClick[VK_SHIFT])
    DrawAxes = FALSE;
  /* Time */
  
  /*      FULL DAY              *
   * 00:00 - 04:59  --- Night   *
   * 05:00 - 06:59  --- Sunset  *
   * 07:00 - 18:59  --- Day     *
   * 19:00 - 20:59  --- Sunset  *
   * 21:00 - 00:00  --- Night   *
   *                            */

  if (((INT)Ani->Time % 24) == 7)
    VI6_DayTime = 2;
  if (((INT)Ani->Time % 24) == 5 || ((INT)Ani->Time % 24) == 19)
    VI6_DayTime = 1;
  if (((INT)Ani->Time % 24) == 21)
    VI6_DayTime = 0;

  if (VI6_DayTime == 0) /* Night */
  {
    VI6_RndLightColor = VecSet(0.118, 0.118, 0.118);
    glClearColor(0.000, 0.000, 0.000, 1);
  }
  if (VI6_DayTime == 2) /* Day */
  {
    VI6_RndLightColor = VecSet(1.000, 1.000, 1.000);
    glClearColor(0.216, 0.608, 1.000, 1);
  }
  if (VI6_DayTime == 1) /* Sunset */
  {
    VI6_RndLightColor = VecSet(0.965, 0.278, 0.278);
    glClearColor(0.965, 0.278, 0.278, 1);
  }
 
  /***
    RELEASE CONTROLS
  ***/

  if (VI6_GameStage == 0 && Ani->Keys[VK_SPACE])
  {
    VI6_GameStage = 1;
    Ani->IsPause = FALSE;
    Ani->Time += 12000;
  }

  if (VI6_IsChickReady)
    VI6_GameStage = 2;

  if (VI6_GameStage == 2)
  {
    Ani->IsPause = TRUE;
    VI6_RndLightColor = VecSet(1.000, 1.000, 1.000);
    glClearColor(0.216, 0.608, 1.000, 1);
  }

  /* Game Stage Control (remove when Release) */
  if (Ani->Keys['8'])
    VI6_GameStage = 0;
  if (Ani->Keys['9'])
    VI6_GameStage = 1;
  if (Ani->Keys['0'])
    VI6_GameStage = 2;

  VI6_RndLightDir.X += Ani->DeltaTime * (Ani->Keys['L'] - Ani->Keys['J']);
  VI6_RndLightDir.Z += Ani->DeltaTime * (Ani->Keys['K'] - Ani->Keys['I']);

  Uni->Dist += Ani->GlobalDeltaTime * (2 * Ani->Mdz + 80 * (1 + Ani->Keys[VK_SHIFT] * 25) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]) / (1 + Ani->Keys[VK_CONTROL] * 10));
  Uni->Azimuth += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 150 * -(Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->Elevator += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 47 * -(Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

  if (Uni->Elevator > 89.99)
    Uni->Elevator = 89.99;
  else if (Uni->Elevator < -89.99)
    Uni->Elevator = -89.99;

  if (Uni->Dist < 0.0002)
    Uni->Dist = 0.0002;

  if (VI6_GameStage == 0)
  {
    Loc = PointTransform(VecSet(0, 0, 3), MatrMulMatr(MatrRotateX(-30), MatrRotateY(-25 * sin(Ani->GlobalTime))));
    At = VecSet(0, 0, 0);
    Up = VecSet(0, 1, 0);
    VI6_RndCamSet(Loc, At, Up);
  }
  if (Ani->IsPause && VI6_GameStage != 0)
  {
    Loc = PointTransform(VecSet(0, 0, 100), MatrMulMatr(MatrRotateX(-30), MatrRotateY(Uni->Azimuth)));
    At = VecSet(0, 0, 0);
    Up = VecSet(0, 1, 0);
    VI6_RndCamSet(Loc, At, Up);
  }
  if (VI6_GameStage == 2)
  {
    Loc = PointTransform(VecSet(0, 5, 30), MatrMulMatr(MatrRotateX(0), MatrRotateY(15 * Ani->GlobalTime)));
    At = VecSet(0, 0, 0);
    Up = VecSet(0, 1, 0);
    VI6_RndCamSet(Loc, At, Up);
  }
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
  MATR m1, m2, m3;
  static CHAR Buf[1000];

  if (DrawAxes)
  {
    glLineWidth(3);
    VI6_RndPrimDraw(&Uni->Axes, MatrIdentity());
    glLineWidth(1);
  }

  m1 = VI6_RndMatrView;
  m2 = VI6_RndMatrProj;
  m3 = VI6_RndMatrVP;
  VI6_RndMatrView = MatrIdentity();
  VI6_RndMatrVP = VI6_RndMatrProj = MatrOrtho(0, (Ani->W - 1) / 30.0, -(Ani->H - 1) / 30.0, 0, -1, 1);
  sprintf(Buf,
    "%s, "
    "time: %i\n",
    VI6_DayTime == 0 ? "Night" : VI6_DayTime == 1 ? "Sunset" : "Day",
    (INT)Ani->Time % 24);

  if (VI6_GameStage == 0)
  {
    sprintf(Buf, "Press Space To Start");
    VI6_RndMatrView = MatrMulMatr(MatrScale(VecSet1(1.5)), MatrTranslate(VecSet((Ani->W - 1) / 75.0, -2 * (Ani->H - 1) / 75.0, 0)));
    VI6_RndFntDraw(Buf);
  }

  if (VI6_GameStage == 1)
  {
    if (Ani->IsPause)
    {
    VI6_RndLightColor = VecSet1(0.3);
    glClearColor(0.5, 0.5, 0.5, 1);
    sprintf(Buf, "Paused");
    VI6_RndMatrView = MatrMulMatr(MatrScale(VecSet1(4)), MatrTranslate(VecSet((Ani->W - 1) / 75.0, -1 * (Ani->H - 1) / 75.0, 0)));
    VI6_RndFntDraw(Buf);
    }
    else
    {
      VI6_RndFntDraw(Buf);
    }
  }
  if (VI6_GameStage == 2)
  {
    sprintf(Buf, "GAME OVER!");
    VI6_RndMatrView = MatrMulMatr(MatrScale(VecSet1(4)), MatrTranslate(VecSet((Ani->W - 1) / 90.0, -2 * (Ani->H - 1) / 75.0, 0)));
    VI6_RndFntDraw(Buf);
  }
  VI6_RndMatrView = m1;
  VI6_RndMatrProj = m2;
  VI6_RndMatrVP = m3;
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