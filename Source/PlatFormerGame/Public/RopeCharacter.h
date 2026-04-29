#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RopeCharacter.generated.h"

class ARopePlayerController;
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UAudioLibrary;
struct FInputActionValue;

UCLASS()
class PLATFORMERGAME_API ARopeCharacter : public APawn
{
	GENERATED_BODY()

public:
	ARopeCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	//로프 시각화
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RopeMesh;

	//갈고리 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	FName HandSocketName = TEXT("RightHand");
	
	//오디오 라이브러리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<UAudioLibrary> AudioLibrary = nullptr;


	//이동 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 900.f; //이동 속도(기본적으로 달리는 상태)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookSensitivity = 1.0f; //마우스 감도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpRange = 1500.f; //점프 정도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashSpeed = 2000.f; //대쉬 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashDuration = 0.2f; //대쉬 지속 시간(초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashCooldown = 1.0f; //대쉬 쿨다운(초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityAccel = -1960.f; //중력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundCheckDistance = 100.f; //지면 감지 거리

	//로프 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RopeMaxLength = 1300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RopeMinLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GrappleTraceDistance = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PullDuration = 1.f;
	
	//리스폰
	UFUNCTION(BlueprintCallable, Category = "Game")
	void Respawn(const FVector& NewLocation);


	//수평 이동 속도 (Idle/Run Blend)
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float CurrentSpeed = 0.f;

	//땅에 닿아있는지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsGrounded = false;

	//로프 그래플링 중인지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsGrappling = false;

	//공중에 있는지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsInAir = false;

	//대쉬 중인지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsDashing = false;
	
	//갈고리 발사
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope")
	float HookFlightSpeed = 8000.f;

protected:
	float VerticalVelocity = 0.f; //현재 수직 속도

	FVector PreviousLocation = FVector::ZeroVector; //이전 위치
	
	FVector GrapplePoint = FVector::ZeroVector;
	float CurrentRopeLength = 0.f;
	FVector GrappleVelocity = FVector::ZeroVector;
	
	//대쉬 상태
	float DashTimeRemaining = 0.f;
	float DashCooldownRemaining = 0.f;
	FVector DashDirection = FVector::ZeroVector;
	
	//갈고리 발사 상태
	bool bIsHookFlying = false;
	float HookFlightTime = 0.f;
	float HookFlightDuration = 0.f;
	FVector HookFlightStart = FVector::ZeroVector;
	FVector HookFlightTarget = FVector::ZeroVector;
	
	//끌어 당기기 상태
	bool bIsPulling = false;
	float PullTime = 0.f;
	FVector PullStartLocation = FVector::ZeroVector;
	
	//발자국 타이머
	float FootstepTimer = 0.f;
	//Land감지
	bool bWasGrounded = false;
	
	UPROPERTY()
	AActor* GrappleActor = nullptr;

	FVector GrappleLocalOffset = FVector::ZeroVector;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyGravity(float DeltaTime);
	void CheckGrounded();
	void UpdateDash(float DeltaTime);
	void UpdateHangPose(float DeltaTime);
	void UpdateRopeVisual(const FVector& HandLoc, const FVector& EndLoc);

	void CheckKillZ();

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void StartDash(const FInputActionValue& Value);
	UFUNCTION()
	void StopDash(const FInputActionValue& Value);

	UFUNCTION()
	void StartGrapple();
	UFUNCTION()
	void StopGrapple();
	UFUNCTION()
	void ApplyGrapplePhysics(float DeltaTime);
	
	UFUNCTION()
	void StartPull(const FInputActionValue& Value);
	UFUNCTION()
	void UpdatePull(float DeltaTime);
};