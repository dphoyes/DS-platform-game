#include <PA_BgStruct.h>

extern const char level_coll_Tiles[];
extern const char level_coll_Map[];
extern const char level_coll_Pal[];

const PA_BgStruct level_coll = {
	PA_BgLarge,
	528, 256,

	level_coll_Tiles,
	level_coll_Map,
	{level_coll_Pal},

	128,
	{4224}
};
