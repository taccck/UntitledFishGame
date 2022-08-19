#include "FishPlayerMovement.h"
#include "Components/CapsuleComponent.h"
#include "FishPlayer.h"

UFishPlayerMovement::UFishPlayerMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<AFishPlayer>(GetOwner());
}

void UFishPlayerMovement::BindInput(UInputComponent* InputComp, APlayerController* PC)
{
	PlayerController = PC;
	
	InputComp->BindAxis(TEXT("WalkForward"), this, &UFishPlayerMovement::MoveForward);
	InputComp->BindAxis(TEXT("WalkRight"), this, &UFishPlayerMovement::MoveRight);
	InputComp->BindAxis(TEXT("TurnUp"), this, &UFishPlayerMovement::TurnUp);
	InputComp->BindAxis(TEXT("TurnRight"), this, &UFishPlayerMovement::TurnRight);
	InputComp->BindAxis(TEXT("LookUp"), this, &UFishPlayerMovement::LookUp);
	InputComp->BindAxis(TEXT("LookRight"), this, &UFishPlayerMovement::LookRight);
	InputComp->BindAction(TEXT("Jump"), IE_Pressed, this, &UFishPlayerMovement::Jump);
	InputComp->BindAction(TEXT("Jump"), IE_Released, this, &UFishPlayerMovement::StopJump);
}

void UFishPlayerMovement::UnbindInput(UInputComponent* InputComp)
{
	PlayerController = nullptr;
	InputComp->ClearActionBindings();
	InputComp->AxisBindings.Empty();
}

void UFishPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const bool OnGround = GroundCheck();
	if(OnGround)
	{
		FloatUp(DeltaTime);
	}
	
	CalculateGravityVelocity(DeltaTime, OnGround);
	CalculateWalkVelocity(DeltaTime, OnGround);
	Move(DeltaTime);
}

void UFishPlayerMovement::BeginPlay()
{
	QueryParams.AddIgnoredActor(GetOwner());
	PlayerRadius = 22.f;
	PlayerHalfHeight = 33.f;
	ColShape = FCollisionShape::MakeSphere(PlayerRadius);
	WalkableSlope += .1f;
}

void UFishPlayerMovement::Move(const float DeltaTime) const
{
	float RemainingTime = DeltaTime;
	FVector Velocity = GravityVelocity + WalkVelocity;
	for (size_t Steps = 0; Steps < MaxMoveSteps; Steps++)
	{
		FHitResult Hit;
		Owner->AddActorWorldOffset(Velocity * RemainingTime, true, &Hit);
		if(Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				Owner->AddActorWorldOffset(Hit.Normal * (Hit.PenetrationDepth + .01));
				continue;
			}
			Velocity = FVector::VectorPlaneProject(Velocity, Hit.Normal);
			RemainingTime -= RemainingTime * Hit.Time;
			continue;
		}
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("STEPBROTHER IM STUCK, unable to depenetrate"));
}

bool UFishPlayerMovement::GroundCheck()
{
	FVector Start = Owner->GetActorLocation();
	Start.Z -= PlayerHalfHeight - PlayerRadius;
	const FVector End = Start + FVector::DownVector * (FloatHeight + 1.f);
	FHitResult GroundSweep;
	GetWorld()->SweepSingleByChannel(GroundSweep, Start, End, FQuat::Identity, ECC_Visibility, ColShape, QueryParams);

	if(GroundSweep.bBlockingHit)
	{
		DistanceToGround = GroundSweep.Distance;
		GroundNormal = GroundSweep.Normal; 
		return acos(FVector::DotProduct(GroundNormal, FVector::UpVector)) * 57.2957795f < WalkableSlope;
	}
	return false;
}

void UFishPlayerMovement::CalculateWalkVelocity(const float DeltaTime, const bool OnGround)
{
	const float InputSize = FMath::Clamp(WalkInput.Length(), 0.f,1.f);
	if (InputSize > 0.f)
	{
		const float AngleToRotate = PlayerController->GetControlRotation().Yaw;
		FVector WalkDirection = WalkInput.RotateAngleAxis(AngleToRotate, FVector::UpVector);
		WalkVelocity += WalkDirection.GetSafeNormal() * InputSize * (WalkAcceleration * DeltaTime);
		const bool WalkingDown = OnGround && FVector::DotProduct(GroundNormal.GetSafeNormal2D(), WalkInput.GetSafeNormal2D());
		if (WalkingDown)
		{
			const FVector Right = FVector::CrossProduct(GroundNormal, WalkVelocity.GetSafeNormal());
			WalkDirection = FVector::CrossProduct(Right.GetSafeNormal(), GroundNormal);
			WalkVelocity = WalkDirection.GetSafeNormal() * WalkVelocity.Length();
		}
		if (WalkVelocity.SizeSquared() > FMath::Square(WalkSpeed)) //accelerated past max speed
		{
			WalkVelocity = WalkVelocity.GetSafeNormal() * WalkSpeed;
		}
		return;
	}
	
	if(WalkVelocity.SizeSquared() > 1.f)
	{
		const FVector WalkDirection = WalkVelocity.GetSafeNormal();
		WalkVelocity -= WalkDirection * (WalkDeceleration * DeltaTime);
		if (WalkVelocity.GetSafeNormal().Dot(WalkDirection) < 0.f) //decelerated past zero
		{
			WalkVelocity = FVector::ZeroVector;
		}
		return;
	}
	
	WalkVelocity = FVector::ZeroVector;
}

void UFishPlayerMovement::CalculateGravityVelocity(const float DeltaTime, const bool OnGround)
{
	const bool JumpTime = CurrentJumpTime > 0.f && CurrentJumpTime < JumpInputTime;
	const bool JumpConditions = JumpTime || OnGround;
	if(JumpConditions && DoJump)
	{
		GravityVelocity = FVector::UpVector * 300.f;
		CurrentJumpTime += DeltaTime;
		return;
	}
	DoJump = false;
	CurrentJumpTime = 0.f;
	
	if(OnGround)
	{
		GravityVelocity = FVector::ZeroVector;
		return;
	}
	
	if (GravityVelocity.SizeSquared() < FMath::Square(TerminalFallSpeed))
	{
		GravityVelocity += FVector::DownVector * (3000.f * DeltaTime);
	}
}

void UFishPlayerMovement::FloatUp(const float DeltaTime)
{
	if (DistanceToGround < FloatHeight)
	{
		const float DistToMove = FMath::Clamp(FloatSpeed * DeltaTime, 0.f, FloatHeight - DistanceToGround);
		GetOwner()->AddActorWorldOffset(FVector::UpVector * DistToMove);
	}
}

void UFishPlayerMovement::Jump()
{
	DoJump = true;
}

void UFishPlayerMovement::StopJump()
{
	DoJump = false;
}

void UFishPlayerMovement::MoveForward(float Axis)
{
	WalkInput.X = Axis;
}

void UFishPlayerMovement::MoveRight(float Axis)
{
	WalkInput.Y = Axis;
}

void UFishPlayerMovement::TurnUp(float Axis)
{
	const float DeltaPitch = GetWorld()->DeltaTimeSeconds * TurnRate/2.f * Axis;
	PlayerController->AddPitchInput(DeltaPitch);
}

void UFishPlayerMovement::TurnRight(float Axis)
{
	const float DeltaYaw = GetWorld()->DeltaTimeSeconds * TurnRate * Axis;
	PlayerController->AddYawInput(DeltaYaw);
}

void UFishPlayerMovement::LookUp(float Delta)
{
	const float DeltaPitch = MouseSensitivity * Delta;
	PlayerController->AddPitchInput(DeltaPitch);
}

void UFishPlayerMovement::LookRight(float Delta)
{
	const float DeltaYaw = MouseSensitivity * Delta;
	PlayerController->AddYawInput(DeltaYaw);
}
