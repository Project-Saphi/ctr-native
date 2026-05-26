#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80043c10-0x80043d24.
void RaceConfig_LoadGameOptions(void)
{
	int i;
	s16 *volumes;

	if (sdata->boolHasLoadedOptions != 0)
		return;

	sdata->boolHasLoadedOptions = 1;
	volumes = &sdata->gameOptions.volFx;

	for (i = 0; i < 3; i++)
	{
		howl_VolumeSet(i, (u8)volumes[i]);
		memcpy(&data.rwd[0], &sdata->gameOptions.rwd[0], sizeof(data.rwd));
	}

	sdata->gGT->gameMode1 |= sdata->gameOptions.gameMode1_0xf00 & 0xf00;
	howl_ModeSet((u8)sdata->gameOptions.audioMode & 1);
}
