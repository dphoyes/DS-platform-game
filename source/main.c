/**
 * @file   main.c
 * @author David Hoyes
 * @brief  Super David DS, a platform adventure game for the Nintendo DS
 */

#include "main.h"
#include "david.h"

#include <PA9.h>
#include "all_gfx.h"
#include <maxmod9.h>
#include "soundbank_bin.h"
#include "soundbank.h"


int main()
{
  david_t david;
  david.x=50 << 8;
  david.y=100 << 8;
  david.speed = 1 << 8;
  david.verticalSpeed = 0;
  
  s32 bgx=0;
  s32 bgy=0;
  
  PA_Init();
  PA_LoadBackground (GAME_SCREEN, 0, &level_gfx); // draw level
  PA_LoadBackground (GAME_SCREEN, 1, &skybg); // draw sky
  PA_LoadSpritePal (GAME_SCREEN, 0, (void*)david_Pal); // load david palette, necessary for…
  PA_CreateSprite(GAME_SCREEN, 0, (void*)david_Sprite, OBJ_SIZE_16X32, 1, 0, 0, 0); // create david sprite
  
  /* Play music via Maxmod */
  mmInitDefaultMem((mm_addr)soundbank_bin);
  mmLoad(MOD_MUSIC);
  mmStart(MOD_MUSIC, MM_PLAY_LOOP);
  
  /* Init text */
  PA_LoadDefaultText(OTHER_SCREEN, 0);
  
  
  /* Main Loop */
  while(true)
    {
      /* Print tile */
      if (Stylus.Held)
        {
          PA_ClearTextBg (OTHER_SCREEN);
          PA_OutputText (OTHER_SCREEN, 0, 1, "X: %d\nY: %d\nTile Num: %d\nTile type: %d", Stylus.X, Stylus.Y, queryTileAt (Stylus.X, Stylus.Y), ((u16*)level_coll.BgMap)[queryTileAt (Stylus.X, Stylus.Y)]);
        }
      
      /* Scroll sky background */
      PA_EasyBgScrollXY (GAME_SCREEN, 1, bgx++ >> 1, bgy-- >> 4);
      
      david_step (&david);
      
      PA_WaitForVBL();
    }
}

