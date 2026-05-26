#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80045254-0x800453e8.
void RECTMENU_DrawRwdBlueRect(RECT *rect, char *metas, u_long *ot, struct PrimMem *primMem)
{
	s16 pos[4];
	int gradient[2];
	int colors[4];

	pos[0] = rect->x;
	pos[2] = rect->w;

	for (int i = 0; (u8)metas[i * 4 + 3] != 0x64; i++)
	{
		u8 *meta = (u8 *)&metas[i * 4];
		gradient[0] = *(int *)&meta[0];
		gradient[1] = *(int *)&meta[4];
		colors[0] = gradient[0];
		colors[1] = gradient[0];
		colors[2] = gradient[1];
		colors[3] = gradient[1];
		pos[1] = rect->y + (s16)(meta[3] * rect->h / 100);
		pos[3] = rect->y + (s16)(meta[7] * rect->h / 100) - pos[1] + 1;

		RECTMENU_DrawRwdBlueRect_Subset(pos, colors, ot, primMem);
	}
}
