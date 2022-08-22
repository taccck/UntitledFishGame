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
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Fishing"))
	void ReceiveFishing();
	
public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Collider;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFishPlayerMovement* MoveComp;
};
