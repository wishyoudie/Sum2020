/* FILE NAME: U_EGG.C
 * PROGRAMMER VI6
 * DATE: 22.06.2020
 * PURPOSE: Egg model file.
 */

#include "../UNITS/UNITS.H"

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIMS Egg;
} vi6UNIT_EGG;


/* Egg unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_EGG *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_EGG *Uni, vi6ANIM *Ani )
{
  vi6MATERIAL mtl = VI6_RndMtlGetDef();
  INT i;

  VI6_RndPrimsLoad(&Uni->Egg, "BIN/MODELS/Egg.g3dm");
  Uni->Egg.Trans = MatrTranslate(VecSet(0, -12.9, 0));
  mtl.Ka = VecSet(0.19225, 0.19225, 0.19225);
  mtl.Kd = VecSet(0.50754, 0.50754, 0.50754);
  mtl.Ks = VecSet(0.508273, 0.508273, 0.508273);
  mtl.Ph = 51.2;
  
  for (i = 0; i < Uni->Egg.NumOfPrims; i++)
    Uni->Egg.Prims[i].MtlNo = VI6_RndMtlAdd(&mtl);
} /* End of 'VI6_UnitInit' function */

/* Egg unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_EGG *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_EGG *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimsFree(&Uni->Egg);
} /* End of 'VI6_UnitClose' function */

/* Egg unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_EGG *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_EGG *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Egg unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_EGG *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_EGG *Uni, vi6ANIM *Ani )
{
  if (VI6_GameStage == 0)
    VI6_RndPrimsDraw(&Uni->Egg, MatrScale(VecSet1(0.05)));
} /* End of 'VI6_UnitRender' function */

/* Egg creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateEgg( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_EGG))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateEgg' function */

/* END OF U_EGG.C FILE */