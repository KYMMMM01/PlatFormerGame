#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AudioLibrary.generated.h"

UCLASS()
class PLATFORMERGAME_API UAudioLibrary : public UDataAsset
{
	GENERATED_BODY()
	
	public:
	
	//SFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> HookFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> HookImpact_A = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> HookImpact_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> Dash = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> Footstep_A = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> Footstep_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<USoundBase> Land = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	TObjectPtr<USoundBase> Coin = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	TObjectPtr<USoundBase> GameOver = nullptr;
	
	//BGM
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	TObjectPtr<USoundBase> Title = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	TObjectPtr<USoundBase> ClearBGM = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tuning")
	float FootstepInterval = 0.3f;
};
