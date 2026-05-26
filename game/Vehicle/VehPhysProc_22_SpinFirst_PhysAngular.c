#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80063dc8-0x80063eac.
void VehPhysProc_SpinFirst_PhysAngular(struct Thread *t, struct Driver *d)
{
	int elapsedTimeMS = sdata->gGT->elapsedTimeMS;

	d->numFramesSpentSteering = 10000;

	d->rotationSpinRate -= d->rotationSpinRate >> 3;
	d->unk3D4[0] -= d->unk3D4[0] >> 3;

	d->turnAngleCurr += d->KartStates.Spinning.driftSpinRate;
	d->turnAngleCurr += 0x800U;
	d->turnAngleCurr &= 0xfff;
	d->turnAngleCurr -= 0x800;

	d->ampTurnState = d->rotationSpinRate;

	d->angle += (s16)((d->rotationSpinRate * elapsedTimeMS) >> 0xd);
	d->angle &= 0xfff;

	d->rotCurr.y = d->unk3D4[0] + d->angle + d->turnAngleCurr;

	d->rotCurr.w = VehCalc_InterpBySpeed(d->rotCurr.w, (elapsedTimeMS << 5) >> 5, 0);

	VehPhysForce_RotAxisAngle(&d->matrixMovingDir, &d->AxisAngle1_normalVec.x, d->angle);
}
