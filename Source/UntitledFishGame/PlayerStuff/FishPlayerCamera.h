#pragma once
#include "FishPlayerCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract)
class AFishPlayerCamera : public AActor
{
	GENERATED_BODY()
public:
	AFishPlayerCamera();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Follow;

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* Tripod;

	TWeakObjectPtr<APlayerController> PlayerController;
};
