#include <common.h>

static struct InstDrawPerPlayer *RB_Burst_DrawAll_GetIDPP(struct Instance *inst, int playerIndex)
{
	return (struct InstDrawPerPlayer *)((char *)inst + sizeof(struct Instance) + (playerIndex * sizeof(struct InstDrawPerPlayer)));
}

static void RB_Burst_DrawAll_SetPushBuffer(struct Instance *inst, int playerIndex, struct PushBuffer *pb)
{
	if (inst != NULL)
		RB_Burst_DrawAll_GetIDPP(inst, playerIndex)->pushBuffer = pb;
}

static struct Instance *RB_Burst_DrawAll_GetSlot(u32 *burst, int index)
{
	// NOTE(aalhendi): burst thread object is retail-width instance slots.
	return (struct Instance *)(uintptr_t)burst[index];
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x800b25b8-0x800b28c0.
void RB_Burst_DrawAll(struct GameTracker *gGT)
{
	struct Thread *selectedThread[4];
	int selectedFrame[4];
	int playerIndex;
	struct Thread *thread;

	for (playerIndex = 0; playerIndex < gGT->numPlyrCurrGame; playerIndex++)
	{
		struct PushBuffer *pb = &gGT->pushBuffer[playerIndex];

		selectedFrame[playerIndex] = 0x10000;
		selectedThread[playerIndex] = NULL;

		SetRotMatrix(&pb->matrix_ViewProj);
		SetTransMatrix(&pb->matrix_ViewProj);

		for (thread = gGT->threadBuckets[BURST].thread; thread != NULL; thread = thread->siblingThread)
		{
			u32 *burst = thread->object;
			struct Instance *burstInst = RB_Burst_DrawAll_GetSlot(burst, 1);
			SVECTOR pos;
			VECTOR transformed;
			int absX;
			int absY;
			int absZ;

#ifdef CTR_NATIVE
			// NOTE(aalhendi): Retail can survive the one-frame null low-RAM read.
			if (burstInst == NULL)
				continue;
#endif

			pos.vx = burstInst->matrix.t[0];
			pos.vy = burstInst->matrix.t[1];
			pos.vz = burstInst->matrix.t[2];

			gte_ldv0(&pos);
			// NOTE(claude): Ghidra 0x800b2694 `cop2 0x0480012` = gte_rt = MVMVA(sf=1, mx=RT,
			// v=V0, cv=TR, lm=0) → (TR·0x1000 + R·V0) >> 12, i.e. the full camera-space transform
			// (rotate + translate + >>12; SetRot/SetTransMatrix above load both from the view-proj
			// matrix). The prior gte_mvmva(0,0,0,3,0) = cop2 0x0406012 = MVMVA(sf=0, cv=none) is
			// rotate-ONLY — it drops the camera translation AND the >>12, so absX/Y/Z came out
			// un-translated and ~0x1000× too large, making the |x|<0x100 && |y|<0x100 cull
			// essentially always fail: the explosion screen fade-flash never fired and pass 2
			// drew every burst on every split-screen. gte_rt matches the binary.
			gte_rt();
			gte_stlvnl(&transformed);

			absX = transformed.vx;
			if (absX < 0)
				absX = -absX;

			absY = transformed.vy;
			if (absY < 0)
				absY = -absY;

			absZ = transformed.vz;
			if (absZ < 0)
				absZ = -absZ;

			if ((absX < 0x100) && (absY < 0x100) && (absZ < (pb->distanceToScreen_PREV << 1)))
			{
				if (burstInst->animFrame < selectedFrame[playerIndex])
				{
					selectedFrame[playerIndex] = burstInst->animFrame;
					selectedThread[playerIndex] = thread;
				}

				if (burstInst->animFrame == 1)
				{
					pb->fadeFromBlack_desiredResult = 0x1000;
					pb->fade_step = -0x88;
					pb->fadeFromBlack_currentValue = 0x1fff - ((absX + absY) << 3);
				}
			}
		}
	}

	for (playerIndex = 0; playerIndex < gGT->numPlyrCurrGame; playerIndex++)
	{
		struct PushBuffer *pb = &gGT->pushBuffer[playerIndex];

		for (thread = gGT->threadBuckets[BURST].thread; thread != NULL; thread = thread->siblingThread)
		{
			u32 *burst = thread->object;
			struct PushBuffer *targetPB = pb;

			if ((selectedThread[playerIndex] != NULL) && (selectedThread[playerIndex] != thread))
				targetPB = NULL;

			RB_Burst_DrawAll_SetPushBuffer(RB_Burst_DrawAll_GetSlot(burst, 1), playerIndex, targetPB);
			RB_Burst_DrawAll_SetPushBuffer(RB_Burst_DrawAll_GetSlot(burst, 2), playerIndex, targetPB);
			RB_Burst_DrawAll_SetPushBuffer(RB_Burst_DrawAll_GetSlot(burst, 0), playerIndex, targetPB);
		}
	}
}
