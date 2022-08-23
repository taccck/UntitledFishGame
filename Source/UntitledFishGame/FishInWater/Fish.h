#pragma once
#include "Components/CapsuleComponent.h"
#include "Fish.generated.h"

UCLASS(abstract)
class AFish : public AActor
{
	GENERATED_BODY()
public:
	AFish();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	bool FindLure() const;
	
public:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Collider;
	UPROPERTY(EditAnywhere)
	FName FishName = TEXT("empty");
	UPROPERTY(EditAnywhere)
	float Commonness = 1;
	
	//add enum for rarity
	FVector LureLocation; //add lure class
	
private:
	UPROPERTY(EditAnywhere)
	float SwimSpeed = 100.f;
	
	FVector SwimDirection;
};
