#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80027838-0x80027b88.
void GhostReplay_Init1(void)
{
	int i;
	struct Thread *t;
	struct Instance *inst;
	struct Instance *wakeInst;
	struct Driver *ghostDriver;
	struct Model *model;
	struct Model *wake;
	int timeTrialFlags;

	struct GhostHeader *gh;
	struct GhostTape *tape;
	int charID;
	char *recordBuffer;

	struct GameTracker *gGT = sdata->gGT;

	// This has to run from MainInit_Drivers
	sdata->boolCanSaveGhost = 0;
	sdata->boolGhostsDrawing = 0;

	// only continue if you're in time trial, not main menu, and not cutscene
	if ((gGT->gameMode1 & 0x20022000) != 0x20000)
		return;

	// === Record Buffer ===

	// In the future, this can move to GhostTape_Start, when byte budget allows

	gh = MEMPACK_AllocMem(0x3e00 /*, "ghost record buffer"*/);
	recordBuffer = GHOSTHEADER_GETRECORDBUFFER(gh);
	sdata->GhostRecording.ptrGhost = gh;
	sdata->GhostRecording.ptrStartOffset = &recordBuffer[0];
	sdata->GhostRecording.ptrEndOffset = &recordBuffer[0x3DD4];

	// === Replay Buffer ===
	// 0: human ghost
	// 1: N Tropy / Oxide ghost

	// ALWAYS initialize ghost threads
	// even if gh == 0, or else the text
	// for "Ghost Too Big" will never play
	for (i = 0; i < 2; i++)
	{
		tape = MEMPACK_AllocMem(0x268 /*, "ghost tape"*/);
		sdata->ptrGhostTape[i] = tape;

		// first ghost pointer is a ghost loaded by player
		if (i == 0)
		{
			// assign the ghost you loaded
			gh = sdata->ptrGhostTapePlaying;
		}

		// second ghost pointer is n tropy or oxide
		else
		{
			// Bitwise ORs
			// 0|0|0 (0) - No Ghost Unlocked
			// 1|0|0 (1) - NTropy Ghost Open
			// 1|2|0 (3) - NTropy Ghost Beaten, Oxide Ghost Open
			// 1|2|4 (7) - NOxide Ghost Beaten
			timeTrialFlags = sdata->gameProgress.highScoreTracks[gGT->levelID].timeTrialFlags;

			void **pointers = ST1_GETPOINTERS(gGT->level1->ptrSpawnType1);

			if ((timeTrialFlags & 2) != 0)
			{
				gh = pointers[ST1_NOXIDE];
			}
			else
			{
				gh = pointers[ST1_NTROPY];
			}
		}

		recordBuffer = GHOSTHEADER_GETRECORDBUFFER(gh);

		tape->gh = gh;
		tape->gh_again = gh;
		tape->ptrStart = &recordBuffer[0];
		tape->constDEADC0ED = 0xDEADC0ED;
		tape->ptrEnd = &recordBuffer[gh->size];

		// if n tropy / oxide
		if (i == 1)
		{
			// guaranteed gh != 0, so dont nullptr check
			gGT->timeToBeatInTimeTrial_ForCurrentEvent = gh->timeElapsedInRace;
		}
	}

	for (i = 0; i < 2; i++)
	{
		t = PROC_BirthWithObject(

		    // creation flags
		    SIZE_RELATIVE_POOL_BUCKET(4, NONE, LARGE, GHOST),

		    GhostReplay_ThTick, sdata->s_ghost, 0);

		t->modelIndex = DYNAMIC_GHOST; // ghost
		t->flags |= 0x1000;            // ignore collisions

		// ghost drivers are 0x638 bytes large
		ghostDriver = t->object;
		memset(ghostDriver, 0, 0x638);
		ghostDriver->ghostID = i;
		ghostDriver->driverID = i + 1;
		ghostDriver->ghostBoolInit = 0;
		ghostDriver->ghostTape = sdata->ptrGhostTape[i];

		// characterID and model
		charID = data.characterIDs[i + 1];
		model = VehBirth_GetModelByName(data.MetaDataCharacters[charID].name_Debug);

		inst = INSTANCE_Birth3D(model, model->name, t);
		t->inst = inst;

		// Ptr Model "Wake"
		wake = gGT->modelPtr[STATIC_WAKE];

		// if "Wake" model exists
		if (wake)
		{
			wakeInst = INSTANCE_Birth3D(wake, wake->name, 0);
			ghostDriver->wakeInst = wakeInst;

			if (wakeInst != 0)
			{
				// make invisible, set to anim 1
				wakeInst->flags |= 0x90;
			}
		}

		inst->unk51 = 0xc;
		inst->flags |= 0x4000000;
		ghostDriver->instSelf = inst;
		VehBirth_TireSprites(t);
		VehBirth_SetConsts(ghostDriver);

		ghostDriver->actionsFlagSet |= 0x100000; // AI driver

		// pointer to TrTire, for transparent tires
		ghostDriver->wheelSprites = ICONGROUP_GETICONS(gGT->iconGroup[0xc]);

		// NOTE(aalhendi): GhostReplay_Init2 owns retail activation/tick.
	}

	return;
}
