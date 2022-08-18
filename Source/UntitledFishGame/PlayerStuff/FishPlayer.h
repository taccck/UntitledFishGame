#pragma once
#include "FishPlayer.generated.h"

class UCapsuleComponent;
class UInputComponent;
class UFishPlayerMovement;
class UStaticMeshComponent;

UCLASS(Abstract)
class AFishPlayer : public AActor
{
	GENERATED_BODY()

public:
	AFishPlayer();
	virtual void EnableInput(APlayerController* PlayerController) override;
	virtual void DisableInput(APlayerController* PlayerController) override;

public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Collider;

private:
	UPROPERTY(EditAnywhere)
	UFishPlayerMovement* MoveComp;
};
