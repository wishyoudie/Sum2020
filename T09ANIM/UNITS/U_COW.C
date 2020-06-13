/* FILE NAME: U_COW.C
 * PROGRAMMER VI6
 * DATE: 12.06.2020
 * PURPOSE: Cow model file for testing
 */

#include "../UNITS/UNITS.H"

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIM Cow;
} vi6UNIT_COW;


/* Cow unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_COW *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_COW *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimLoad(&Uni->Cow, "cow.obj");
} /* End of 'VI6_UnitInit' function */

/* Cow unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_COW *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_COW *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimFree(&Uni->Cow);
} /* End of 'VI6_UnitClose' function */

/* Cow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_COW *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_COW *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Cow unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_COW *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_COW *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimDraw(&Uni->Cow, MatrMulMatr3(MatrScale(VecSet1(0.3)), MatrTranslate(VecSet(-0.5, 0, 1.5)), MatrRotateY(15 * VI6_Anim.Time)));
} /* End of 'VI6_UnitRender' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateCow( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateCow' function */

/* END OF U_COW.C FILE */