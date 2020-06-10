/* FILE NAME: U_BBALL.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: Ball model file for testing
 */

#include "../UNITS/UNITS.H"

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIM Ball;
} vi6UNIT_BALL;


/* Bounce ball unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_BALL *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimSphereCreate(&Uni->Ball, VecSet(0, 0, 0), 1, 20, 10);
} /* End of 'VI6_UnitInit' function */

/* Bounce ball unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_BALL *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimFree(&Uni->Ball);
} /* End of 'VI6_UnitClose' function */

/* Bounce ball unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_BALL *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Bounce ball unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_BALL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_BALL *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimDraw(&Uni->Ball, MatrIdentity());
} /* End of 'VI6_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateBall( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_BALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateBall' function */

/* END OF U_BBALL.C FILE */