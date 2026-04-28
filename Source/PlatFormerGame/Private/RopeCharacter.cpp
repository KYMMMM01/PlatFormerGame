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
#include "DrawDebugHelpers.h"


ARopeCharacter::ARopeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCapsuleHalfHeight(88.f);
	CapsuleComp->SetCapsuleRadius(35.f);
	CapsuleComp->SetSimulatePhysics(false); 

	//콜리전
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionObjectType(ECC_Pawn);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CapsuleComp->SetGenerateOverlapEvents(true);
	
	//스켈레탈 메쉬
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	
	//스프링암
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = false;
	
	//카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
	
	//로프 케이블
	RopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("RopeMesh"));
	RopeMesh->SetupAttachment(CapsuleComp);
	RopeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RopeMesh->SetVisibility(false);
	RopeMesh->SetUsingAbsoluteLocation(true);
	RopeMesh->SetUsingAbsoluteRotation(true);
	RopeMesh->SetUsingAbsoluteScale(true);

	//갈고리 메쉬
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HookMesh"));
	HookMesh->SetupAttachment(CapsuleComp);
	HookMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HookMesh->SetVisibility(false);
	HookMesh->SetUsingAbsoluteLocation(true);
	HookMesh->SetUsingAbsoluteRotation(true);
}

void ARopeCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(0.f, 0.f, 0.f));

	//AnimBP용 속도 계산 초기값
	PreviousLocation = GetActorLocation();
}


void ARopeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGrappling)
	{
		ApplyGrapplePhysics(DeltaTime);
		
		FVector HandLoc = GetActorLocation();
		if (SkeletalMeshComp && SkeletalMeshComp->DoesSocketExist(HandSocketName))
		{
			HandLoc = SkeletalMeshComp->GetSocketLocation(HandSocketName);
		}

		//로프
		if (RopeMesh)
		{
			const FVector ToGrapple = GrapplePoint - HandLoc;
			const float Distance = ToGrapple.Size();

			if (Distance > KINDA_SMALL_NUMBER)
			{
				RopeMesh->SetWorldLocation((HandLoc + GrapplePoint) * 0.5f);
				
				const FQuat AlignZ = FQuat::FindBetweenNormals(FVector::UpVector, ToGrapple.GetSafeNormal());
				RopeMesh->SetWorldRotation(AlignZ);
				
				RopeMesh->SetWorldScale3D(FVector(0.03f, 0.03f, Distance / 100.f));
			}
		}

		//갈고리
		if (HookMesh)
		{
			HookMesh->SetWorldLocation(GrapplePoint);
			const FVector ToChar = GetActorLocation() - GrapplePoint;
			if (!ToChar.IsNearlyZero())
			{
				HookMesh->SetWorldRotation(ToChar.Rotation());
			}
		}
	}
	else
	{
		CheckGrounded();
		ApplyGravity(DeltaTime);
		
	}
	UpdateDash(DeltaTime);
	UpdateHangPose(DeltaTime);
	CheckKillZ();
	
	//수평(XY) 이동 속도
	const FVector CurrentLocation = GetActorLocation();
	const FVector Delta = CurrentLocation - PreviousLocation;
	const FVector HorizontalDelta(Delta.X, Delta.Y, 0.f);
	CurrentSpeed = (DeltaTime > KINDA_SMALL_NUMBER)
		? HorizontalDelta.Size() / DeltaTime
		: 0.f;
	PreviousLocation = CurrentLocation;

	//공중 상태
	bIsInAir = !bIsGrounded && !bIsGrappling;
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
				ETriggerEvent::Started,
				this,
				&ARopeCharacter::StartDash
				);
			}
			if (PlayerController->IA_Dash)
			{
				EnhancedInput->BindAction(
				PlayerController->IA_Dash,
				ETriggerEvent::Completed,
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

	//좌우(Yaw)
	AddActorLocalRotation(FRotator(0.0f, LookInput.X * LookSensitivity, 0.0f));

	//상하(Pitch)
	FRotator CurrentArm = SpringArmComp->GetRelativeRotation();
	float NewPitch = FMath::Clamp(CurrentArm.Pitch - LookInput.Y * LookSensitivity, -80.f, 80.f);
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
	if (!bIsGrappling) return;
	if (DashCooldownRemaining > 0.f) return;

	//액터 정면(수평)
	FVector Forward = GetActorForwardVector();
	Forward.Z = 0.f;
	Forward = Forward.GetSafeNormal();
	if (Forward.IsNearlyZero()) return;
	
	const FVector ToChar = GetActorLocation() - GrapplePoint;
	const FVector RopeDir = ToChar.GetSafeNormal();
	FVector TangentDir = Forward - RopeDir * FVector::DotProduct(Forward, RopeDir);
	TangentDir = TangentDir.GetSafeNormal();
	
	if (TangentDir.IsNearlyZero())
	{
		TangentDir = Forward;
	}
	
	GrappleVelocity += TangentDir * DashSpeed;

	bIsDashing = true;
	DashTimeRemaining = DashDuration;
	DashCooldownRemaining = DashCooldown;
	DashDirection = TangentDir;
}
void ARopeCharacter::StopDash(const FInputActionValue& Value)
{

}

void ARopeCharacter::UpdateDash(float DeltaTime)
{
	//쿨다운
	if (DashCooldownRemaining > 0.f)
	{
		DashCooldownRemaining = FMath::Max(0.f, DashCooldownRemaining - DeltaTime);
	}

	if (bIsDashing)
	{
		DashTimeRemaining -= DeltaTime;
		if (DashTimeRemaining <= 0.f)
		{
			bIsDashing = false;
			DashTimeRemaining = 0.f;
		}
	}
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

	//수직 이동 
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

//----------------------------------낙사/리스폰--------------------------------------
void ARopeCharacter::CheckKillZ()
{
	if (ARopeGameMode* GM = Cast<ARopeGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GetActorLocation().Z < GM->KillZ)
		{
			GM->OnPlayerFell();
		}
	}
}

void ARopeCharacter::Respawn(const FVector& NewLocation)
{
	//상태 초기화
	bIsGrappling       = false;
	GrappleVelocity    = FVector::ZeroVector;
	GrapplePoint       = FVector::ZeroVector;
	GrappleActor       = nullptr;
	GrappleLocalOffset = FVector::ZeroVector;
	VerticalVelocity   = 0.f;
	bIsGrounded        = false;
	bIsDashing            = false;            
	DashTimeRemaining     = 0.f;               
	DashCooldownRemaining = 0.f;               
	DashDirection         = FVector::ZeroVector;

	if (RopeMesh)
	{
		RopeMesh->SetVisibility(false);
	}
	if (HookMesh)
	{
		HookMesh->SetVisibility(false);
	}
	
	//위치 이동
	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
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

        //히트된 Actor 저장
        GrappleActor = Hit.GetActor();
        if (GrappleActor)
        {
            //월드 히트 지점을 해당 Actor 로컬 좌표계로 역변환해 저장
            GrappleLocalOffset = GrappleActor->GetActorTransform().InverseTransformPosition(Hit.ImpactPoint);
        }
        else
        {
            GrappleLocalOffset = FVector::ZeroVector;
        }

        //자동 길이 조정
        float Distance = FVector::Distance(GetActorLocation(), GrapplePoint);
        CurrentRopeLength = FMath::Clamp(Distance, RopeMinLength, RopeMaxLength);

        bIsGrappling = true;
        bIsGrounded  = false;

        GrappleVelocity  = FVector(0.f, 0.f, VerticalVelocity);
        VerticalVelocity = 0.f;
    	
    	if (RopeMesh)
    	{
    		RopeMesh->SetVisibility(true);
    	}
    	if (HookMesh)
    	{
    		HookMesh->SetWorldLocation(GrapplePoint);
    		HookMesh->SetVisibility(true);
    	}
    }
}

void ARopeCharacter::StopGrapple()
{
    if (!bIsGrappling) return;

    VerticalVelocity = GrappleVelocity.Z;
    bIsGrappling     = false;
    GrappleVelocity  = FVector::ZeroVector;
	
    GrappleActor       = nullptr;
    GrappleLocalOffset = FVector::ZeroVector;
	
	if (RopeMesh)
	{
		RopeMesh->SetVisibility(false);
	}
	if (HookMesh)
	{
		HookMesh->SetVisibility(false);
	}
}

//스윙
void ARopeCharacter::ApplyGrapplePhysics(float DeltaTime)
{
    //움직이는 플랫폼 추적
    if (GrappleActor)
    {
        GrapplePoint = GrappleActor->GetActorTransform().TransformPosition(GrappleLocalOffset);
    }

    GrappleVelocity.Z += GravityAccel * DeltaTime;

    FHitResult Hit;
    AddActorWorldOffset(GrappleVelocity * DeltaTime, true, &Hit);
    if (Hit.IsValidBlockingHit())
    {
    	if (GrappleVelocity.Z < 0.f)
    	{
    		bIsGrounded = true;
    	}
    	GrappleVelocity = FVector::ZeroVector;
    	return;
    }

	FVector ToChar = GetActorLocation() - GrapplePoint;
	float Dist = ToChar.Size();
	if (Dist < 1.f) return;

	FVector Dir = ToChar / Dist;

	//로프 길이 제한: SetActorLocation 대신 AddActorWorldOffset으로 Sweep 확실히 적용
	FVector DesiredLocation = GrapplePoint + Dir * CurrentRopeLength;
	FVector Delta = DesiredLocation - GetActorLocation();
	FHitResult ConstraintHit;
	AddActorWorldOffset(Delta, true, &ConstraintHit);

	if (ConstraintHit.IsValidBlockingHit())
	{
		bIsGrounded = GrappleVelocity.Z < 0.f;
		GrappleVelocity = FVector::ZeroVector;
		return;
	}

	float RadialSpeed = FVector::DotProduct(GrappleVelocity, Dir);
	GrappleVelocity -= Dir * RadialSpeed;
}

void ARopeCharacter::UpdateHangPose(float DeltaTime)
{
	if (!SkeletalMeshComp) return;
	
	const float InterpSpeed = 8.f;
 
	if (bIsGrappling)
	{
		const FVector AnchorDir = (GrapplePoint - GetActorLocation()).GetSafeNormal();
		if (AnchorDir.IsNearlyZero()) return;
		
		const float LeanAmount = 0.3f;
		const FVector BlendedUp = FMath::Lerp(FVector::UpVector, AnchorDir, LeanAmount).GetSafeNormal();
		
		//메쉬가 정면을 보던 방향(액터 Forward)을 유지하면서, "위"가 로프 쪽을 향하도록 회전 구성.
		const FRotator TargetWorldRot = FRotationMatrix::MakeFromZY(BlendedUp, GetActorForwardVector()).Rotator();
 
		const FRotator CurrentWorldRot = SkeletalMeshComp->GetComponentRotation();
		const FRotator NewWorldRot = FMath::RInterpTo(CurrentWorldRot, TargetWorldRot, DeltaTime, InterpSpeed);
 
		SkeletalMeshComp->SetWorldRotation(NewWorldRot);
	}
	else
	{
		//그래플 끝나면 원래 로컬 회전으로 복귀
		const FRotator BaseLocal(0.f, -90.f, 0.f);
		const FRotator CurrentLocal = SkeletalMeshComp->GetRelativeRotation();
		const FRotator NewLocal =
			FMath::RInterpTo(CurrentLocal, BaseLocal, DeltaTime, InterpSpeed);
 
		SkeletalMeshComp->SetRelativeRotation(NewLocal);
	}
}