#include "FishPlayer.h"
#include "FishPlayerMovement.h"
#include "Components/CapsuleComponent.h"

AFishPlayer::AFishPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;
	MoveComp = CreateDefaultSubobject<UFishPlayerMovement>(TEXT("MovementComponent"));
}

void AFishPlayer::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	MoveComp->BindInput(InputComponent, PlayerController);
}

void AFishPlayer::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);
	MoveComp->UnbindInput(InputComponent);
}
