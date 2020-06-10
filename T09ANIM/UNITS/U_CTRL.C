/* FILE NAME: U_CTRL.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: Control model file.
 */

#include "../UNITS/UNITS.H"

typedef struct tagvi6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc;
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
  HDC hDC = GetDC(NULL);

  Uni->CamLoc = VecSet(0, 5, 5);
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
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(VI6_Anim.hWnd, &pt);
  //VI6_RndCamSet(PointTransform(VecSet(0, 5, 5), MatrRotateY(26. * Ani->Time)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['S'])
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(VecNormalize(VecSubVec(VecSet(0, 0, 0), Uni->CamLoc)), Ani->GlobalDeltaTime * 30 * Ani->Mdz));
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(-Ani->Keys[VK_LBUTTON] * Ani->GlobalDeltaTime * 30 * Ani->Mdx));
  VI6_RndCamSet(Uni->CamLoc, VecSet(0, 0, 0), VecSet(0, 1, 0));
  /*Uni->CamLoc = PointTransform(Uni->CamLoc, MatrTranslate(VecSet(0, -Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime * 50, -Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime * 50)));
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrTranslate(VecSet(0, Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime * 50, Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime * 50)));
  VI6_RndCamSet(Uni->CamLoc, VecSet(pt.x, pt.y, sqrt(pt.x * pt.x + pt.y * pt.y)), VecSet(0, 1, 0));*/
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

  n = sprintf(Buf, "FPS: %.3f", Ani->FPS);

  glDisable(GL_DEPTH_TEST);
  glLoadMatrixf(m.A[0]);
  glRasterPos2d(15, 30);

  glListBase(102);
  glCallLists(n, GL_UNSIGNED_BYTE, Buf);
  glEnable(GL_DEPTH_TEST);
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