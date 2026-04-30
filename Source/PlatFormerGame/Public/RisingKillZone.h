#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RisingKillZone.generated.h"

class UStaticMeshComponent;

UCLASS()
class PLATFORMERGAME_API ARisingKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ARisingKillZone();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlaneMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float RiseSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool bGameModeKillZ = true;	
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
