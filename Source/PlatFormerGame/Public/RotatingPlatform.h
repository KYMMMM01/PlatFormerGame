#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingPlatform.generated.h"

class UStaticMeshComponent;


  //매 프레임 지정된 속도로 회전하는 플랫폼.
  //로프 그래플링 타겟, 시각적 장애물, 기믹용으로 활용.
 
UCLASS()
class PLATFORMERGAME_API ARotatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	ARotatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	// 초당 회전 속도 (Pitch, Yaw, Roll) - 단위 deg/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationRate = FRotator(0.f, 45.f, 0.f);

protected:
	virtual void Tick(float DeltaTime) override;
};