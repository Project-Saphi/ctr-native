#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x800683f4-0x800685b0.
void VehStuckProc_Warp_MoveDustPuff(s16 *points, int span, int radius, s16 *jitterScale)
{
	int radiusHalf = radius >> 1;

	int jitterX = ((MixRNG_Scramble() & 0xfff) * radius) >> 0xc;
	if (jitterX < radiusHalf)
		jitterX -= radius;

	int jitterY = ((MixRNG_Scramble() & 0xfff) * radius) >> 0xc;
	if (jitterY < radiusHalf)
		jitterY -= radius;

	int jitterZ = ((MixRNG_Scramble() & 0xfff) * radius) >> 0xc;
	if (jitterZ < radiusHalf)
		jitterZ -= radius;

	s16 *end = points + span * 4;
	int halfSpan = span >> 1;
	s16 *mid = points + halfSpan * 4;

	mid[0] = (s16)((points[0] + end[0]) >> 1) + (s16)((jitterScale[0] * jitterX) >> 0xc);
	mid[1] = (s16)((points[1] + end[1]) >> 1) + (s16)((jitterScale[1] * jitterY) >> 0xc);
	mid[2] = (s16)((points[2] + end[2]) >> 1) + (s16)((jitterScale[2] * jitterZ) >> 0xc);

	if (span > 2)
	{
		int nextRadius = (radius * 0xc00) >> 0xc;
		VehStuckProc_Warp_MoveDustPuff(points, halfSpan, nextRadius, jitterScale);
		VehStuckProc_Warp_MoveDustPuff(mid, halfSpan, nextRadius, jitterScale);
	}
}
