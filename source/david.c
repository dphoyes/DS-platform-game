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
  /* Speed modifier */
      if (Pad.Held.B)
        {
          david->speed = 2 <<8;
        }
      else if (Pad.Held.X)
        {
          david->speed = 1 << 6;
        }
      else
        {
          david->speed = 1 <<8;
        }
      
      /* Move david left and right */
      david->x += (Pad.Held.Right - Pad.Held.Left) * david->speed;
      
      /* Flip david sprite horizontally if direction changed */
      if (Pad.Newpress.Left)
        {
          david->flipped = 1;
          PA_SetSpriteHflip (GAME_SCREEN, 0, 1);
        }
      else if (Pad.Newpress.Right)
        {
          david->flipped = 0;
          PA_SetSpriteHflip (GAME_SCREEN, 0, 0);
        }
      
      /* Left/right collision */
      
      while (queryCollisionLeft(david) == 1)
        {
          PA_OutputText (OTHER_SCREEN, 0, 20, "collision left   ");
          david->x += 1 <<8;
        }
      while (queryCollisionRight(david) == 1)
        {
          PA_OutputText (OTHER_SCREEN, 0, 20, "collision right     ");
          david->x -= 1 <<8;
        }
      
      /* Jump! */
      if (Pad.Newpress.A && queryOnSolid(david)) // david must be on the ground in order to jump…
        {
          david->verticalVelocity = -11 << 7;
        }
      
      /* Fall :( */     
      david->verticalVelocity += GRAVITY;
      if (Pad.Held.A == 0 && david->verticalVelocity < 0)
        {
          david->verticalVelocity += 60;
        }
      if (david->verticalVelocity >>8 > 10)
        {
          david->verticalVelocity = 10 <<8;
        }
      david->y += david->verticalVelocity;
      
      /* Stop falling if he hits the ground */
      if (queryOnSolid (david) != 0 && david->verticalVelocity > 0)
        {
          david->verticalVelocity = 0;
          while (queryCollisionDown (david) != 0)
            {
              david->y -= 1 <<8;
            }
        }
     
      /* Up collision */ 
      while (queryCollisionUp(david) == 1)
        {
          PA_OutputText (OTHER_SCREEN, 0, 20, "collision up     ");
          david->y += 1 <<8;
          david->verticalVelocity = 0;
        }
        
      /* Wrap around horizontally */
      if (david->x > 255 <<8)
        {
          david->x = -16 <<8;
        }
      else if (david->x < -16 <<8)
        {
          david->x = 255 <<8;
        }
      
      /* Update david's sprite with his new position :) */
      PA_SetSpriteXY (GAME_SCREEN, 0, david->x >>8, david->y >>8);
}

u32 queryTileAt (s32 x, s32 y)
{
  return (y/8)*32 + (x/8);
}

u16 queryTileTypeAt (s32 x, s32 y)
{
  return ((u16*)level_coll.BgMap)[queryTileAt(x,y)];
}

u16 queryCollisionUp (david_t *david)
{
  return  queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->topBound  );
}

u16 queryCollisionLeft (david_t *david)
{
  return  queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->topBound + 8  )
            ||
          queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->topBound + 16  )
            ||
          queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->topBound + 24  )
            ||
          queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->bottomBound  );
}

u16 queryCollisionRight (david_t *david)
{
  return queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->topBound + 8  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->topBound + 16  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->topBound + 24  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->bottomBound  );
}

u16 queryCollisionDown (david_t *david)
{
  return  queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->bottomBound  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->bottomBound  );
}

u16 queryOnSolid (david_t *david)
{
  return  queryIfSolid (  (david->x >>8) + david->leftBound    ,    (david->y >>8) + david->bottomBound + 1  )
            ||
          queryIfSolid (  (david->x >>8) + david->rightBound    ,    (david->y >>8) + david->bottomBound + 1  );
}

u16 queryIfSolid (s32 x, s32 y)
{
  if (x < 0)
    {
      x = 0;
    }
  if (x > 255)
    {
      x = 255;
    }
  return queryTileTypeAt(x, y); 
}