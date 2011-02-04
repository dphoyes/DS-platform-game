#include <PA_BgStruct.h>

extern const char level_gfx_Tiles[];
extern const char level_gfx_Map[];
extern const char level_gfx_Pal[];

const PA_BgStruct level_gfx = {
	PA_BgNormal,
	256, 192,

	level_gfx_Tiles,
	level_gfx_Map,
	{level_gfx_Pal},

	1216,
	{1536}
};
