#include "RopeGameMode.h"
#include "RopeCharacter.h"
#include "RopePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
 
ARopeGameMode::ARopeGameMode()
{
	DefaultPawnClass = ARopeCharacter::StaticClass();
	PlayerControllerClass = ARopePlayerController::StaticClass();
}

void ARopeGameMode::BeginPlay()
{
	Super::BeginPlay();
 
	//플레이어 시작 위치 기억 (리스폰 때 사용)
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		PlayerStartLocation = Player->GetActorLocation();
	}
 
	//HUD 위젯 생성
	if (HUDWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		HUDWidget = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
 
	CurrentState = ERopeGameState::Playing;
	OnGameStateChanged(CurrentState);
}
 
void ARopeGameMode::OnLevelCleared()
{
	if (CurrentState != ERopeGameState::Playing) return;
 
	CurrentState = ERopeGameState::Cleared;
	OnGameStateChanged(CurrentState);
 
	//클리어 시 입력 비활성 + 마우스 커서 노출
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetPause(true);
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}
}
 
void ARopeGameMode::OnPlayerFell()
{
	if (CurrentState != ERopeGameState::Playing) return;
 
	CurrentState = ERopeGameState::GameOver;
	OnGameStateChanged(CurrentState);
 
	// 지정 시간 후 자동 리스폰
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ARopeGameMode::RespawnPlayer, RespawnDelay, false);
}
 
void ARopeGameMode::RespawnPlayer()
{
	if (ARopeCharacter* Player = Cast<ARopeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->Respawn(PlayerStartLocation);
	}
	CurrentState = ERopeGameState::Playing;
	OnGameStateChanged(CurrentState);
}
 
void ARopeGameMode::RestartLevel()
{
	// 현재 레벨 재로드
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}