#include "CoinActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RopeCharacter.h"
#include "RopeGameMode.h"
#include "Kismet/GameplayStatics.h"

ACoinActor::ACoinActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(80.f);
	SphereComp->SetCollisionProfileName(TEXT("Trigger"));
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::OnOverlapBegin);

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("CoinMesh"));
	CoinMesh->SetupAttachment(SphereComp);
	CoinMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CoinMesh)
	{
		CoinMesh->AddLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}

void ACoinActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ARopeCharacter>(OtherActor)) return;
	
	if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnCoinCollected(GetActorLocation());
	}
	
	Destroy();
}

