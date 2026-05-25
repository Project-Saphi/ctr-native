#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80047fd8-0x80047ff8.
int SelectProfile_UI_ConvertY(int param_1, int param_2)
{
	param_2 = (param_1 + -0x6c) * param_2;
	if (param_2 < 0)
	{
		param_2 = param_2 + 0xff;
	}
	return param_2 >> 8;
}
