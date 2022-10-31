#include "FishPlayerAnim.h"
#include "Curves/CurveVector.h"

void UFishPlayerAnim::Idle()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = IdleCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	HeadLocation = IdleCurves->HeadCurve->GetVectorValue(Time);
	HandRLocation = IdleCurves->HandRCurve->GetVectorValue(Time);
	HandLLocation = IdleCurves->HandLCurve->GetVectorValue(Time);
	FootRLocation = IdleCurves->FootRCurve->GetVectorValue(Time);
	FootLLocation = IdleCurves->FootLCurve->GetVectorValue(Time);
}

void UFishPlayerAnim::Walk()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = WalkCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	HeadLocation = WalkCurves->HeadCurve->GetVectorValue(Time);
	HandRLocation = WalkCurves->HandRCurve->GetVectorValue(Time);
	HandLLocation = WalkCurves->HandLCurve->GetVectorValue(Time);
	FootRLocation = WalkCurves->FootRCurve->GetVectorValue(Time);
	FootLLocation = WalkCurves->FootLCurve->GetVectorValue(Time);
}

void UFishPlayerAnim::Jump()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = IdleCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	HeadLocation = JumpCurves->HeadCurve->GetVectorValue(Time);
	HandRLocation = JumpCurves->HandRCurve->GetVectorValue(Time);
	HandLLocation = JumpCurves->HandLCurve->GetVectorValue(Time);
}

void UFishPlayerAnim::Fall()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = FallCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	HeadLocation = FallCurves->HeadCurve->GetVectorValue(Time);
	HandRLocation = FallCurves->HandRCurve->GetVectorValue(Time);
	HandLLocation = FallCurves->HandLCurve->GetVectorValue(Time);
	FootRLocation = FallCurves->FootRCurve->GetVectorValue(Time);
	FootLLocation = FallCurves->FootLCurve->GetVectorValue(Time);
}