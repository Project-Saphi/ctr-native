#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80047fb8-0x80047fd8.
int SelectProfile_UI_ConvertX(int param_1, int param_2)
{
	param_2 = (param_1 - 0x100) * param_2;
	if (param_2 < 0)
	{
		param_2 = param_2 + 0xff;
	}
	return param_2 >> 8;
}
