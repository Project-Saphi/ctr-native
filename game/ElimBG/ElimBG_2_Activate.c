#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x8002481c-0x80024840.
void ElimBG_Activate(struct GameTracker *gGT)
{
	sdata->pause_backup_renderFlags = gGT->renderFlags;
	sdata->pause_backup_hudFlags = gGT->hudFlags;
	sdata->pause_state = 1;
}
