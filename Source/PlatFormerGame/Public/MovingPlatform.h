#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

class UStaticMeshComponent;

UCLASS()
class PLATFORMERGAME_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* PlatformMesh;
	
	//이동 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MoveDirection = FVector(0.f, 1.f, 0.f);
	
	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 300.f;
	
	//왕복 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRange = 500.f;
	
	//회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnSpeed = 5.f;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//왕복 기준점
	FVector StartLocation = FVector::ZeroVector;
	FRotator BaseRotation = FRotator::ZeroRotator;
	//이동 방향
	int32 DirectionSign = 1;
}; 
