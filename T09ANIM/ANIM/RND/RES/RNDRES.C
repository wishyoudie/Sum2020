/* FILE NAME: RNDRES.C
 * PROGRAMMER: VI6
 * DATE: 14.06.2020
 * PURPOSE: 3D animation project.
 *          Render system resources common functions module.
 */

#include "rndres.h"

/* Initialize all resources function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndResInit( VOID )
{
  VI6_RndShdInit();
  VI6_RndTexInit();
  VI6_RndMtlInit();
  VI6_RndFntInit();
} /* End of 'VI6_RndResInit' function */

/* Close all resources function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndResClose( VOID )
{
  VI6_RndShdClose();
  VI6_RndTexClose();
  VI6_RndMtlClose();
  VI6_RndFntClose();
} /* End of 'VI6_RndResInit' function */
/* END OF 'RNDRES.C' FILE */