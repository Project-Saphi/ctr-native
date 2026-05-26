#include <common.h>

#if defined(CTR_NATIVE)
// 32mb RAM, so that PrimMem can fill the entirety
// of 24-bit address space (16mb max) and have enough
// RAM left over for the rest of the game (16mb more)
char memory[32 * 1024 * 1024];
#endif

void CS_EndOfFile();
void RB_EndOfFile();
void AH_EndOfFile();
void MM_EndOfFile();
void OVR_Region3();

// NOTE(aalhendi): ASM-verified NTSC-U 926 PS1 path 0x8003e740-0x8003e80c; CTR_NATIVE uses host RAM.
void MEMPACK_Init(int ramSize)
{
	struct Mempack *ptrMempack;
	u32 startPtr;
	u32 maxOverlayEnd;
	int packSize;

#if defined(CTR_NATIVE)

	// must be a 24-bit address
	// Visual Studio -> Properties -> Linker -> Advanced ->
	// Base Address, Randomized Base Address, Fixed Base Address
	startPtr = (u32)&memory[0];

	int boolValid = startPtr < 0x01000000;

	printf("[CTR] Where does memory starts? (%s) %08x\n", boolValid ? "GOOD" : "BAD", startPtr);

	packSize = 32 * 1024 * 1024;
	memset((void *)startPtr, 0, packSize);

	MEMPACK_NewPack((void *)startPtr, packSize);

#else

	maxOverlayEnd = (u32)AH_EndOfFile;
	if (maxOverlayEnd < (u32)RB_EndOfFile)
		maxOverlayEnd = (u32)RB_EndOfFile;
	if (maxOverlayEnd < (u32)MM_EndOfFile)
		maxOverlayEnd = (u32)MM_EndOfFile;
	if (maxOverlayEnd < (u32)CS_EndOfFile)
		maxOverlayEnd = (u32)CS_EndOfFile;

	startPtr = (u32)OVR_Region3 + (((maxOverlayEnd - (u32)OVR_Region3) + 0x7ff) & ~0x7ff);
	packSize = ramSize - (int)(startPtr & 0xffffff) - 0x800;

	ptrMempack = sdata->PtrMempack;
	ptrMempack->start = (void *)startPtr;
	ptrMempack->endOfAllocator = (void *)(startPtr + packSize);
	ptrMempack->lastFreeByte = (void *)(startPtr + packSize);
	ptrMempack->packSize = packSize;
	ptrMempack->numBookmarks = 0;
	ptrMempack->endOfMemory = (void *)0x80200000;
	ptrMempack->firstFreeByte = (void *)startPtr;
#endif
}
