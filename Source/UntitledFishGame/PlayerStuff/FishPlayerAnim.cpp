#include "FishPlayerAnim.h"

#include "FishPlayer.h"
#include "Curves/CurveVector.h"

UFishPlayerAnim::UFishPlayerAnim()
{
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<AFishPlayer>(GetOwner());
}

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

	PlantedRight = true;
	PlantedLeft = true;
	FootPlanting();
}

void UFishPlayerAnim::Walk()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = WalkCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	
	HeadLocation = WalkCurves->HeadCurve->GetVectorValue(Time);
	HandRLocation = WalkCurves->HandRCurve->GetVectorValue(Time);
	HandLLocation = WalkCurves->HandLCurve->GetVectorValue(Time);
	const FVector NewFootRLocation = WalkCurves->FootRCurve->GetVectorValue(Time);
	const FVector NewFootLLocation = WalkCurves->FootLCurve->GetVectorValue(Time);
	
	PlantedRight = Time > AnimTime / 2.f; //swap planted on contact pos
	PlantedLeft = !PlantedRight;
	const FVector NewPlantedFootLocation = PlantedRight? NewFootRLocation : NewFootLLocation;
	const FVector OldPlantedFootLocation = PlantedRight? FootRLocation : FootLLocation;
	MoveDistance = abs(NewPlantedFootLocation.Y - OldPlantedFootLocation.Y);

	FootRLocation.Z = FMath::Lerp(FootRLocation.Z, NewFootRLocation.Z,.1f);
	FootLLocation.Z = FMath::Lerp(FootLLocation.Z, NewFootLLocation.Z,.1f);
	FootRLocation.X = NewFootRLocation.X;
	FootRLocation.Y = NewFootRLocation.Y;
	FootLLocation.X = NewFootLLocation.X;
	FootLLocation.Y = NewFootLLocation.Y;
	FootPlanting();
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
	
	PlantedRight = false;
	PlantedLeft = false;
}

void UFishPlayerAnim::FootPlanting()
{
	const FTransform MeshTransform = Owner->Mesh->GetComponentTransform();
	if(PlantedRight)
	{
		FootRLocation.Z = 0;
		const FVector WorldLocation = MeshTransform.TransformPosition(FootRLocation);
		FVector StartLocation = WorldLocation;
		StartLocation.Z += FootSnappingHeight;
		FVector EndLocation = WorldLocation;
		EndLocation.Z -= FootSnappingHeight;
		FHitResult GroundHit;
		GetWorld()->LineTraceSingleByChannel(GroundHit, StartLocation, EndLocation, ECC_Visibility);
		if(GroundHit.bBlockingHit)
			FootRLocation.Z = GroundHit.ImpactPoint.Z - MeshTransform.GetLocation().Z;
		UE_LOG(LogTemp, Log, TEXT("Right: %f"), FootRLocation.Z);
	}

	if (PlantedLeft)
	{
		FootLLocation.Z = 0;
		const FVector WorldLocation = MeshTransform.TransformPosition(FootLLocation);
		FVector StartLocation = WorldLocation;
		StartLocation.Z += FootSnappingHeight;
		FVector EndLocation = WorldLocation;
		EndLocation.Z -= FootSnappingHeight;
		FHitResult GroundHit;
		GetWorld()->LineTraceSingleByChannel(GroundHit, StartLocation, EndLocation, ECC_Visibility);
		if(GroundHit.bBlockingHit)
			FootLLocation.Z = GroundHit.ImpactPoint.Z - MeshTransform.GetLocation().Z;
		UE_LOG(LogTemp, Log, TEXT("Left: %f"), FootLLocation.Z);
	}
}