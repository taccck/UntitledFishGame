#include "FishPlayer.h"

#include "FishPlayerMovement.h"
#include "Components/StaticMeshComponent.h"

AFishPlayer::AFishPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	RootComponent = Collider;
	Collider->SetVisibility(false);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	Mesh->SetupAttachment(Collider);
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
