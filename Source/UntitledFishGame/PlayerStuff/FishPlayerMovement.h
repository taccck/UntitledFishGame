#pragma once
#include "FishPlayerMovement.generated.h"

class APlayerController;
class AFishPlayer;

UCLASS()
class UFishPlayerMovement : public UActorComponent
{
	GENERATED_BODY()
public:
	UFishPlayerMovement();
	void BindInput(UInputComponent* InputComp, APlayerController* PC);
	void UnbindInput(UInputComponent* InputComp);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
private:
	void Move(const float DeltaTime) const;
	bool GroundCheck();
	void CalculateWalkVelocity(const float DeltaTime, const bool OnGround);
	void CalculateGravityVelocity(const float DeltaTime, const bool OnGround);
	void FloatUp(const float DeltaTime);

	void Jump();
	void StopJump();
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void TurnUp(float Axis);
	void TurnRight(float Axis);
	void LookUp(float Delta);
	void LookRight(float Delta);

private:
	UPROPERTY(EditAnywhere, Category = "Move|Look")
	float GamepadTurnRate = 100.f;
	UPROPERTY(EditAnywhere, Category = "Move|Look")
	float MouseSensitivity = .5f;
	UPROPERTY(EditAnywhere, Category = "Move|Walk")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Move|Walk")
	float WalkAcceleration = 3000.f;
	UPROPERTY(EditAnywhere, Category = "Move|Walk")
	float WalkDeceleration = 4000.f;
	UPROPERTY(EditAnywhere, Category = "Move|Walk")
	float WalkableSlope = 45.f;
	UPROPERTY(EditAnywhere, Category = "Move|Float")
	float FloatHeight = 20.5f;
	UPROPERTY(EditAnywhere, Category = "Move|Float")
	float FloatSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Move|Gravity")
	float GravityAcceleration = 3000.f;
	UPROPERTY(EditAnywhere, Category = "Move|Gravity")
	float TerminalFallSpeed = 6000.f;
	UPROPERTY(EditAnywhere, Category = "Move|Jump")
	float JumpSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Move|Jump")
	float JumpInputTime = .2f;

	TWeakObjectPtr<AFishPlayer> Owner;
	TWeakObjectPtr<APlayerController> PlayerController;
	FVector WalkInput;
	FVector WalkVelocity;
	FVector GravityVelocity;
	FVector GroundNormal;
	float DistanceToGround;
	bool bDoJump;
	float CurrentJumpTime;

	FCollisionQueryParams QueryParams;
	FCollisionShape ColShape;
	float PlayerRadius;
	float PlayerHalfHeight;

	const size_t MaxMoveSteps = 10;
};
