#pragma once
#include "FishDataAsset.h"
#include "FishSpawner.generated.h"

UCLASS()
class UFishSpawner : public UActorComponent
{
	GENERATED_BODY()
public:
	UFishSpawner();
	AActor* SpawnFish(const int Level) const;

private:
	TArray<TSubclassOf<AFish>> GetGradeClasses(const int Level) const;
	
	double GradeOdds [9][7] =
	{
		{.750f, .190f, .050f, .001f, 0.f, 0.f, 0.f},
		{.666f, .250f, .075f, .0025f, 0.f, 0.f, 0.f},
		{.575f, .310f, .100f, .010f, .001f, 0.f, 0.f},
		{.475f, .300f, .200f, .020f, .0025f, 0.f, 0.f},
		{.200f, .480f, .250f, .050f, .010f, .001f, 0.f},
		{.100f, .480f, .290f, .100f, .025f, .0025f, 0.f},
		{0.f, .240f, .500f, .200f, .050f, .010f, 0.f},
		{0.f, 0.f, .200f, .670f, .100f, .025f, .001f}, //soft cap
		{0.f, 0.f, 0.f, 0.f, .666f, .222f, .112f}
	};
	
public:
	UPROPERTY(EditAnywhere)
	UFishDataAsset* FishTable;
};