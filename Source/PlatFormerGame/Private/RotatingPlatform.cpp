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

	//프레임 독립적 회전 : RotationRate는 deg/s이므로 DeltaTime을 곱함
	AddActorLocalRotation(RotationRate * DeltaTime);
}