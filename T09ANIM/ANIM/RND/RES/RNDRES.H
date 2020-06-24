/* FILE NAME: RNDRES.H
 * PROGRAMMER: VI6
 * DATE: 14.06.2020
 * PURPOSE: 3D animation project.
 *          Render system resource header module.
 */
#ifndef __RNDRES_H_
#define __RNDRES_H_
#include "../rnd.h"

/***
 * Useful defines
***/

#define VI6_STR_MAX 300        /* String maximum length */
#define VI6_MAX_SHADERS 300     /* Shaders stock maximum size */
#define VI6_MAX_TEXTURES 300    /* Textures stock maximum size */
#define VI6_MAX_MATERIALS 3000   /* Material stock */

/***
 * Store types
***/

/* Shaders */
typedef struct tagvi6SHADER
{
  CHAR Name[VI6_STR_MAX];      /* Shader filename prefix */
  INT ProgId;                  /* Shader program ID */
} vi6SHADER;

/* Textures */
typedef struct tagvi6TEXTURE
{
  CHAR Name[VI6_STR_MAX];      /* Texture name */
  INT W, H;                    /* Texture size in pixels */
  UINT TexId;                  /* OpenGL texture Id */ 
} vi6TEXTURE;

/* Materials */
typedef struct tagvi6MATERIAL
{
  CHAR Name[VI6_STR_MAX];      /* Material name */

  /* Illumination coefficients */
  VEC Ka, Kd, Ks;              /* Ambient, diffuse, specular coefficients */
  FLT Ph;                      /* Phong power coefficient */
  FLT Trans;                   /* Transparency factor */
  INT Tex[8];                  /* Material references from texture table (or -1) */
  INT ShdNo;                   /* Shader number in shader array */
} vi6MATERIAL;

/* Images */
typedef struct tagvi6IMAGE
{
  INT W;                  /* Image width */
  INT H;                  /* Image height */
  BYTE *Pixels;           /* Pixels array pointer (W * H * 4bytes per color) */
  DWORD *PixelsDW;        /* Pixels array pointer as DWORD */
  BYTE (*PixelsC)[4];     /* Pixels array pointer as components */
  HBITMAP hBm;            /* Image bitmap */
} vi6IMAGE;

/***
 * Global variables
***/

extern vi6SHADER VI6_RndShaders[VI6_MAX_SHADERS];       /* Array of shaders */
extern INT VI6_RndShadersSize;                          /* Shaders array store size */

extern vi6TEXTURE VI6_RndTextures[VI6_MAX_TEXTURES];    /* Array of textures */
extern INT VI6_RndTexturesSize;                         /* Textures array store size */

extern vi6MATERIAL VI6_RndMaterials[VI6_MAX_MATERIALS]; /* Array of materials */
extern INT VI6_RndMaterialsSize;                        /* Materials array store size */

/***
 * Functions
***/

/* Images */
BOOL VI6_ImgCreate( vi6IMAGE *Im, INT W, INT H );
BOOL VI6_RndImgLoad( vi6IMAGE *Im, CHAR *FileName );
VOID VI6_RndImgFree( vi6IMAGE *Im );

/* Shaders */
INT VI6_RndShdLoad( CHAR *ShaderFileNamePrefix );
VOID VI6_RndShdDelete( INT ProgId );
INT VI6_RndShdAdd( CHAR *ShaderFileNamePrefix );
VOID VI6_RndShdUpdate( VOID );
VOID VI6_RndShdInit( VOID );
VOID VI6_RndShdClose( VOID );

/* Textures */
INT VI6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT VI6_RndTexAdd( CHAR *FileName );
VOID VI6_RndTexInit( VOID );
VOID VI6_RndTexClose( VOID );
INT VI6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType );

/* Materials */
INT VI6_RndMtlAdd( vi6MATERIAL *Mtl );
VOID VI6_RndMtlInit( VOID );
INT VI6_RndMtlApply( INT MtlNo );
VOID VI6_RndMtlClose( VOID );
vi6MATERIAL VI6_RndMtlGetDef( VOID );
vi6MATERIAL * VI6_RndMtlGet( INT MtlNo );

/* Fonts */
BOOL VI6_RndFntLoad( CHAR *FileName );
VOID VI6_RndFntInit( VOID );
VOID VI6_RndFntClose( VOID );
VOID VI6_RndFntDraw( CHAR *Str );

/* All resources */
VOID VI6_RndResInit( VOID );
VOID VI6_RndResClose( VOID );

#endif /* __RNDRES_H_ */
/* END OF 'RNDRES.H' FILE */