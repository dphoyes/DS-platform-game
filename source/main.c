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
  david_t david = {70 <<8, 100 <<8, 0, 0, 0, 0, 31, 4, 11};
  
  s32 bgx=0;
  s32 bgy=0;
  
  int i;
  
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
      for (i=0; i<20; i++)
        {
          PA_OutputText (OTHER_SCREEN, 0, i, "                              ");
        }
      PA_OutputText (OTHER_SCREEN, 0, 0, "Stylus X, Y: (%d, %d)\nStylus Tile Num: %d\nStylus Tile type: %d\nDavid X, Y: (%d, %d)\nDavid's vertical velocity: %d\nTile type below David: %d",
                                          Stylus.X,
                                          Stylus.Y,
                                          queryTileAt (Stylus.X, Stylus.Y),
                                          queryIfSolid (Stylus.X, Stylus.Y),
                                          david.x >>8,
                                          david.y >>8,
                                          david.verticalVelocity >>8,
                                          queryOnSolid(&david)
                                          
                    );
      
      /* Scroll sky background */
      PA_EasyBgScrollXY (GAME_SCREEN, 1, bgx++ >> 1, bgy-- >> 4);
      
      /* Do magic */
      david_step (&david);
      
      /* Wait till next frame */
      PA_WaitForVBL();
    }
  return 0;
}

