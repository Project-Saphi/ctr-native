#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80043d24-0x80043e34.
void RaceConfig_SaveGameOptions(void)
{
	int i;
	s16 *volumes;

	volumes = &sdata->gameOptions.volFx;

	for (i = 0; i < 3; i++)
		volumes[i] = howl_VolumeGet(i) & 0xff;

	memcpy(&sdata->gameOptions.rwd[0], &data.rwd[0], sizeof(data.rwd));
	sdata->gameOptions.gameMode1_0xf00 = sdata->gGT->gameMode1 & 0xf00;
	*(s16 *)&sdata->gameOptions.audioMode = howl_ModeGet() != 0;
}
