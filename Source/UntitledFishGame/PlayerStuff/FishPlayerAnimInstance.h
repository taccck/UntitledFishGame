#pragma once
#include "FishPlayerAnimInstance.generated.h"

class UCurveVector;
class UFishPlayerMovement;
class AFishPlayer;

UCLASS(BlueprintType)
class UPlayerCurveData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector* HeadCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector* HandRCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector* HandLCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector* FootRCurve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector* FootLCurve;
};

USTRUCT(BlueprintType)
struct FSkeletonPose
{
	GENERATED_BODY() //should set default values here
	UPROPERTY(BlueprintReadOnly)
	FVector HeadLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector HandRLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector HandLLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector FootRLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector FootLLocation;
};

UENUM(BlueprintType)
enum class EPlayerAnimState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Jump UMETA(DisplayName = "Jump"),
	Fall UMETA(DisplayName = "Fall")
};

UCLASS()
class UFishPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFishPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ToIdleState();
	UFUNCTION(BlueprintCallable)
	void ToWalkState();
	UFUNCTION(BlueprintCallable)
	void ToJumpState();
	UFUNCTION(BlueprintCallable)
	void ToFallState();

private:
	void PickState();
	void UpdatePose();
	void FootPlanting();
	void ApplyMaxSpeed(const float DeltaTime);
	void Animate(const float DeltaTime);

public:
	UPROPERTY(EditAnywhere)
	UPlayerCurveData* IdleCurves;
	UPROPERTY(EditAnywhere)
	UPlayerCurveData* WalkCurves;
	UPROPERTY(EditAnywhere)
	UPlayerCurveData* JumpCurves;
	UPROPERTY(EditAnywhere)
	UPlayerCurveData* FallCurves;

	UPROPERTY(BlueprintReadOnly)
	FSkeletonPose CurrentPose;
	UPROPERTY(BlueprintReadOnly)
	FSkeletonPose NextPose;

	UPROPERTY(BlueprintReadOnly)
	EPlayerAnimState AnimState;
	UPROPERTY(EditAnywhere)
	float MaxAnimSpeed = 150.f;

private:
	UPROPERTY(EditAnywhere)
	float FootSnappingHeight = 21.f;
	
	TWeakObjectPtr<AFishPlayer> Owner;
	TWeakObjectPtr<UFishPlayerMovement> MoveComp;
	TWeakObjectPtr<UPlayerCurveData> CurrentCurves;
	FCollisionShape FootColShape;
	float AnimStartTime;
	bool PlantedRight;
	bool PlantedLeft;
};