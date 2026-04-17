#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RopeCharacter.generated.h"

class ARopePlayerController;  // 추가
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
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
	
	
	//이동 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 900.f; //이동 속도(기본적으로 달리는 상태)	
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookSensitivity = 1.0f; //마우스 감도
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpSpeed = 700.f; //점프 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DashSpeed = 2000.f; //대쉬 속도
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityAccel = -980.f; //중력 가속도
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundCheckDistance = 100.f; //지면 감지 거리
	
	
	
protected:
	float VerticalVelocity = 0.f; //현재 수직 속도
	bool  bIsGrounded = false; //땅에 닿아있는지 여부
	bool bIsGrappling = false;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void ApplyGravity(float DeltaTime);
	void CheckGrounded();
	
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
	
};
