#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"
 
class UUserWidget;
class UAudioLibrary;
class UAudioComponent;
class UAudioLibrary;
class UAudioComponent; 

UCLASS()
class PLATFORMERGAME_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
 
public:
	AMainMenuGameMode();
 
	//메인 메뉴 위젯 클래스 (에디터에서 WBP_MainMenu 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* MainMenuWidget = nullptr;
	
	//오디오 라이브러리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<UAudioLibrary> AudioLibrary = nullptr;

	//TitleBGM 재생 컴포넌트
	UPROPERTY()
	TObjectPtr<UAudioComponent> TitleBGMComp = nullptr;
 
protected:
	virtual void BeginPlay() override;
};