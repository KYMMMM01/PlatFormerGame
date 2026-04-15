// Fill out your copyright notice in the Description page of Project Settings.


#include "RopeCharacter.h"

// Sets default values
ARopeCharacter::ARopeCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARopeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARopeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARopeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

