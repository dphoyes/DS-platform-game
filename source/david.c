/**
 * @file   david.c
 * @author David Hoyes
 * @brief  Implementation file for David-related stuff
 */

#include <PA9.h>
#include "main.h"
#include "david.h"
#include "all_gfx.h"

void david_step (david_t *david)
{
  u32 tile;
  
  /* Speed modifier */
      if (Pad.Held.B)
        {
          david->speed = 2 << 8;
        }
      else if (Pad.Held.X)
        {
          david->speed = 1 << 6;
        }
      else
        {
          david->speed = 1 << 8;
        }
      
      /* Move david left and right */
      david->x += (Pad.Held.Right - Pad.Held.Left) * david->speed;
      
      /* Flip david sprite horizontally if direction changed */
      if (Pad.Newpress.Left)
        {
          PA_SetSpriteHflip (GAME_SCREEN, 0, 1);
        }
      else if (Pad.Newpress.Right)
        {
          PA_SetSpriteHflip (GAME_SCREEN, 0, 0);
        }
      
      /* Left/right collision */
      
      while (queryCollisionLeft(david) == 1)
        {
          david->x += 1 << 8;
        }
      while (queryCollisionRight(david) == 1)
        {
          david->x -= 1 << 8;
        }
      
      /* Jump! */
      if (Pad.Newpress.A && queryTileTypeAt((david->x >> 8) + 8, (david->y >> 8) + 32) != 0) // david must be on the ground in order to jump…
        {
          david->verticalSpeed = -11 << 7;
        }
      
      /* Fall :( */
      david->y += david->verticalSpeed;
      david->verticalSpeed += GRAVITY;
      if (Pad.Held.A == 0 && david->verticalSpeed < 0)
        {
          david->verticalSpeed += 80;
        }
      
      /* Stop falling if he hits the ground */
      tile = queryTileTypeAt((david->x >> 8) + 8, (david->y >> 8) + 32);
      if (tile != 0 && david->verticalSpeed > 0)
        {
          david->verticalSpeed = 0;
          tile = queryTileTypeAt((david->x >> 8) + 8, (david->y >> 8) + 31);
          while (tile != 0)
            {
              david->y -= 1 << 8;
              tile = queryTileTypeAt((david->x >> 8) + 8, (david->y >> 8) + 31);
            }
        }
        
      /* Wrap around horizontally */
      if (david->x>256 << 8)
        {
          david->x = -32 << 8;
        }
      else if (david->x< -32 << 8)
        {
          david->x = 256 << 8;
        }
      
      /* Update david's sprite with his new position :) */
      PA_SetSpriteXY (GAME_SCREEN, 0, david->x >> 8, david->y >> 8);
}

u32 queryTileAt (u32 x, u32 y)
{
  return (y/8)*32 + (x/8);
}

u16 queryTileTypeAt (u32 x, u32 y)
{
  return ((u16*)level_coll.BgMap)[queryTileAt(x,y)];
}

u16 queryCollisionLeft (david_t *david)
{
  return queryTileTypeAt((david->x >> 8) + 5, (david->y >> 8) + 30);
}

u16 queryCollisionRight (david_t *david)
{
  return queryTileTypeAt((david->x >> 8) + 10, (david->y >> 8) + 30);
}