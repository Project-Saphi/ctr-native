#include <common.h>

void MM_Video_KickCD(CdlLOC *location)
{
	// NOTE(aalhendi): ASM-verified NTSC-U 926 0x800b5b7c-0x800b5c8c.
	int iVar1;
	int CdlMode;
	u8 mode[4];

	// NOTE(claude): Ghidra 0x800b5b9c-0x800b5ba0 `addiu v0,0x6824; beq v1,v0` — the
	// sentinel is &cdLocation1 (0x800b6824), the address StartStream seeds and points
	// ptrCdLoc at. The prior &cdLocation2 (0x800b6828) is a distinct offset, so this
	// guard would wrongly fire on the restart KickCD(&field@0x682c) call while
	// ptrCdLoc still == &cdLocation1, resetting the CD state machine (field12_0x20=0)
	// instead of continuing it. Compare against cdLocation1 to match the binary.
	if ((location != NULL) && (V230.ptrCdLoc != &V230.cdLocation1))
	{
		V230.field12_0x20 = 0;
		V230.ptrCdLoc = location;
	}

	switch (V230.field12_0x20)
	{
	case 0:
		// 2 = CdlSetloc
		iVar1 = CdControl(2, V230.ptrCdLoc, 0);
		if (iVar1 == 0)
			return;

		V230.field12_0x20 = 1;

		// do NOT break,
		// original code never quit here

	case 1:
		// CdlModeSpeed
		mode[0] = 0x80;

		// 0xe = CdlSetmode
		iVar1 = CdControl(0xe, mode, 0);
		if (iVar1 == 0)
			return;

		V230.field12_0x20 = 2;
		break;

	case 2:
		V230.field12_0x20 = 3;
		break;

	case 3:
		// CdlModeStream2|CdlModeSpeed
		CdlMode = 0x1a0;

		// scrapbook
		// if video contains audio
		if ((V230.flags & 2) != 0)
		{
			// CdlModeStream2|CdlModeSpeed|CdlModeRT
			CdlMode = 0x1e0;
		}

		V230.field12_0x20 = 0;

		iVar1 = CdRead2(CdlMode);
		if (iVar1 == 0)
			return;

		V230.ptrCdLoc = 0;
		break;
	}
}
