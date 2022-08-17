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
	void CalculateGravityVelocity(const float DeltaTime);
	void FloatUp(const float DeltaTime);
	
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void TurnUp(float Axis);
	void TurnRight(float Axis);
	void LookUp(float Delta);
	void LookRight(float Delta);

private:
	UPROPERTY(EditAnywhere)
	float TurnRate = 100.f;
	UPROPERTY(EditAnywhere)
	float MouseSensitivity = .5f;
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere)
	float WalkAcceleration = 3000.f;
	UPROPERTY(EditAnywhere)
	float WalkDeceleration = 4000.f;
	UPROPERTY(EditAnywhere)
	float FloatHeight = 10.f;
	UPROPERTY(EditAnywhere)
	float FloatSpeed = 100.f;
	UPROPERTY(EditAnywhere)
	float GravityAcceleration = 3000.f;
	UPROPERTY(EditAnywhere)
	float TerminalFallSpeed = 6000.f;

	TWeakObjectPtr<AFishPlayer> Owner;
	TWeakObjectPtr<APlayerController> PlayerController;
	FVector WalkInput;
	FVector WalkVelocity;
	FVector GravityVelocity;
	FVector GroundNormal;
	float DistanceToGround;

	FCollisionQueryParams QueryParams;
	FCollisionShape ColShape;
	float PlayerRadius;
	float PlayerHalfHeight;

	const size_t MaxMoveSteps = 10;

	//temp
	float MaxHeight;
};
