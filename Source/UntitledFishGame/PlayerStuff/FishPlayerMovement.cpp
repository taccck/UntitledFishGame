#include "FishPlayerMovement.h"
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
	const bool bOnGround = GroundCheck();
	if(bOnGround)
	{
		FloatUp(DeltaTime);
	}
	
	CalculateGravityVelocity(DeltaTime, bOnGround);
	CalculateWalkVelocity(DeltaTime, bOnGround);
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

void UFishPlayerMovement::CalculateWalkVelocity(const float DeltaTime, const bool bOnGround)
{
	const float InputSize = FMath::Clamp(WalkInput.Length(), 0.f,1.f);
	if (InputSize > 0.f) //accelerate
	{
		const float AngleToRotate = PlayerController->GetControlRotation().Yaw;
		const FVector WalkDirection = WalkInput.RotateAngleAxis(AngleToRotate, FVector::UpVector);
		WalkVelocity += WalkDirection.GetSafeNormal() * InputSize * (WalkAcceleration * DeltaTime);
		const bool bWalkingDown = bOnGround && FVector::DotProduct(GroundNormal.GetSafeNormal2D(), WalkInput.GetSafeNormal2D());
		if (bWalkingDown)
		{
			WalkVelocity = FVector::VectorPlaneProject(WalkVelocity, GroundNormal);
		}
		const bool bPassedMaxSpeed = WalkVelocity.SizeSquared() > FMath::Square(WalkSpeed);
		if (bPassedMaxSpeed) 
		{
			WalkVelocity = WalkVelocity.GetSafeNormal() * WalkSpeed;
		}
		return;
	}
	
	if(WalkVelocity.SizeSquared() > 1.f) //decelerate
	{
		const FVector WalkDirection = WalkVelocity.GetSafeNormal();
		WalkVelocity -= WalkDirection * (WalkDeceleration * DeltaTime);
		const bool bPassedZeroSpeed = WalkVelocity.GetSafeNormal().Dot(WalkDirection) < 0.f;
		if (bPassedZeroSpeed)
		{
			WalkVelocity = FVector::ZeroVector;
		}
		return;
	}
	
	WalkVelocity = FVector::ZeroVector;
}

void UFishPlayerMovement::CalculateGravityVelocity(const float DeltaTime, const bool bOnGround)
{
	const bool bJumpTime = CurrentJumpTime > 0.f && CurrentJumpTime < JumpInputTime;
	const bool bJumpConditions = bJumpTime || bOnGround;
	if(bJumpConditions && bDoJump)
	{
		GravityVelocity = FVector::UpVector * 300.f;
		CurrentJumpTime += DeltaTime;
		return;
	}
	bDoJump = false;
	CurrentJumpTime = 0.f;
	
	if(bOnGround)
	{
		GravityVelocity = FVector::ZeroVector;
		return;
	}

	const bool bPassedTerminalVelocity = GravityVelocity.SizeSquared() < FMath::Square(TerminalFallSpeed);
	if (bPassedTerminalVelocity)
	{
		GravityVelocity += FVector::DownVector * (GravityAcceleration * DeltaTime);
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
	bDoJump = true;
}

void UFishPlayerMovement::StopJump()
{
	bDoJump = false;
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
	const float DeltaPitch = GetWorld()->DeltaTimeSeconds * GamepadTurnRate/2.f * Axis;
	PlayerController->AddPitchInput(DeltaPitch);
}

void UFishPlayerMovement::TurnRight(float Axis)
{
	const float DeltaYaw = GetWorld()->DeltaTimeSeconds * GamepadTurnRate * Axis;
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
