#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"
 
class UUserWidget;
 
UCLASS()
class PLATFORMERGAME_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
 
public:
	AMainMenuGameMode();
 
	// 메인 메뉴 위젯 클래스 (에디터에서 WBP_MainMenu 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* MainMenuWidget = nullptr;
 
protected:
	virtual void BeginPlay() override;
};