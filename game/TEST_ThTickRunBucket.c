#include <common.h>

void TEST_ThTickRunBucket(struct Thread *t)
{
	for (/**/; t != 0; t = t->siblingThread)
	{
		if ((t->flags & 0x800) != 0)
			continue;

		if (t->funcThTick == 0)
			continue;

		if (t->cooldownFrameCount != 0)
		{
			t->cooldownFrameCount--;
			continue;
		}

		t->funcThTick(t);
	}
}

// NOTE(aalhendi): Retail ThTick_FastRET unwinds the R3000A scratchpad thread
// runner. Native thread ticks are normal C calls, so the portable equivalent is
// just the caller returning normally after this no-op.
void ThTick_FastRET(struct Thread *t)
{
	(void)t;
}
