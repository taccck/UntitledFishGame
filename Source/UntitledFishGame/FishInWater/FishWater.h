#pragma once
#include "FishWater.generated.h"

class UFishDataAsset;
class UBoxComponent;

UCLASS(abstract)
class AFishWater : public AActor
{
	GENERATED_BODY()
public:
	AFishWater();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

public:
	int Level = 1;
	float Height;
	
private:
	UPROPERTY(EditAnywhere)
	UFishDataAsset* FishData;
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;

	float Timer;
	
};
