/////////////////
// Super David //
/////////////////

#include <PA9.h>
#include "all_gfx.h"
#include <maxmod9.h>
#include "soundbank_bin.h"
#include "soundbank.h"

#define GRAVITY 40
#define GAME_SCREEN 1 // set to 0 for bottom screen, 1 for top screen


int main()
{
  s32 x=50 << 8;
  s32 y=50 << 8;
  s32 bgx=0;
  s32 bgy=0;
  u32 speed = 1 << 8;
  s32 verticalSpeed = 0;
  
  PA_Init();
  PA_LoadBackground (GAME_SCREEN, 0, &level); // draw level
  PA_LoadBackground (GAME_SCREEN, 1, &skybg); // draw sky
  PA_LoadSpritePal (GAME_SCREEN, 0, (void*)david_Pal); // load david palette, necessary for…
  PA_CreateSprite(GAME_SCREEN, 0, (void*)david_Sprite, OBJ_SIZE_16X32, 1, 0, 0, 0); // create david sprite
  
  /* Play music via Maxmod */
  mmInitDefaultMem((mm_addr)soundbank_bin);
  mmLoad(MOD_MUSIC);
  mmStart(MOD_MUSIC, MM_PLAY_LOOP);
  
  
  /* Main Loop */
  while(true)
    {
      /* Scroll sky background */
      PA_EasyBgScrollXY (GAME_SCREEN, 1, bgx++ >> 1, bgy-- >> 4);
      
      /* Speed modifier */
      if (Pad.Held.B)
        {
          speed = 2 << 8;
        }
      else if (Pad.Held.X)
        {
          speed = 1 << 6;
        }
      else
        {
          speed = 1 << 8;
        }
      
      /* Move david left and right */
      x += (Pad.Held.Right - Pad.Held.Left) * speed;      
      
      /* Flip david sprite horizontally if direction changed */
      if (Pad.Newpress.Left)
        {
          PA_SetSpriteHflip (GAME_SCREEN, 0, 1);
        }
      else if (Pad.Newpress.Right)
        {
          PA_SetSpriteHflip (GAME_SCREEN, 0, 0);
        }
      
      /* Jump! */
      if (Pad.Newpress.A && y == (192-32-16) << 8) // david must be on the ground in order to jump…
        {
          verticalSpeed = -4 << 8;
        }
      
      /* Fall :( */
      y += verticalSpeed;
      verticalSpeed += GRAVITY;
      
      
      /* Stop falling if he hits the ground */
      if (y > (192-32-16) << 8)
        {
          y = (192-32-16) << 8;
          verticalSpeed = 0;
        }
      
      /* Wrap around horizontally */
      if (x>256 << 8)
        {
          x = -32 << 8;
        }
      else if (x< -32 << 8)
        {
          x = 256 << 8;
        }
      
      /* Update david's sprite with his new position :) */
      PA_SetSpriteXY (GAME_SCREEN, 0, x >> 8, y >> 8);
      
      PA_WaitForVBL();
    }
}
