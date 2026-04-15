

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RopeCharacter.generated.h"

UCLASS()
class PLATFORMERGAME_API ARopeCharacter : public APawn
{
	GENERATED_BODY()

public:
	ARopeCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
