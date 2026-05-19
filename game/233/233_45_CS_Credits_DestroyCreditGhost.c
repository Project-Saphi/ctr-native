#include <common.h>

void CS_Credits_DestroyCreditGhost(void)
{
	for (int i = 0; i < 5; i++)
	{
		DECOMP_INSTANCE_Death(creditsBSS.creditsObj.creditGhostInst[i]);
	}

	DECOMP_MEMPACK_ClearHighMem();
}
