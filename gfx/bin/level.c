#include <PA_BgStruct.h>

extern const char level_Tiles[];
extern const char level_Map[];
extern const char level_Pal[];

const PA_BgStruct level = {
	PA_BgNormal,
	256, 192,

	level_Tiles,
	level_Map,
	{level_Pal},

	704,
	{1536}
};
