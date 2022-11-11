#include "FishPlayerAnim.h"

#include "FishPlayer.h"
#include "Curves/CurveVector.h"

UFishPlayerAnim::UFishPlayerAnim()
{
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<AFishPlayer>(GetOwner());
}

void UFishPlayerAnim::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Animate(DeltaTime);
}

void UFishPlayerAnim::ToIdleState()
{
	CurrentCurves = IdleCurves;
	AnimState = "Idle";
	
	PlantedRight = true;
	PlantedLeft = true;
}

void UFishPlayerAnim::ToWalkState()
{
	CurrentCurves = WalkCurves;
	AnimState = "Walk";
}

void UFishPlayerAnim::ToJumpState()
{
	CurrentCurves = JumpCurves;
	AnimState = "Jump";
}

void UFishPlayerAnim::ToFallState()
{
	CurrentCurves = FallCurves;
	AnimState = "Fall";
	
	PlantedRight = false;
	PlantedLeft = false;
}


void UFishPlayerAnim::UpdatePose()
{
	//move to next frame in the animation
	
	const float WorldTime = GetWorld()->GetTimeSeconds();
	const float AnimTime = CurrentCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
    	
	NextPose.HeadLocation = CurrentCurves->HeadCurve->GetVectorValue(Time);
	NextPose.HandRLocation = CurrentCurves->HandRCurve->GetVectorValue(Time);
	NextPose.HandLLocation = CurrentCurves->HandLCurve->GetVectorValue(Time);
	NextPose.FootRLocation = CurrentCurves->FootRCurve->GetVectorValue(Time);
	NextPose.FootLLocation = CurrentCurves->FootLCurve->GetVectorValue(Time);

	if(AnimState == "Walk")
	{
		PlantedRight = Time > AnimTime / 2.f; //swap planted on contact pos
		PlantedLeft = !PlantedRight;
	}
}

void UFishPlayerAnim::FootPlanting()
{
	//makes sure feet are on the ground when they should be
	
	const FTransform MeshTransform = Owner->Mesh->GetComponentTransform();
	if(PlantedRight)
	{
		NextPose.FootRLocation.Z = 0;
		const FVector WorldLocation = MeshTransform.TransformPosition(NextPose.FootRLocation);
		FVector StartLocation = WorldLocation;
		StartLocation.Z += FootSnappingHeight;
		FVector EndLocation = WorldLocation;
		EndLocation.Z -= FootSnappingHeight;
		FHitResult GroundHit;
		GetWorld()->LineTraceSingleByChannel(GroundHit, StartLocation, EndLocation, ECC_Visibility);
		if(GroundHit.bBlockingHit)
			NextPose.FootRLocation.Z = GroundHit.ImpactPoint.Z - MeshTransform.GetLocation().Z;
	}

	if (PlantedLeft)
	{
		NextPose.FootLLocation.Z = 0;
		const FVector WorldLocation = MeshTransform.TransformPosition(NextPose.FootLLocation);
		FVector StartLocation = WorldLocation;
		StartLocation.Z += FootSnappingHeight;
		FVector EndLocation = WorldLocation;
		EndLocation.Z -= FootSnappingHeight;
		FHitResult GroundHit;
		GetWorld()->LineTraceSingleByChannel(GroundHit, StartLocation, EndLocation, ECC_Visibility);
		if(GroundHit.bBlockingHit)
			NextPose.FootLLocation.Z = GroundHit.ImpactPoint.Z - MeshTransform.GetLocation().Z;
	}
}

void UFishPlayerAnim::ApplyMaxSpeed(const float DeltaTime)
{
	const float MaxDistance = MaxAnimSpeed * DeltaTime;
	auto Apply = [&](const FVector New, const FVector Old)->FVector
	{
		const float CurrDistance = FVector::Distance(Old, New);
		if (CurrDistance > MaxDistance)
		{
			return Old + (New - Old).GetSafeNormal() * MaxDistance;
		}
		return New;
	};
	
	NextPose.HeadLocation = Apply(NextPose.HeadLocation, CurrentPose.HeadLocation);
	NextPose.HandRLocation = Apply(NextPose.HandRLocation, CurrentPose.HandRLocation);
	NextPose.HandLLocation = Apply(NextPose.HandLLocation, CurrentPose.HandLLocation);
	NextPose.FootRLocation = Apply(NextPose.FootRLocation, CurrentPose.FootRLocation);
	NextPose.FootLLocation = Apply(NextPose.FootLLocation, CurrentPose.FootLLocation);
}

void UFishPlayerAnim::Animate(const float DeltaTime)
{
	if(!CurrentCurves.IsValid()) return;
	
	UpdatePose();
	FootPlanting();
	ApplyMaxSpeed(DeltaTime);

	if(AnimState == "Walk")
	{
		const FVector NewPlantedFootLocation = PlantedRight? NextPose.FootRLocation : NextPose.FootLLocation;
		const FVector OldPlantedFootLocation = PlantedRight? CurrentPose.FootRLocation : CurrentPose.FootLLocation;
		MoveDistance = abs(NewPlantedFootLocation.Y - OldPlantedFootLocation.Y);
	}
	CurrentPose = NextPose;
}
