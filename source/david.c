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
  david->roomPos.x += (Pad.Held.Right - Pad.Held.Left) * david->speed;

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
      david->roomPos.x += 1 <<8;
    }
  while (queryCollisionRight(david) == 1)
    {
      PA_OutputText (OTHER_SCREEN, 0, 20, "collision right     ");
      david->roomPos.x -= 1 <<8;
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
  david->roomPos.y += david->verticalVelocity;

  /* Stop falling if he hits the ground */
  if (queryOnSolid (david) != 0 && david->verticalVelocity > 0)
    {
      david->verticalVelocity = 0;
      while (queryCollisionDown (david) != 0)
        {
          david->roomPos.y -= 1 <<8;
        }
    }

  /* Up collision */ 
  while (queryCollisionUp(david) == 1)
    {
      PA_OutputText (OTHER_SCREEN, 0, 20, "collision up     ");
      david->roomPos.y += 1 <<8;
      david->verticalVelocity = 0;
    }
  
  /* Wrap around horizontally */
  if (david->roomPos.x > 527 <<8)
    {
      david->roomPos.x = -16 <<8;
    }
  else if (david->roomPos.x < -16 <<8)
    {
      david->roomPos.x = 527 <<8;
    }
  
  /* Scroll view */
  PA_EasyBgScrollXY (GAME_SCREEN, 1, david->viewPos.x >>8, david->viewPos.y >>8); // for some reason this has to be first, no idea why
  if (david->roomPos.x - david->viewPos.x > (150 <<8))
    {
      david->viewPos.x = david->roomPos.x - (150 <<8) ;
    }
  else if (david->roomPos.x - david->viewPos.x < (100 <<8))
    {
      david->viewPos.x = david->roomPos.x - (100 <<8) ;
    }
  if (david->viewPos.x < 0)
    {
      david->viewPos.x = 0;
    }
  if (david->viewPos.x > (527 - 256) <<8)
    {
      david->viewPos.x = (527 - 256) <<8;
    }
  
  /* Update david's sprite with his new position :) */
  david->screenPos.x = david->roomPos.x - david->viewPos.x;
  david->screenPos.y = david->roomPos.y - david->viewPos.y;
  if (david->screenPos.x < -31 <<8 || david->screenPos.x > 256 <<8)
    {
      david->screenPos.x = 257 <<8;
    }
  PA_SetSpriteXY (GAME_SCREEN, 0, david->screenPos.x >>8, david->screenPos.y >>8);
}

u32 queryTileAt (s32 x, s32 y)
{
  return (y/8)*66 + (x/8); //Will need a better way to pass the room's width to this function…
}

u16 queryTileTypeAt (s32 x, s32 y)
{
  return ((u16*)level_coll.BgMap)[queryTileAt(x,y)];
}

u16 queryCollisionUp (david_t *david)
{
  return  queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->topBound  );
}

u16 queryCollisionLeft (david_t *david)
{
  return  queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->topBound + 8  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->topBound + 16  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->topBound + 24  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->bottomBound  );
}

u16 queryCollisionRight (david_t *david)
{
  return queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->topBound  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->topBound + 8  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->topBound + 16  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->topBound + 24  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->bottomBound  );
}

u16 queryCollisionDown (david_t *david)
{
  return  queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->bottomBound  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->bottomBound  );
}

u16 queryOnSolid (david_t *david)
{
  return  queryIfSolid (  (david->roomPos.x >>8) + david->leftBound    ,    (david->roomPos.y >>8) + david->bottomBound + 1  )
            ||
          queryIfSolid (  (david->roomPos.x >>8) + david->rightBound    ,    (david->roomPos.y >>8) + david->bottomBound + 1  );
}

u16 queryIfSolid (s32 x, s32 y)
{
  if (y < 0)
    {
      y = 0;
    }
  if (x < 0)
    {
      x = 0;
    }
  if (x > 527) //will need to store this value somehow…
    {
      x = 527;
    }
  return queryTileTypeAt(x, y); 
}