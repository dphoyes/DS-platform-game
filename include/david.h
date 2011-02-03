/**
 * @file   david.c
 * @author David Hoyes
 * @brief  Header file for David-related stuff
 */
 
#ifndef _DAVID_H
#define _DAVID_H

#include <PA9.h>

typedef struct david_attrs
{
  s32 x;
  s32 y;
  u32 speed;
  s32 verticalSpeed;
} david_t;

void david_step (david_t *david);
u32 queryTileAt (u32 x, u32 y);
u16 queryTileTypeAt (u32 x, u32 y);
u16 queryCollisionLeft (david_t *david);
u16 queryCollisionRight (david_t *david);

#endif