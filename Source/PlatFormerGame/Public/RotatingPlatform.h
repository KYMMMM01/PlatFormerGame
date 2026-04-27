#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingPlatform.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class PLATFORMERGAME_API ARotatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	ARotatingPlatform();

	//회전의 중심이 되는 Scene Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* PivotRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	//초당 회전 속도 (Pitch, Yaw, Roll) - 단위 deg/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationRate = FRotator(0.f, 45.f, 0.f);

protected:
	virtual void Tick(float DeltaTime) override;
};