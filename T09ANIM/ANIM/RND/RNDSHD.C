/* FILE NAME: RNDSHD.C
 * PROGRAMMER: VI6
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *          Render system shader handle module.
 */

#include <stdio.h>
#include <string.h>

#include "rnd.h"

/* Shader program store type */
typedef struct tagvi6SHADER
{
  CHAR Name[VI6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program ID */
} vi6SHADER;

/* Shaders stock maximum size */
#define VI6_MAX_SHADERS 30

/* Array of shaders */
vi6SHADER VI6_RndShaders[VI6_MAX_SHADERS];

/* Shaders array store size */
INT VI6_RndShadersSize;

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * VI6_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F); /* <=> fseek(F, 0, SEEK_SET); */

  /* Allocate memory for file text */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }

  /* Load text from file */
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
} /* End of 'VI6_RndShdLoadTextFromFile' function */


/* Store log to file function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *Prefix, *PartName, *Text;
 * RETURNS: None.
 */
static VOID VI6_RndShdLog( CHAR *Prefix, CHAR *PartName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/VI6{30}SHD.LOG", "a")) == NULL)
    return;
  fprintf(F, "%s/%s.glsl\n%s\n", Prefix, PartName, Text);
  fclose(F);
} /* End of 'VI6_RndShdLog' function */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT VI6_RndShdLoad( CHAR *ShaderFileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader file prefix name (e.g. "VERT") */
    INT Type;   /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
    INT Id;     /* Obtained shader Id from OpenGL */
  } shd[] =
  {
    {"vert", GL_VERTEX_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
  };
  INT NoofS = sizeof(shd) / sizeof(shd[0]), i, res, prg;
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[1000];

  for (i = 0; i < NoofS; i++)
  {
    /* Build file name */
    sprintf(Buf, "BIN/SHADERS/%s/%s.glsl", ShaderFileNamePrefix, shd[i].Name);
    /* Load shader text from file */
    txt = VI6_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      VI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      VI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt);
      is_ok = FALSE;
      break;
    }
    /* Attach shader text to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      VI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create shader program */
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      VI6_RndShdLog(ShaderFileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        VI6_RndShdLog(ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }

  if (!is_ok)
  {
    /* Delete all created shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'VI6_RndShdLoad' function */

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID VI6_RndShdDelete( INT ProgId )
{
  INT shdrs[5], n, i;

  if (ProgId == 0)
    return;

  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'VI6_RndShdDelete' function */

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *      CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT VI6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  if (VI6_RndShadersSize >= VI6_MAX_SHADERS)
    return 0;
  strncpy(VI6_RndShaders[VI6_RndShadersSize].Name,
    ShaderFileNamePrefix, VI6_STR_MAX - 1);
  VI6_RndShaders[VI6_RndShadersSize].ProgId = 
    VI6_RndShdLoad(ShaderFileNamePrefix);
  
  return VI6_RndShadersSize++;
} /* End of 'VI6_RndShdStockAdd' function */

/* Update from file all load shaders function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < VI6_RndShadersSize; i++)
  {
    VI6_RndShdDelete(VI6_RndShaders[i].ProgId);
    VI6_RndShaders[i].ProgId = 
      VI6_RndShdLoad(VI6_RndShaders[i].Name);
  }
} /* End of 'VI6_RndShdUpdate' function */

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndShdInit( VOID )
{
  VI6_RndShdAdd("BIN/SHADERS/DEFAULT");
} /* End of 'VI6_RndShdInit' function */

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < VI6_RndShadersSize; i++)
    VI6_RndShdDelete(VI6_RndShaders[i].ProgId);
  VI6_RndShadersSize = 0;
} /* End of 'VI6_RndShdClose' function */

/* END OF 'RNDSHD.C' FILE */