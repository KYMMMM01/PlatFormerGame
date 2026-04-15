#include "RopeCharacter.h"

#include <rapidjson/document.h>

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "RopePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ARopeCharacter::ARopeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCapsuleHalfHeight(88.f);
	CapsuleComp->SetCapsuleRadius(35.f);
	CapsuleComp->SetSimulatePhysics(false); //코드로 직접 제어
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void ARopeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void ARopeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARopeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ARopePlayerController* PlayerController = Cast<ARopePlayerController>(GetController()))
		{
			if (PlayerController->IA_Move)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Move,
				ETriggerEvent::Triggered,
				this,
				&ARopeCharacter::Move
				);
			}
			if (PlayerController->IA_Jump)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Jump,
				ETriggerEvent::Started,
				this,
				&ARopeCharacter::StartJump
				);
			}
			if (PlayerController->IA_Jump)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Jump,
				ETriggerEvent::Completed,
				this,
				&ARopeCharacter::StopJump
				);
			}
			if (PlayerController->IA_Look)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Look,
				ETriggerEvent::Triggered,
				this,
				&ARopeCharacter::Look
				);
			}
			if (PlayerController->IA_Dash)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Dash,
				ETriggerEvent::Triggered,
				this,
				&ARopeCharacter::StartDash
				);
			}
			if (PlayerController->IA_Dash)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Dash,
				ETriggerEvent::Triggered,
				this,
				&ARopeCharacter::StopDash
				);
			}
		}
	}
}

void ARopeCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	const FVector2D MoveInput = Value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y))
	{	
		FVector Foward = GetActorForwardVector();
		FVector Right = GetActorRightVector();
		
		FVector MoveOffset = MoveInput.X * Right + MoveInput.Y * Foward;
		AddActorLocalOffset(MoveOffset);
	}
}
void ARopeCharacter::Look(const FInputActionValue& Value)
{
		
}
void ARopeCharacter::StartJump(const FInputActionValue& Value)
{
		
}
void ARopeCharacter::StopJump(const FInputActionValue& Value)
{
		
}
void ARopeCharacter::StartDash(const FInputActionValue& Value)
{
		
}
void ARopeCharacter::StopDash(const FInputActionValue& Value)
{
		
}

void ARopeCharacter::ApplyGravity(float DeltaTime)
{
	
}

void ARopeCharacter::CheckGrounded()
{
	
}