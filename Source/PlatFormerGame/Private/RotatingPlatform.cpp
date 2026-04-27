#include "RotatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
 
ARotatingPlatform::ARotatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
 
	//회전 중심 Scene Root
	PivotRoot = CreateDefaultSubobject<USceneComponent>(FName("PivotRoot"));
	SetRootComponent(PivotRoot);
 
	//메쉬는 PivotRoot의 자식. BP 인스턴스에서 Relative Location 조정해 메쉬 중심을 피벗에 맞춤
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlatformMesh"));
	PlatformMesh->SetupAttachment(PivotRoot);
}
 
void ARotatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation(RotationRate * DeltaTime);
}