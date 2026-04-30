#include "RopeGameMode.h"
#include "RopeCharacter.h"
#include "RopePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AudioLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "CoinActor.h"

ARopeGameMode::ARopeGameMode()
{
	DefaultPawnClass = ARopeCharacter::StaticClass();
	PlayerControllerClass = ARopePlayerController::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}
 
void ARopeGameMode::BeginPlay()
{
	Super::BeginPlay();
 
	//플레이어 시작 위치(리스폰 때 사용)
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
	OnGameStateChanged.Broadcast(CurrentState);
	
	TArray<AActor*> Coins;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoinActor::StaticClass(), Coins);
	TotalCoins = Coins.Num();
	CollectedCoins = 0;
	
	TimeRemaining = TimeLimit;
	bIsTimmerRunning = (TimeLimit > 0.0f);
	
	if (StageBGM)
	{
		StageBGMComp = UGameplayStatics::SpawnSound2D(this, StageBGM);
	}
}

 
void ARopeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsTimmerRunning) return;
	if (CurrentState != ERopeGameState::Playing) return;
	
	TimeRemaining -= DeltaTime;
	if (TimeRemaining <= 0.0f)
	{
		TimeRemaining = 0.0f;
		bIsTimmerRunning = false;
		OnPlayerFell();
	}
}
void ARopeGameMode::OnLevelCleared()
{
	if (CurrentState != ERopeGameState::Playing) return;
 
	CurrentState = ERopeGameState::Cleared;
	OnGameStateChanged.Broadcast(CurrentState);
 
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		//클리어 위젯 표시
		if (ClearWidgetClass)
		{
			ClearWidget = CreateWidget<UUserWidget>(PC, ClearWidgetClass);
			if (ClearWidget)
			{
				ClearWidget->AddToViewport();
			}
		}
		
		if (StageBGMComp && StageBGMComp->IsPlaying())
		{
			StageBGMComp->FadeOut(0.5f, 0.f);
		}
		
		if (AudioLibrary && AudioLibrary->ClearBGM)
		{
			ClearBGMComp = UGameplayStatics::SpawnSound2D(this, AudioLibrary->ClearBGM);
		}
	
		//입력 비활성
		PC->SetPause(true);
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}
}
 
void ARopeGameMode::OnPlayerFell()
{
	if (CurrentState != ERopeGameState::Playing) return;
 
	CurrentState = ERopeGameState::GameOver;
	OnGameStateChanged.Broadcast(CurrentState);
 
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		//게임 오버
		if (GameOverWidgetClass)
		{
			GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();
			}
		}
		
		if (StageBGMComp && StageBGMComp->IsPlaying())
		{
			StageBGMComp->FadeOut(0.5f, 0.f);
		}
 		
 		if (AudioLibrary && AudioLibrary->GameOver)
        {
        	UGameplayStatics::PlaySound2D(this, AudioLibrary->GameOver);
        }
        
		//마우스 커서 노출, UI 입력
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}
}
 
void ARopeGameMode::RespawnPlayer()
{
	if (ARopeCharacter* Player = Cast<ARopeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Player->Respawn(PlayerStartLocation);
	}
	CurrentState = ERopeGameState::Playing;
	OnGameStateChanged.Broadcast(CurrentState);
}
 
void ARopeGameMode::RestartLevel()
{
	//현재 레벨 재시작
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
}

void ARopeGameMode::OnCoinCollected(const FVector& CoinLocation)
{
	if  (CurrentState != ERopeGameState::Playing) return;
	
	CollectedCoins++;
	
	//사운드 추가
	if (AudioLibrary && AudioLibrary->Coin)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AudioLibrary->Coin, CoinLocation);
	}
	
	//코인 전부 모으면 => 클리어
	if (CollectedCoins >= TotalCoins && TotalCoins > 0)
	{
		bIsTimmerRunning = false;
		OnLevelCleared();
	}
}

