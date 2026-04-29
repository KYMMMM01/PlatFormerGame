#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class PLATFORMERGAME_API ACoinActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoinActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* CoinMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set")
	float RotationSpeed = 90.f;
protected:
	virtual void Tick(float DeltaTime) override;
	
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
