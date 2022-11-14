#include "FishPlayerAnimInstance.h"

#include "FishPlayer.h"
#include "FishPlayerMovement.h"
#include "Curves/CurveVector.h"

UFishPlayerAnimInstance::UFishPlayerAnimInstance()
{
	FootColShape = FCollisionShape::MakeSphere(6.f);
}

void UFishPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	PickState();
	Animate(DeltaTime);
}

void UFishPlayerAnimInstance::NativeBeginPlay()
{
	Owner = Cast<AFishPlayer>(GetSkelMeshComponent()->GetOwner());
	MoveComp = Owner->MoveComp;
	ToIdleState();
}

void UFishPlayerAnimInstance::ToIdleState()
{
	if(AnimState == EPlayerAnimState::Idle) return;
	
	CurrentCurves = IdleCurves;
	AnimState = EPlayerAnimState::Idle;
	AnimStartTime = GetWorld()->GetTimeSeconds();
	
	PlantedRight = true;
	PlantedLeft = true;
}

void UFishPlayerAnimInstance::ToWalkState()
{
	if(AnimState == EPlayerAnimState::Walk) return;
	
	CurrentCurves = WalkCurves;
	AnimState = EPlayerAnimState::Walk;
	AnimStartTime = GetWorld()->GetTimeSeconds();
}

void UFishPlayerAnimInstance::ToJumpState()
{
	if(AnimState == EPlayerAnimState::Jump) return;
	
	CurrentCurves = JumpCurves;
	AnimState = EPlayerAnimState::Jump;
	AnimStartTime = GetWorld()->GetTimeSeconds();
}

void UFishPlayerAnimInstance::ToFallState()
{
	if(AnimState == EPlayerAnimState::Fall) return;
	
	CurrentCurves = FallCurves;
	AnimState = EPlayerAnimState::Fall;
	AnimStartTime = GetWorld()->GetTimeSeconds();
	
	PlantedRight = false;
	PlantedLeft = false;
}

void UFishPlayerAnimInstance::PickState()
{
	if(!MoveComp.IsValid()) return;
	
	if(MoveComp->WalkInput.SquaredLength() > 0)
		ToWalkState();
	else
		ToIdleState();
}

void UFishPlayerAnimInstance::UpdatePose()
{
	//move to next frame in the animation
	
	const float WorldTime = GetWorld()->GetTimeSeconds() - AnimStartTime;
	const float AnimTime = CurrentCurves->AnimTime;
	const float Time = WorldTime - static_cast<int>(WorldTime / AnimTime) * AnimTime;
	
	NextPose.HeadLocation = CurrentCurves->HeadCurve->GetVectorValue(Time);
	NextPose.HandRLocation = CurrentCurves->HandRCurve->GetVectorValue(Time);
	NextPose.HandLLocation = CurrentCurves->HandLCurve->GetVectorValue(Time);
	NextPose.FootRLocation = CurrentCurves->FootRCurve->GetVectorValue(Time);
	NextPose.FootLLocation = CurrentCurves->FootLCurve->GetVectorValue(Time);
	
	if(AnimState == EPlayerAnimState::Walk)
	{
		PlantedRight = Time > AnimTime / 2.f; //swap planted on contact pos
		PlantedLeft = !PlantedRight;
	}
}

void UFishPlayerAnimInstance::FootPlanting()
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
		GetWorld()->SweepSingleByChannel(GroundHit, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FootColShape);
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

void UFishPlayerAnimInstance::ApplyMaxSpeed(const float DeltaTime)
{
	const float MaxDistance = MaxAnimSpeed * DeltaTime;
	const float CurrentMaxDistance = FMath::Lerp(MaxDistance / 2.f, MaxDistance, FMath::Clamp(MoveComp->WalkInput.Length(), 0.f,1.f));
	auto Apply = [&](const FVector New, const FVector Old)->FVector
	{
		const float CurrDistance = FVector::Distance(Old, New);
		if (CurrDistance > CurrentMaxDistance)
		{
			return Old + (New - Old).GetSafeNormal() * CurrentMaxDistance;
		}
		return New;
	};
	
	NextPose.HeadLocation = Apply(NextPose.HeadLocation, CurrentPose.HeadLocation);
	NextPose.HandRLocation = Apply(NextPose.HandRLocation, CurrentPose.HandRLocation);
	NextPose.HandLLocation = Apply(NextPose.HandLLocation, CurrentPose.HandLLocation);
	NextPose.FootRLocation = Apply(NextPose.FootRLocation, CurrentPose.FootRLocation);
	NextPose.FootLLocation = Apply(NextPose.FootLLocation, CurrentPose.FootLLocation);
}

void UFishPlayerAnimInstance::Animate(const float DeltaTime)
{
	if(!CurrentCurves.IsValid()) return;
	
	UpdatePose();
	FootPlanting();
	ApplyMaxSpeed(DeltaTime);

	if(AnimState == EPlayerAnimState::Walk)
	{
		const FVector NewPlantedFootLocation = PlantedRight? NextPose.FootRLocation : NextPose.FootLLocation;
		const FVector OldPlantedFootLocation = PlantedRight? CurrentPose.FootRLocation : CurrentPose.FootLLocation;
		MoveComp->WalkDistance = abs(NewPlantedFootLocation.Y - OldPlantedFootLocation.Y);
	}
	CurrentPose = NextPose;
}
