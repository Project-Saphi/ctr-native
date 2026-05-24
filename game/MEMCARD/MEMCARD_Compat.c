#include <common.h>

void MEMCARD_SetIcon(int iconID)
{
	DECOMP_MEMCARD_SetIcon(iconID);
}

u32 MEMCARD_CRC16(u32 crc, int nextByte)
{
	return DECOMP_MEMCARD_CRC16(crc, nextByte);
}

int MEMCARD_ChecksumSave(u8 *saveBytes, int len)
{
	return DECOMP_MEMCARD_ChecksumSave(saveBytes, len);
}

u32 MEMCARD_ChecksumLoad(u8 *saveBytes, int len)
{
	return DECOMP_MEMCARD_ChecksumLoad(saveBytes, len);
}

char *MEMCARD_StringInit(int slotIndex, char *dstString)
{
	return DECOMP_MEMCARD_StringInit(slotIndex, dstString);
}

void MEMCARD_StringSet(char *dstString, int slotIdx, char *srcString)
{
	DECOMP_MEMCARD_StringSet(dstString, slotIdx, srcString);
}

void MEMCARD_InitCard(void)
{
	DECOMP_MEMCARD_InitCard();
}

void MEMCARD_CloseCard(void)
{
	DECOMP_MEMCARD_CloseCard();
}

u8 MEMCARD_GetNextSwEvent(void)
{
	return DECOMP_MEMCARD_GetNextSwEvent();
}

u8 MEMCARD_GetNextHwEvent(void)
{
	return DECOMP_MEMCARD_GetNextHwEvent();
}

void MEMCARD_SkipEvents(void)
{
	DECOMP_MEMCARD_SkipEvents();
}

int MEMCARD_NewTask(int slotIdx, char *name, u8 *ptrMemcard, int memcardFileSize, int flags)
{
	return DECOMP_MEMCARD_NewTask(slotIdx, name, ptrMemcard, memcardFileSize, flags);
}

void MEMCARD_CloseFile(void)
{
	DECOMP_MEMCARD_CloseFile();
}

int MEMCARD_ReadFile(int start_offset, int size)
{
	return DECOMP_MEMCARD_ReadFile(start_offset, size);
}

u8 MEMCARD_WriteFile(int start_offset, const u8 *data, int size)
{
	return DECOMP_MEMCARD_WriteFile(start_offset, data, size);
}

void MEMCARD_GetFreeBytes(int slotIdx)
{
	(void)slotIdx;
	sdata->memoryCard_SizeRemaining = 0x1e000;
}

u8 MEMCARD_GetInfo(int slotIdx)
{
	(void)slotIdx;
	return MC_RETURN_IOE;
}

u8 MEMCARD_Format(int slotIdx)
{
	(void)slotIdx;
	return MC_RETURN_IOE;
}

int MEMCARD_IsFile(int slotIdx, char *save_name)
{
	(void)slotIdx;
	(void)save_name;
	return MC_RETURN_NODATA;
}

char *MEMCARD_FindFirstGhost(int slotIdx, char *srcString)
{
	(void)slotIdx;
	(void)srcString;
	return NULL;
}

char *MEMCARD_FindNextGhost(void)
{
	return NULL;
}

u8 MEMCARD_EraseFile(int slotIdx, char *srcString)
{
	(void)slotIdx;
	(void)srcString;
	return MC_RETURN_IOE;
}
