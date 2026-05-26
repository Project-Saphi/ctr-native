#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80063a44-0x80063af8.
void VehPhysProc_SlamWall_PhysAngular(struct Thread *t, struct Driver *d)
{
	int elapsedTimeMS = sdata->gGT->elapsedTimeMS;

	d->angle += (s16)((d->ampTurnState * elapsedTimeMS) >> 0xd);
	d->angle &= 0xfff;

	d->rotCurr.y = d->unk3D4[0] + d->angle + d->turnAngleCurr;

	d->rotCurr.w = VehCalc_InterpBySpeed(d->rotCurr.w, (elapsedTimeMS << 5) >> 5, 0);

	d->turnAngleCurr = VehCalc_InterpBySpeed(d->turnAngleCurr, (elapsedTimeMS << 7) >> 5, 0);

	VehPhysForce_RotAxisAngle(&d->matrixMovingDir, &d->AxisAngle1_normalVec.x, d->angle);
}
