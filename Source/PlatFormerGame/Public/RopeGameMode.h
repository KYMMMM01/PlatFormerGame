#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RopeGameMode.generated.h"

UENUM(BlueprintType)
enum class ERopeGameState : uint8
{
	Playing, Cleared, GameOver
};

UCLASS()
class PLATFORMERGAME_API ARopeGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ARopeGameMode();
	
	//현재 게임 상태
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	ERopeGameState CurrentState = ERopeGameState::Playing;
 
	//낙사 기준 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float KillZ = -2000.f;
 
	//리스폰 위치
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	FVector PlayerStartLocation = FVector::ZeroVector;
 
	//리스폰까지 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float RespawnDelay = 1.5f;
 
	//HUD 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDWidget = nullptr;
 
	//골 지점 도달 시
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnLevelCleared();
 
	//플레이어 낙사 시
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnPlayerFell();
 
	//플레이어 리스폰
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RespawnPlayer();
 
	//레벨 재시작 (클리어/게임오버 화면에서 버튼으로 호출)
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartLevel();
 
	//UI가 구독할 수 있는 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnGameStateChanged(ERopeGameState NewState);
 
protected:
	virtual void BeginPlay() override;
};
