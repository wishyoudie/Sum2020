/* FILE NAME: UNIT.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: 3D Animation unit functions.
 */

#include "ANIM.H"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_AnimUnitCreate( INT Size )
{
  vi6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(vi6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = VI6_UnitInit;
  Uni->Close = VI6_UnitClose;
  Uni->Response = VI6_UnitResponse;
  Uni->Render = VI6_UnitRender;

  return Uni;
} /* End of 'VI6_AnimUnitCreate' function */

/* END OF UNIT.C FILE */