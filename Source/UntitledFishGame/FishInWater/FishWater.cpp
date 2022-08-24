#include "FishWater.h"

#include "FishDataAsset.h"
#include "Components/BoxComponent.h"

AFishWater::AFishWater()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Root);
}

void AFishWater::BeginPlay()
{
	Super::BeginPlay();
	Height = GetActorLocation().Z;
}

void AFishWater::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timer += DeltaTime;
	if (Timer > 10.f)
	{
		FishData->SpawnFish(Level);
	}
}
