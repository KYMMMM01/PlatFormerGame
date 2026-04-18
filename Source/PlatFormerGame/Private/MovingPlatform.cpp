#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMesh");
	SetRootComponent(PlatformMesh);
}


void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FVector Dir = MoveDirection.GetSafeNormal(); //정규화 
	if (Dir.IsNearlyZero()) return;
	
	const FVector Delta = Dir * MoveSpeed * DirectionSign * DeltaTime;
	const FVector NewLocation = GetActorLocation() + Delta;
	
	const float DistFromStart = FVector::DotProduct(NewLocation - StartLocation, Dir);
	
	// 범위
	if (FMath::Abs(DistFromStart) >= MaxRange)
	{
		const float ClampedDist = FMath::Sign(DistFromStart) * MaxRange;
		SetActorLocation(StartLocation + Dir * ClampedDist);
		DirectionSign *= -1;
	}
	else
	{
		SetActorLocation(NewLocation);
	}
}

