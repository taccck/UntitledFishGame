#include "FishPlayerCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AFishPlayerCamera::AFishPlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	Tripod = CreateDefaultSubobject<USpringArmComponent>(TEXT("Tripod"));
	RootComponent = Tripod;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Tripod);
}

void AFishPlayerCamera::Tick(float DeltaSeconds)
{
	if (Follow)
	{
		SetActorLocation(Follow->GetActorLocation());
	}
	if (PlayerController != nullptr)
	{
		SetActorRotation(PlayerController->GetControlRotation());
	}
}

void AFishPlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = this->GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTarget(this);
}

void AFishPlayerCamera::BecomeViewTarget(APlayerController* PC)
{
	Super::BecomeViewTarget(PC);
	if (Follow)
	{
		Follow->EnableInput(PC);
	}
}

void AFishPlayerCamera::EndViewTarget(APlayerController* PC)
{
	Super::EndViewTarget(PC);
	if (Follow)
	{
		Follow->DisableInput(PC);
	}
}

