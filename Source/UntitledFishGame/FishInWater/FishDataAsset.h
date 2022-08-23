#pragma once
#include "Fish.h"
#include "FishDataAsset.generated.h"

USTRUCT()
struct FFishClassArray
{
	GENERATED_BODY()
	TArray<TSubclassOf<AFish>> Array;
	TSubclassOf<AFish> operator[] (int32 i)
	{
		return Array[i];
	}
};

UCLASS(BlueprintType)
class UFishDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	TArray<FFishClassArray> Fishies;

	TArray<TSubclassOf<AFish>> operator[] (int32 i)
	{
		return Fishies[i].Array;
	}
};