#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "AudioLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	//메인 메뉴에서 Pawn 비활성화
	DefaultPawnClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	//메인 메뉴 위젯 생성
	if (MainMenuWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		MainMenuWidget = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}

	//마우스 커서 노출, UI 입력
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}
	
	if (AudioLibrary && AudioLibrary->Title)
	{
		TitleBGMComp = UGameplayStatics::SpawnSound2D(this, AudioLibrary->Title);
	}
}