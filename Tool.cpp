// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "Tool.h"
#include "SurvivalCharacter.h"

ATool::ATool()
{
	//InteractableObjectSkeletalMesh->SetSimulatePhysics(true);
	toolSocket = CreateDefaultSubobject<USkeletalMeshSocket>(TEXT("ToolSocket"));
	_canBePutToInventory = true;
}

void ATool::SetIsInteracted(bool newIsInteracted)
{
	Super::SetIsInteracted(newIsInteracted);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASurvivalCharacter* SurvivalCharacter = Cast<ASurvivalCharacter>(PlayerCharacter);

	if (SurvivalCharacter)
	{
		FVector ToolSocketLocation;
		ToolSocketLocation = SurvivalCharacter->GetMesh()->GetSocketLocation("ToolPoint");
		
		this->InteractableObjectStaticMesh->SetSimulatePhysics(false);
		AttachToComponent(SurvivalCharacter->GetPlayerMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("ToolPoint"));
		this->SetActorRelativeRotation(FRotator(-100.f, 0.0f, -150.f));
		this->SetActorRelativeLocation(FVector(1.0f, 3.3f, 8.73f));
		this->SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));
	}
}

void ATool::BeginPlay()
{
	Super::BeginPlay();
}
