#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x800248bc-0x80024974.
void ElimBG_ToggleAllInstances(struct GameTracker *gGT, int boolGameIsPaused)
{
	struct Level *lev;
	struct Instance *inst;
	struct InstDef *ptrInstDefs;

	lev = gGT->level1;

	// Loop through all instances in level
	for (ptrInstDefs = &lev->ptrInstDefs[0]; ptrInstDefs < &lev->ptrInstDefs[lev->numInstances]; ptrInstDefs++)
	{
		inst = ptrInstDefs->ptrInstance;

		if (inst != 0)
			ElimBG_ToggleInstance(inst, boolGameIsPaused);
	}

	// Loop through all instances in Instance Pool
	for (inst = (struct Instance *)gGT->JitPools.instance.taken.first; inst != 0; inst = inst->next)
	{
		ElimBG_ToggleInstance(inst, boolGameIsPaused);
	}
}
