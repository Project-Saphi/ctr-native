#include <common.h>

#if defined(CTR_NATIVE)
#include <platform/native_audio.h>
#endif

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x8001d06c-0x8001d094.
void CDSYS_XAPauseAtEnd()
{
	if (sdata->boolUseDisc == 0)
	{
#if defined(CTR_NATIVE)
		int xaPlaying = NativeAudio_IsXAPlaying();

		if (xaPlaying != 0)
		{
			int prevOffset = sdata->XA_CurrOffset;
			int currOffset = NativeAudio_GetXACurrOffset();
			int firstOffset = -1;

			if (currOffset < prevOffset)
				prevOffset = currOffset;

			if (sdata->XA_MaxSampleNumSaved == 0)
			{
				firstOffset = currOffset;
			}
			else if (currOffset > prevOffset)
			{
				firstOffset = currOffset - 2;
				if (firstOffset < prevOffset + 1)
					firstOffset = prevOffset + 1;
				if (firstOffset < 0)
					firstOffset = 0;
			}

			for (int offset = firstOffset; offset >= 0 && offset <= currOffset; offset++)
				CDSYS_SpuGetMaxSampleAtOffset(offset);

			sdata->XA_CurrOffset = currOffset;
		}

		if (sdata->XA_State == 4)
		{
			sdata->XA_VolumeBitshift -= sdata->XA_VolumeDeduct;
			if (sdata->XA_VolumeBitshift <= 0)
			{
				sdata->XA_VolumeBitshift = 0;
				NativeAudio_StopXA();
				sdata->XA_boolFinished = 0;
				sdata->XA_State = 0;
				sdata->XA_MaxSampleVal = 0;
				sdata->XA_MaxSampleValInArr = 0;
				sdata->XA_PauseFrame = sdata->gGT->frameTimer_MainFrame_ResetDB;
			}
			else
			{
				NativeAudio_SetXAVolume((s16)sdata->XA_VolumeBitshift, (s16)sdata->XA_VolumeBitshift);
				sdata->XA_CurrOffset = NativeAudio_GetXACurrOffset();
			}
		}
		else if (xaPlaying == 0)
		{
			if (sdata->XA_State != 0)
				sdata->XA_PauseFrame = sdata->gGT->frameTimer_MainFrame_ResetDB;
			sdata->XA_State = 0;
			sdata->XA_MaxSampleVal = 0;
			sdata->XA_MaxSampleValInArr = 0;
		}
#endif
		return;
	}

	// wait until IRQ says XA is finished
	if (sdata->XA_boolFinished == 0)
		return;

	CDSYS_XAPauseForce();
}
