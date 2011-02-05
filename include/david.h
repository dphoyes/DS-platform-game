/**
 * @file   david.c
 * @author David Hoyes
 * @brief  Header file for David-related stuff
 */
 
#ifndef _DAVID_H
#define _DAVID_H

#include <PA9.h>

typedef struct coords
{
  s32 x;
  s32 y;
} coords_t;

typedef struct david_attrs
{
  coords_t roomPos;
  coords_t screenPos;
  coords_t viewPos;
  u32 speed;
  s32 verticalVelocity;
  u8 flipped;
  u8 topBound;
  u8 bottomBound;
  u8 leftBound;
  u8 rightBound;
} david_t;

void david_step (david_t *david);
u32 queryTileAt (s32 x, s32 y);
u16 queryTileTypeAt (s32 x, s32 y);
u16 queryCollisionUp (david_t *david);
u16 queryCollisionLeft (david_t *david);
u16 queryCollisionRight (david_t *david);
u16 queryCollisionDown (david_t *david);
u16 queryOnSolid (david_t *david);
u16 queryIfSolid (s32 x, s32 y);

#endif