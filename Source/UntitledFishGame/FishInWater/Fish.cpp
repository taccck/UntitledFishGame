#include "Fish.h"

AFish::AFish()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

void AFish::BeginPlay()
{
	Super::BeginPlay();
	SwimDirection = FVector(FMath::RandRange(0.f,1.f), FMath::RandRange(0.f,1.f), 0.f);
	SwimDirection.Normalize();
}

void AFish::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AddActorWorldOffset(SwimDirection * (DeltaSeconds * SwimSpeed));
	if(FindLure())
	{
		Destroy();
	}
}

bool AFish::FindLure() const
{
	return FVector::Distance(LureLocation, GetActorLocation()) < 50.f;
}

