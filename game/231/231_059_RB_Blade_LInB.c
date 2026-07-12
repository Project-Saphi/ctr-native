#include <common.h>

static char s_blade[] = "blade";

// NOTE(aalhendi): ASM-verified against NTSC-U 926 overlay 231 0x800b3978-0x800b39dc.
void RB_Blade_LInB(struct Instance *inst)
{
	// Four "blades" from two blimps, Hot Air Skyway
	// One "blade" is a group of three fins,

	struct Blade *bladeObj;

	// NOTE(claude): Ghidra 0x800b3988 `lw v0,0x6c(s0); bne v0,zero,end` — retail guards the
	// entire birth on inst->thread==0 (as RB_Baron_LInB and the sibling LInB handlers do). The
	// decomp dropped it; without it a repeat LInB call re-births a second blade thread and
	// overwrites inst->thread, orphaning (leaking) the first still-spinning blade. Match the binary.
	if (inst->thread != 0)
		return;

	struct Thread *t = PROC_BirthWithObject(
	    // creation flags
	    SIZE_RELATIVE_POOL_BUCKET(sizeof(struct Blade), NONE, SMALL, STATIC),

	    RB_Blade_ThTick, // behavior
	    s_blade,         // debug name
	    0                // thread relative
	);

	if (t == 0)
		return;
	inst->thread = t;
	t->inst = inst;

	bladeObj = ((struct Blade *)t->object);
	bladeObj->angle = 0;
}
