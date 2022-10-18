#include "FishPlayerAnim.h"
#include "Curves/CurveVector.h"

void UFishPlayerAnim::Idle()
{
	const float TotalTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = IdleCurves.AnimTime;
	const float Time = TotalTime - static_cast<int>(TotalTime / AnimTime) * AnimTime;
	HeadLocation = IdleCurves.HeadCurve->GetVectorValue(Time);
	HandRLocation = IdleCurves.HandRCurve->GetVectorValue(Time);
	HandLLocation = IdleCurves.HandLCurve->GetVectorValue(Time);
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