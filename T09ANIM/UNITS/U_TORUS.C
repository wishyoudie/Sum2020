/* FILE NAME: U_TORUS.C
 * PROGRAMMER VI6
 * DATE: 12.06.2020
 * PURPOSE: Torus model file.
 */

#include "../UNITS/UNITS.H"

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIM Torus;
} vi6UNIT_TORUS;


/* Torus unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_TORUS *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_TORUS *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimTorusCreate(&Uni->Torus, VecSet(0, 0, 0), 4, 2, 25, 12);
} /* End of 'VI6_UnitInit' function */

/* Torus unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_TORUS *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimFree(&Uni->Torus);
} /* End of 'VI6_UnitClose' function */

/* Torus unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_TORUS *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_TORUS *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Torus unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_TORUS *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimDraw(&Uni->Torus, MatrIdentity());
} /* End of 'VI6_UnitRender' function */

/* Torus creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateTorus( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_TORUS))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateBall' function */

/* END OF U_BBALL.C FILE */