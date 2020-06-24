/* FILE NAME: U_CHICK.C
 * PROGRAMMER VI6
 * DATE: 16.06.2020
 * PURPOSE: Chicken model file for final project.
 */

#include "../UNITS/UNITS.H"

#define OUT_OF_BORDER (Uni->Pos.X >= 73 || Uni->Pos.X <= -63 || Uni->Pos.Z >= 65 || Uni->Pos.Z <= -75)
#define DESTINY_REACHED (Uni->Pos.Z <= -59 && Uni->Pos.Z >= -63 && Uni->Pos.X <= 5 && Uni->Pos.X >= 0 && VI6_GameStage == 1 && Ani->Keys[VK_SPACE])

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIMS Chick;
  VEC Pos;
  VEC CamPos;
  FLT Head;
  FLT Speed;
  vi6PRIM Mark;
} vi6UNIT_CHICK;

/* Chicken unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CHICK *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_CHICK *Uni, vi6ANIM *Ani )
{
  vi6MATERIAL mtl3 = VI6_RndMtlGetDef();

  VI6_RndPrimsLoad(&Uni->Chick, "BIN/MODELS/Chicken.g3dm");
  Uni->Chick.Trans = MatrMulMatr(MatrRotateY(120), MatrTranslate(VecSet(0, -0.5, 0)));
  VI6_RndPrimSphereCreate(&Uni->Mark, VecSet1(0), 1, 36, 18);
  VI6_IsChickReady = FALSE;

  mtl3.ShdNo = VI6_RndShdAdd("JRED");
  Uni->Mark.MtlNo = VI6_RndMtlAdd(&mtl3);
} /* End of 'VI6_UnitInit' function */

/* Chicken unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CHICK *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_CHICK *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimsFree(&Uni->Chick);
  VI6_RndPrimFree(&Uni->Mark);
} /* End of 'VI6_UnitClose' function */

/* Chicken unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CHICK *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_CHICK *Uni, vi6ANIM *Ani )
{
  Uni->Speed += (Ani->Keys['W'] - Ani->Keys['S']) * 0.011 * Ani->DeltaTime;
  Uni->Head += (Ani->Keys['A'] - Ani->Keys['D']) * 90 * Ani->DeltaTime;
  Uni->Pos.Y += (Ani->Keys['R'] - Ani->Keys['F']) * 4 * Ani->DeltaTime;
  if (Ani->Keys['W'] && Ani->Keys[VK_SHIFT])
    Uni->Speed += 0.0003;
  
  if (Ani->KeysClick['S'] && Ani->KeysClick[VK_SHIFT] || Ani->IsPause)
    Uni->Speed = 0;

  if (OUT_OF_BORDER || DESTINY_REACHED)
    VI6_IsChickReady = TRUE;

  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(VectorTransform(VecSet(0, 0, 1), MatrRotateY(Uni->Head)), Uni->Speed));
  if (VI6_GameStage == 1 && Ani->IsPause == FALSE)
    VI6_RndCamSet(VecAddVec(Uni->Pos, VectorTransform(VecSet(0, 3, -4), MatrRotateY(Uni->Head))), Uni->Pos, VecSet(0, 1, 0));
} /* End of 'VI6_UnitResponse' function */

/* Chicken unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_CHICK *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_CHICK *Uni, vi6ANIM *Ani )
{
  if (VI6_GameStage == 1)
  {
    VI6_RndPrimsDraw(&Uni->Chick, MatrMulMatr(MatrRotateY(Uni->Head), MatrTranslate(Uni->Pos)));
    VI6_RndPrimDraw(&Uni->Mark, MatrTranslate(VecSet(Uni->Pos.X, 40, Uni->Pos.Z)));
  }
} /* End of 'VI6_UnitRender' function */

/* Unit chicken creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateChick( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_CHICK))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateChick' function */

/* END OF U_CHICK.C FILE */