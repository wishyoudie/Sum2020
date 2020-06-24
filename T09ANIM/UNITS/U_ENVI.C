/* FILE NAME: U_ENVI.C
 * PROGRAMMER VI6
 * DATE: 22.06.2020
 * PURPOSE: Environment models file.
 */

#include "../UNITS/UNITS.H"

typedef struct
{
  UNIT_BASE_FIELDS;
  vi6PRIM Stars;
  vi6PRIM Moon;
  vi6PRIMS Shed;
  vi6PRIMS Microwave;
  vi6PRIMS Earth;
  vi6PRIM Destiny;
  vi6PRIMS Cooked;
} vi6UNIT_ENVI;

/* Environment units initialization function.
 * ARGUMENTS:
 *   - self-pointer to units object:
 *       vi6UNIT_ENVI *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_ENVI *Uni, vi6ANIM *Ani )
{
  INT i, shd;
  vi6MATERIAL mtl, mtl2;

  VI6_RndPrimsLoad(&Uni->Shed, "BIN/MODELS/Shed.g3dm");
  VI6_RndPrimsLoad(&Uni->Microwave, "BIN/MODELS/Microwave.g3dm");
  VI6_RndPrimsLoad(&Uni->Earth, "BIN/MODELS/Earth.g3dm");
  VI6_RndPrimsLoad(&Uni->Cooked, "BIN/MODELS/Destiny.g3dm");
  VI6_RndPrimSphereCreate(&Uni->Stars, VecSet1(0), 500, 36, 18);
  VI6_RndPrimSphereCreate(&Uni->Moon, VecSet(-100, 50, -100), 20, 50, 50);
  VI6_RndPrimSphereCreate(&Uni->Destiny, VecSet(0, 40, -60), 1, 36, 18);

  Uni->Microwave.Trans = MatrTranslate(VecSet(-5, 0, 0));
  Uni->Shed.Trans = MatrRotateX(-90);
  Uni->Cooked.Trans = MatrRotateZ(90);
  Uni->Stars.Type = VI6_RND_PRIM_POINTS;

  shd = VI6_RndShdAdd("REVTEX");
  mtl = mtl2 = VI6_RndMtlGetDef();
  mtl.ShdNo = VI6_RndShdAdd("AXES");
  Uni->Stars.MtlNo = VI6_RndMtlAdd(&mtl);
  Uni->Moon.MtlNo = VI6_RndMtlAdd(&mtl);
  mtl2.ShdNo = VI6_RndShdAdd("JBLUE");
  Uni->Destiny.MtlNo = VI6_RndMtlAdd(&mtl2);

  for (i = 0; i < Uni->Shed.NumOfPrims; i++)
    VI6_RndMaterials[Uni->Shed.Prims[i].MtlNo].ShdNo = shd;

  for (i = 0; i < Uni->Microwave.NumOfPrims; i++)
    VI6_RndMaterials[Uni->Microwave.Prims[i].MtlNo].ShdNo = shd;
} /* End of 'VI6_UnitInit' function */

/* Environment units deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to units object:
 *       vi6UNIT_ENVI *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_ENVI *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimFree(&Uni->Stars);
  VI6_RndPrimFree(&Uni->Destiny);
  VI6_RndPrimFree(&Uni->Moon);
  VI6_RndPrimsFree(&Uni->Shed);
  VI6_RndPrimsFree(&Uni->Microwave);
  VI6_RndPrimsFree(&Uni->Earth);
  VI6_RndPrimsFree(&Uni->Cooked);
} /* End of 'VI6_UnitClose' function */

/* Environment units inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to units object:
 *       vi6UNIT_ENVI *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_ENVI *Uni, vi6ANIM *Ani )
{ 
} /* End of 'VI6_UnitResponse' function */

/* Environment units render function.
 * ARGUMENTS:
 *   - self-pointer to units object:
 *       vi6UNIT_ENVI *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_ENVI *Uni, vi6ANIM *Ani )
{
  if (VI6_GameStage != 2)
  {
    VI6_RndPrimsDraw(&Uni->Shed, MatrMulMatr(MatrTranslate(VecSet(-5, -1.03, 3)), MatrScale(VecSet1(1.5))));
    VI6_RndPrimsDraw(&Uni->Earth, MatrMulMatr(MatrScale(VecSet1(2)), MatrTranslate(VecSet(5, -1, -5))));
    VI6_RndPrimsDraw(&Uni->Microwave, MatrTranslate(VecSet(5, -1, -65)));
  }
  else
    VI6_RndPrimsDraw(&Uni->Cooked, MatrScale(VecSet1(0.03)));

  if (VI6_GameStage == 1)
    VI6_RndPrimDraw(&Uni->Destiny, MatrIdentity());

  if ((((INT)Ani->Time % 24) < 7 || ((INT)Ani->Time % 24) >= 19) && VI6_GameStage != 2)
  {
    VI6_RndPrimDraw(&Uni->Stars, MatrMulMatr3(MatrTranslate(VecSet(0, 0, 0)), MatrScale(VecSet(10.1, 5.8, 15.239)), MatrRotateX(2.39)));
    if (((INT)Ani->Time % 24) < 5 || ((INT)Ani->Time % 24) >= 21)
      VI6_RndPrimDraw(&Uni->Moon, MatrTranslate(VecSet(0, -30, 0)));
  }
  else
  {
    VI6_RndPrimDraw(&Uni->Stars, MatrMulMatr3(MatrTranslate(VecSet(0, -9000, 0)), MatrScale(VecSet(10.1, 5.8, 15.239)), MatrRotateX(2.39)));
    VI6_RndPrimDraw(&Uni->Moon, MatrTranslate(VecSet(0, -9000, 0)));
  }
} /* End of 'VI6_UnitRender' function */

/* Environment creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created units.
 */
vi6UNIT * VI6_UnitCreateEnvi( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_ENVI))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateEnvi' function */

/* END OF U_ENVI.C FILE */