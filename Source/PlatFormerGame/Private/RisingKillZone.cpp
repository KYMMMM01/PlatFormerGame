#include "RisingKillZone.h"
#include "Components/StaticMeshComponent.h"
#include "RopeGameMode.h"
#include "ToolContextInterfaces.h"
#include "Kismet/GameplayStatics.h"

ARisingKillZone::ARisingKillZone()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlaneMesh"));
	SetRootComponent(PlaneMesh);
	PlaneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ARisingKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	if (bGameModeKillZ)
	{
		if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->KillZ = GetActorLocation().Z;
		}
	}
}

void ARisingKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorWorldOffset(FVector(0.0f, 0.0f, RiseSpeed * DeltaTime), false);
	
	if (bGameModeKillZ)
	{
		if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->KillZ = GetActorLocation().Z;
		}
	}
}

