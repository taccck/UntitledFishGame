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

UCLASS(Blueprintable)
class UFishPlayerAnim : public UActorComponent
{
	GENERATED_BODY()
public:
	UFishPlayerAnim();
	
	UFUNCTION(BlueprintCallable)
	void Idle();
	UFUNCTION(BlueprintCallable)
	void Walk();
	UFUNCTION(BlueprintCallable)
	void Jump();
	UFUNCTION(BlueprintCallable)
	void Fall();

private:
	void FootPlanting();

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
	FVector HeadLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector HandRLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector HandLLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector FootRLocation;
	UPROPERTY(BlueprintReadOnly)
	FVector FootLLocation;

	float MoveDistance;

private:
	UPROPERTY(EditAnywhere)
	float FootSnappingHeight = 22.f;
	
	TWeakObjectPtr<AFishPlayer> Owner;
	bool PlantedRight;
	bool PlantedLeft;
};