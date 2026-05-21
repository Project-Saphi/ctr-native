#include <common.h>

static void COLL_FIXED_TRIANGL_TestPoint_Body(u8 *sps, u8 *v1, u8 *v2, u8 *v3, s32 normalZW);

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x8001ef1c-0x8001ef50
void COLL_FIXED_TRIANGL_UNUSED(void *sps, void *v1, void *v2, void *v3)
{
	u8 *spsBytes = (u8 *)sps;

	// NOTE(aalhendi): Retail skips the TestPoint setup and jumps into the
	// shared body with t2 preloaded from sps+0x58. Native makes a0-a3 and t2
	// explicit.
	COLL_FIXED_TRIANGL_TestPoint_Body(spsBytes, (u8 *)v1, (u8 *)v2, (u8 *)v3, CollFixed_ReadS32(spsBytes, 0x58));
}
