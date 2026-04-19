#include "RotatingPlatform.h"
#include "Components/StaticMeshComponent.h"

ARotatingPlatform::ARotatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlatformMesh"));
	SetRootComponent(PlatformMesh);
}

void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//프레임 독립적 회전
	AddActorLocalRotation(RotationRate * DeltaTime);
}