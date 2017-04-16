// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "InteractableObject.h"
#include "SurvivalCharacter.h"
#include "Inventory.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableObjectStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Object Mesh"));
	InteractableObjectStaticMesh->SetupAttachment(RootComponent);

	InteractableObjectStaticMesh->SetSimulatePhysics(true);
	IsInteracted = false;

}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

bool AInteractableObject::GetIsInteracted()
{
	return IsInteracted;
}

void AInteractableObject::SetIsInteracted(bool newIsInteracted)
{
	IsInteracted = newIsInteracted;
}

void AInteractableObject::SetPhysicsEnabled()
{

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASurvivalCharacter* SurvivalCharacter = Cast<ASurvivalCharacter>(PlayerCharacter);
	FVector CameraForwardVector = SurvivalCharacter->GetFirstPersonCamera()->GetForwardVector();

	this->InteractableObjectStaticMesh->SetSimulatePhysics(true);
	this->InteractableObjectStaticMesh->AddForce(CameraForwardVector);

	GetWorld()->GetTimerManager().ClearTimer(InteractableObjectTimerHandler);
}

void AInteractableObject::Use() {}

void AInteractableObject::Drop()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASurvivalCharacter* SurvivalCharacter = Cast<ASurvivalCharacter>(PlayerCharacter);

	FVector StartPoint = SurvivalCharacter->GetFirstPersonCamera()->GetComponentLocation();
	FVector CameraForwardVector = SurvivalCharacter->GetFirstPersonCamera()->GetForwardVector();

	this->SetActorEnableCollision(true);
	this->SetActorHiddenInGame(false);
	this->SetActorLocation(StartPoint + CameraForwardVector * 4);
	SurvivalCharacter->GetPlayerInventory()->RemoveFromInventory(this);
	GetWorld()->GetTimerManager().SetTimer(InteractableObjectTimerHandler, this, &AInteractableObject::SetPhysicsEnabled, 0.001f);
}