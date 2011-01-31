#include <PA_BgStruct.h>

extern const char skybg_Tiles[];
extern const char skybg_Map[];
extern const char skybg_Pal[];

const PA_BgStruct skybg = {
	PA_BgNormal,
	256, 256,

	skybg_Tiles,
	skybg_Map,
	{skybg_Pal},

	4096,
	{2048}
};
