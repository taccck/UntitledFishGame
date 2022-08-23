#include "FishSpawner.h"

UFishSpawner::UFishSpawner()
{
	PrimaryComponentTick.bCanEverTick = true;
}

AActor* UFishSpawner::SpawnFish(const int Level) const
{
	TArray<TSubclassOf<AFish>> GradeClasses = GetGradeClasses(Level);

	double TotalOdds = 0;
	for (size_t i = 0; i < GradeClasses.Num(); i++)
	{
		TotalOdds += GradeClasses[i]->GetDefaultObject<AFish>()->Commonness;
	}
	const double FishRoll = FMath::RandRange(.0 ,TotalOdds);
	
	double CurrentOdds = 0;
	for (size_t i = 0; i < GradeClasses.Num(); i++)
	{
		CurrentOdds += GradeClasses[i]->GetDefaultObject<AFish>()->Commonness;
		if(FishRoll <= CurrentOdds)
		{
			return GetWorld()->SpawnActor(GradeClasses[i]);
		}
	}
	return nullptr;
}

TArray<TSubclassOf<AFish>> UFishSpawner::GetGradeClasses(const int Level) const
{
	const double* Odds = GradeOdds[(Level - 1)];
	constexpr double TotalOdds = Odds[0] + Odds[1] + Odds[2] + Odds[3] + Odds[4] + Odds[5] + Odds[6];
	const double Grade = FMath::RandRange(.0 ,TotalOdds);
            
	double CurrentOdds = 0;
	for(size_t i = 0; i < 7; i++)
	{
		CurrentOdds += Odds[i];
		if (Grade <= CurrentOdds)
		{
			return FishTable->Fishies[i].Array;
		}
	}
	return {};
}

