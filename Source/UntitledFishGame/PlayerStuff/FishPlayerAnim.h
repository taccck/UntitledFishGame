#pragma once

#include "FishPlayerAnim.generated.h"

class UCurveVector;

USTRUCT(BlueprintType)
struct FPlayerCurveData
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
	UFUNCTION(BlueprintCallable)
	void Idle();
	UFUNCTION(BlueprintCallable)
	void Walk();
	UFUNCTION(BlueprintCallable)
	void Jump();
	UFUNCTION(BlueprintCallable)
	void Fall();

public:
	UPROPERTY(EditAnywhere)
	FPlayerCurveData IdleCurves;
	UPROPERTY(EditAnywhere)
	FPlayerCurveData WalkCurves;
	UPROPERTY(EditAnywhere)
	FPlayerCurveData JumpCurves;
	UPROPERTY(EditAnywhere)
	FPlayerCurveData FallCurves;
	
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