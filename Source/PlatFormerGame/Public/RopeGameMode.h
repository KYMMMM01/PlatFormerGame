#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RopeGameMode.generated.h"

class UAudioLibrary;
class UAudioComponent;
class ACoinActor;

UENUM(BlueprintType)
enum class ERopeGameState : uint8
{
	Playing, Cleared, GameOver
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, ERopeGameState, NewState);

UCLASS()
class PLATFORMERGAME_API ARopeGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ARopeGameMode();
	
	//현재 게임 상태
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	ERopeGameState CurrentState = ERopeGameState::Playing;
 
	//낙사 기준 Z 높이 (이 값보다 아래로 떨어지면 게임오버)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float KillZ = -2000.f;
 
	//리스폰 위치 (BeginPlay에서 플레이어 시작 위치로 자동 설정)
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	FVector PlayerStartLocation = FVector::ZeroVector;
 
	//리스폰까지 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float RespawnDelay = 1.5f;
 
	//HUD 위젯 클래스 (에디터에서 UMG 위젯 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDWidget = nullptr;
 
	//게임 오버 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* GameOverWidget = nullptr;
 
	//레벨 클리어 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClearWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* ClearWidget = nullptr;
	
	//마지막 스테이지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	bool bIsLastStage = false;
	
	//스테이지 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FText StageInstruction;
	
	//오디오 라이브러리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<UAudioLibrary> AudioLibrary = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> StageBGMComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> StageBGM = nullptr;
	
	//클리어 BGM 재생 컴포넌트
	UPROPERTY()
	TObjectPtr<UAudioComponent> ClearBGMComp = nullptr;
	
	
	//골 지점 도달 시 호출
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnLevelCleared();
 
	//플레이어 낙사 시 호출
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnPlayerFell();
 
	//플레이어 리스폰
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RespawnPlayer();
 
	//레벨 재시작 (클리어/게임오버 화면에서 버튼으로 호출)
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartLevel();
	
	//UI가 구독할 수 있는 이벤트
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnGameStateChangedSignature OnGameStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	float TimeLimit = 60.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	float TimeRemaining = 0.f;

	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly, Category = "Stage")
	int32 TotalCoins = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	int32 CollectedCoins = 0;
	
	void OnCoinCollected(const FVector& CoinLocation);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	bool bIsTimmerRunning = false;
};
