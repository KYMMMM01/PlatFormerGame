#include "RopeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "RopePlayerController.h"
#include "RopeGameMode.h"
#include "Kismet/GameplayStatics.h"

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
	
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


void ARopeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsGrappling)
	{
		ApplyGrapplePhysics(DeltaTime);	
	}
	else
	{
		CheckGrounded();
		ApplyGravity(DeltaTime);
	}
	
	CheckKillZone();
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
			
			if (PlayerController->IA_Grapple)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Grapple,
					ETriggerEvent::Started,
					this,
					&ARopeCharacter::StartGrapple
					);
			}
			if (PlayerController->IA_Grapple)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Grapple,
					ETriggerEvent::Completed,
					this,
					&ARopeCharacter::StopGrapple
					);
			}
		}
	}
}

//--------------------------------이동 관련---------------------------------------
void ARopeCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	const FVector2D MoveInput = Value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y))
	{	
		FVector Foward = GetActorForwardVector();
		FVector Right = GetActorRightVector();
		
		FVector MoveOffset = Right * MoveInput.X + Foward * MoveInput.Y;
		//충돌 감지(bSweep = true) 하면서 Offset 방향으로 Speed의 속도로 이번 프레임에 해당하는 만큼 움직여라
		AddActorWorldOffset(MoveOffset * MoveSpeed * GetWorld()->GetDeltaSeconds(), true); 
	}
}
void ARopeCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	
	//좌우 (Yaw)
	AddActorLocalRotation(FRotator(0.0f, LookInput.X * LookSensitivity, 0.0f));
	
	//상하 (Pitch)
	FRotator CurrentArm = SpringArmComp->GetRelativeRotation();
	float NewPitch = FMath::Clamp(CurrentArm.Pitch - LookInput.Y * LookSensitivity, -60.f, 30.f);
	SpringArmComp->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
	
}
void ARopeCharacter::StartJump(const FInputActionValue& Value)
{
	//그래플링 중 점프
	if (bIsGrappling)
	{
		VerticalVelocity = GrappleVelocity.Z + JumpRange;
		bIsGrappling = false;
		GrappleVelocity = FVector::ZeroVector;
		bIsGrounded = false;
	}
	else if (bIsGrounded)
	{
		VerticalVelocity = JumpRange;
		bIsGrounded = false;
	}
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
	if (!bIsGrounded)
	{
		VerticalVelocity += GravityAccel * DeltaTime;
	}
	else
	{
		VerticalVelocity = 0.f;
	}

	//수직 이동 적용
	FHitResult Hit;
	AddActorWorldOffset(FVector(0.f, 0.f, VerticalVelocity * DeltaTime), true, &Hit);

	//충돌 시 초기화
	if (Hit.IsValidBlockingHit())
	{
		if (VerticalVelocity < 0.f) bIsGrounded = true;
		VerticalVelocity = 0.f;
	}
}

void ARopeCharacter::CheckGrounded()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, GroundCheckDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	bIsGrounded = bHit && (VerticalVelocity <= 0.f);
}

//----------------------------------로프 관련-------------------------------------------
void ARopeCharacter::StartGrapple()
{
    //카메라 방향으로 LineTrace
    FVector Start = CameraComp->GetComponentLocation();
    FVector End = Start + CameraComp->GetForwardVector() * GrappleTraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        GrapplePoint = Hit.ImpactPoint;

        //자동 길이 조정
        float Distance = FVector::Distance(GetActorLocation(), GrapplePoint);
        CurrentRopeLength = FMath::Clamp(Distance, RopeMinLength, RopeMaxLength);

        bIsGrappling = true;
        bIsGrounded  = false;
    	
        GrappleVelocity  = FVector(0.f, 0.f, VerticalVelocity);
        VerticalVelocity = 0.f;
    }
}

void ARopeCharacter::StopGrapple()
{
    if (!bIsGrappling) return;
	
    VerticalVelocity = GrappleVelocity.Z;
    bIsGrappling     = false;
    GrappleVelocity  = FVector::ZeroVector;
}

//스윙
void ARopeCharacter::ApplyGrapplePhysics(float DeltaTime)
{
    GrappleVelocity.Z += GravityAccel * DeltaTime;
	
    FHitResult Hit;
    AddActorWorldOffset(GrappleVelocity * DeltaTime, true, &Hit);
    if (Hit.IsValidBlockingHit())
    {
    	if (GrappleVelocity.Z < 0.f)
    	{
    		bIsGrounded = true;
    		StopGrapple();
    		return;
    	}
    	GrappleVelocity = FVector::ZeroVector;
    	return;
    }
	
	FVector ToChar = GetActorLocation() - GrapplePoint;
	float Dist = ToChar.Size();
	if (Dist < 1.f) return;

	FVector Dir = ToChar / Dist;

	//충돌 체크
	FHitResult ConstraintHit;
	SetActorLocation(GrapplePoint + Dir * CurrentRopeLength, true, &ConstraintHit);

	if (ConstraintHit.IsValidBlockingHit())
	{
		bIsGrounded = GrappleVelocity.Z < 0.f;
		StopGrapple();
		return;
	}

	float RadialSpeed = FVector::DotProduct(GrappleVelocity, Dir);
	GrappleVelocity -= Dir * RadialSpeed;
}

//---------------------------------낙사, 리스폰 관련----------------------------------------
void ARopeCharacter::CheckKillZone()
{
	if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GetActorLocation().Z < GM->KillZ)
			GM->OnPlayerFell();
	}
}

void ARopeCharacter::Respawn(const FVector& NewLocation)
{
	bIsGrappling     = false;
	GrappleVelocity  = FVector::ZeroVector;
	GrapplePoint     = FVector::ZeroVector;
	VerticalVelocity = 0.f;
	bIsGrounded      = false;

	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
}