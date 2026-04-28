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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RopeMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HookMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	FName HandSocketName = TEXT("RightHand");


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

	//리스폰
	UFUNCTION(BlueprintCallable, Category = "Game")
	void Respawn(const FVector& NewLocation);
	
	
	//수평 이동 속도 (XY 평면 크기). Idle/Run
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float CurrentSpeed = 0.f;
 
	//땅에 닿아있는지 (Idle/Run => Fall 전환)
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsGrounded = false;
 
	//로프 그래플링 중인지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsGrappling = false;
 
	//공중에 있는지 Jump/Fall
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsInAir = false;
	
	//대쉬 하는지
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsDashing = false;

protected:
	float VerticalVelocity = 0.f; //현재 수직 속도
	
	FVector PreviousLocation = FVector::ZeroVector; //이전 위치
	
	FVector GrapplePoint = FVector::ZeroVector; //로프가 닿는 위치
	float CurrentRopeLength = 0.f;
	FVector GrappleVelocity = FVector::ZeroVector;

	float DashTimeRemaining = 0.f;   //남은 대쉬 지속 시간
	float DashCooldownRemaining = 0.f;   //남은 쿨다운
	FVector DashDirection = FVector::ZeroVector; //대쉬 시작 시 고정한 방향
	
	//움직이는 플랫폼에 로프를 건 경우, 해당 Actor를 추적해 매 프레임 GrapplePoint를 따라가게 함
	UPROPERTY()
	AActor* GrappleActor = nullptr;

	//GrappleActor의 로컬 좌표 기준 로프 부착점
	FVector GrappleLocalOffset = FVector::ZeroVector;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyGravity(float DeltaTime);
	void CheckGrounded();
	void UpdateDash(float DeltaTime);
	void UpdateHangPose(float DeltaTime);
	
	//낙사 체크
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
	
};