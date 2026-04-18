#include "GoalActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RopeCharacter.h"
#include "RopeGameMode.h"
#include "Kismet/GameplayStatics.h"

AGoalActor::AGoalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(150.f, 150.f, 200.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OnOverlapBegin);

	//보이는 메쉬
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("GoalMesh"));
	GoalMesh->SetupAttachment(TriggerBox);
	GoalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGoalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ARopeCharacter>(OtherActor)) return;

	if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnLevelCleared();
	}
}