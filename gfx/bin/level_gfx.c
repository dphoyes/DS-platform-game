#include <PA_BgStruct.h>

extern const char level_gfx_Tiles[];
extern const char level_gfx_Map[];
extern const char level_gfx_Pal[];

const PA_BgStruct level_gfx = {
	PA_BgLarge,
	528, 256,

	level_gfx_Tiles,
	level_gfx_Map,
	{level_gfx_Pal},

	1920,
	{4224}
};
