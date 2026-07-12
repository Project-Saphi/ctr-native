#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x800347d0-0x80034874.
// Dont get confused, packID is LOAD_GetAdvPackIndex(),
// which gives the pack of the hub you're NOT on, because the
// game does 3-GetAdvPackIndex to load the hub you ARE on
void LOAD_TalkingMask(int packID, int maskID)
{
	sdata->modelMaskHints3D = 0;

	// invalidate alternative-hub, because
	// the mask will load in that level's RAM
	sdata->gGT->levID_in_each_mempack[packID] = -1;

	// Swap to pack of hub you're NOT on,
	// wipe the pack to reload the new MASK
	MEMPACK_SwapPacks(packID);
	MEMPACK_ClearLowMem();

	// NOTE(claude): Ghidra 0x80034830 `sw v0,0x138(gp)` = 0x8008d0a4 =
	// load_inProgress, not PatchMem_Size (0x8008d094) — same fix as
	// LOAD_Hub_ReadFile.
	sdata->load_inProgress = 1;

	int offset = maskID * 4 + (packID - 1) * 2;

	// NOTE(aalhendi): Retail queues legacy VRAM type 3 with no final callback.
	// NOTE(claude): Ghidra 0x80034828/0x80034850 `lw a0,0x130(gp)` = 0x8008d09c
	// = ptrBigfileCdPos_2 — retail passes the real bigfile header, not NULL
	// (the NULL→ptrBigfile1 fallback in ReadFile_ex is a port shim).
	LOAD_AppendQueue(sdata->ptrBigfileCdPos_2, LT_VRAM, BI_UKAHEAD + offset, NULL, NULL);

	LOAD_AppendQueue(sdata->ptrBigfileCdPos_2, LT_GETADDR, BI_UKAHEAD + offset + 1, NULL, LOAD_Callback_MaskHints3D);
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80034874-0x800348e8.
void LOAD_LevelFile(int levelID)
{
	struct GameTracker *gGT = sdata->gGT;

	// why here?
	sdata->modelMaskHints3D = 0;

	gGT->hudFlags &= 0xfe;

	gGT->prevLEV = gGT->levelID;
	gGT->levelID = levelID;

	// disable all rendering except checkeredFlag
	gGT->renderFlags &= 0x1000;

	if (RaceFlag_IsFullyOffScreen() == 1)
	{
		RaceFlag_BeginTransition(1);
	}

	// start loading
	sdata->Loading.stage = LOAD_TEN_STAGES_0;
}
