#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80047230-0x800472d0.
void RefreshCard_GameProgressAndOptions(void)
{
	struct MemcardProfile *memcard;

	*(s16 *)&sdata->unk8008d95c = 1;
	*(s16 *)&sdata->unk_memcardRelated_8008d928[0] = 1;
	*(s16 *)&sdata->advProfileIndex = -1;

	memcard = (struct MemcardProfile *)sdata->ptrToMemcardBuffer1;

	GAMEPROG_SyncGameAndCard(&memcard->gameProgress, &sdata->gameProgress);
	memcpy(&sdata->gameProgress, &memcard->gameProgress, sizeof(struct GameProgress) + sizeof(struct GameOptions));
	RaceConfig_LoadGameOptions();
}
