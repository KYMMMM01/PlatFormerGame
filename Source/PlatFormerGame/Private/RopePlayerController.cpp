#include "RopePlayerController.h"
#include  "EnhancedInputSubsystems.h"


ARopePlayerController::ARopePlayerController()
	: IA_Move(nullptr), IA_Look(nullptr), IA_Jump(nullptr), IA_Dash(nullptr)
{
	
}

void ARopePlayerController::BeginPlay()
{
	Super::BeginPlay();
    
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) //현재 플레이어의 LocalPlayer 객체를 가져와라 (우선적으로 가져와야함)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
		   LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Character)
			{
				SubSystem->AddMappingContext(IMC_Character, 0); //0은 가장 높은 우선순위
			}
		}
	}
}