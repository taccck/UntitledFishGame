#include "FishPlayerAnim.h"
#include "Curves/CurveVector.h"

void UFishPlayerAnim::Idle()
{
	const float TotalTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = IdleCurves.AnimTime;
	const float Time = TotalTime - truncf(TotalTime / AnimTime) * AnimTime;
	HeadLocation = IdleCurves.HeadCurve->GetVectorValue(Time);
}

void UFishPlayerAnim::Walk()
{
}

void UFishPlayerAnim::Jump()
{
}

void UFishPlayerAnim::Fall()
{
}