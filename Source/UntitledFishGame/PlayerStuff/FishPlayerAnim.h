#pragma once

#include "FishPlayerAnim.generated.h"

class UCurveVector;
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
	GENERATED_BODY()
public:
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

UCLASS(Blueprintable)
class UFishPlayerAnim : public UActorComponent
{
	GENERATED_BODY()
public:
	UFishPlayerAnim();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void ToIdleState();
	UFUNCTION(BlueprintCallable)
	void ToWalkState();
	UFUNCTION(BlueprintCallable)
	void ToJumpState();
	UFUNCTION(BlueprintCallable)
	void ToFallState();

private:
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
	FString AnimState;

	float MoveDistance;

private:
	UPROPERTY(EditAnywhere)
	float FootSnappingHeight = 22.f;
	UPROPERTY(EditAnywhere)
	float MaxAnimSpeed = 100.f;
	
	TWeakObjectPtr<AFishPlayer> Owner;
	TWeakObjectPtr<UPlayerCurveData> CurrentCurves;
	bool PlantedRight;
	bool PlantedLeft;
};