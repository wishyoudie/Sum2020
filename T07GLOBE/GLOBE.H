/* FILE NAME: GLOBE.H
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application header
 */

#define PI 3.14159265358979323846
#define DBL DOUBLE
#define GLOBE_W 36
#define GLOBE_H 18

VOID GlobeSet( DBL Xc, DBL Yc, DBL R );
VOID GlobeDraw( HDC hDC );