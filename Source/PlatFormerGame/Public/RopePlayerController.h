#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RopePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
UCLASS()
class PLATFORMERGAME_API ARopePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:
	ARopePlayerController();
	
	//Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dash;
	
protected:
	virtual void BeginPlay() override;
	
};
