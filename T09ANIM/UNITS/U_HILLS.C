/* FILE NAME: U_HILLS.C
 * PROGRAMMER VI6
 * DATE: 12.06.2020
 * PURPOSE: Hills model file.
 */

#include "../UNITS/UNITS.H"
#define HILLSW 1024
#define HILLSH 1024

typedef struct tagvi6UNIT_HILLS
{
  UNIT_BASE_FIELDS;
  vi6PRIM Hills;
} vi6UNIT_HILLS;

/* Hills unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_HILLS *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitInit( vi6UNIT_HILLS *Uni, vi6ANIM *Ani )
{
  HBITMAP hBm = LoadImage(NULL, "hf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

  if (hBm)
  {
    BITMAP bm;
    vi6VERTEX *V;
    INT W, H, BPL;
    BYTE *ptr;

    GetObject(hBm, sizeof(bm), &bm);
    W = bm.bmWidth;
    H = bm.bmHeight;
    BPL = bm.bmWidthBytes;
    ptr = bm.bmBits;
    if ((V = malloc(sizeof(vi6VERTEX) * W * H)) != NULL)
    {
      INT x, y;

      memset(V, 0, sizeof(vi6VERTEX) * W * H);
      for (y = 0; y < H; y++)
        for (x = 0; x < W; x++)
        {
          BYTE
            b = ptr[y * BPL + x],
            g = ptr[y * BPL + x],
            r = ptr[y * BPL + x];
          V[y * W + x].P = VecSet(x - W / 2, 40 * g / 255.0, H / 2 - y);
          V[y * W + x].C = Vec4Set(r / 255.0, g / 255.0, b / 255.0, 1);
        }
        VI6_RndPrimCreateFromGrid(&Uni->Hills, V, W, H, TRUE);
        free(V);
    }
    DeleteObject(hBm);
  }
} /* End of 'VI6_UnitInit' function */

/* Hills unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_HILLS *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitClose( vi6UNIT_HILLS *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimFree(&Uni->Hills);
} /* End of 'VI6_UnitClose' function */

/* Hills unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_HILL *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitResponse( vi6UNIT_HILLS *Uni, vi6ANIM *Ani )
{
} /* End of 'VI6_UnitResponse' function */

/* Hills unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vi6UNIT_HILLS *Uni;
 *   - animation context:
 *       vi6ANIM *Ani;
 * RETURNS: None.
 */
static VOID VI6_UnitRender( vi6UNIT_HILLS *Uni, vi6ANIM *Ani )
{
  VI6_RndPrimDraw(&Uni->Hills, MatrIdentity());
} /* End of 'VI6_UnitRender' function */

/* Unit hills creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vi6UNIT *) pointer to created unit.
 */
vi6UNIT * VI6_UnitCreateHills( VOID )
{
  vi6UNIT *Uni;

  if ((Uni = VI6_AnimUnitCreate(sizeof(vi6UNIT_HILLS))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)VI6_UnitInit;
  Uni->Close = (VOID *)VI6_UnitClose;
  Uni->Response = (VOID *)VI6_UnitResponse;
  Uni->Render = (VOID *)VI6_UnitRender;

  return Uni;
} /* End of 'VI6_UnitCreateHills' function */

/* END OF U_HILLS.C FILE */