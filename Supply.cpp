// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "Supply.h"
#include "SurvivalCharacter.h"
#include "Inventory.h"

ASupply::ASupply() 
{
	_canBePutToInventory = true;
}

void ASupply::SetIsInteracted(bool newIsInteracted)
{
	Super::SetIsInteracted(newIsInteracted);
}

void ASupply::Use()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASurvivalCharacter* SurvivalCharacter = Cast<ASurvivalCharacter>(PlayerCharacter);

	if (SurvivalCharacter)
	{
		SurvivalCharacter->ApplySupply(_EnergySupply, _HungerSupply, _ThirstSupply, _HealthSupply);
		SurvivalCharacter->GetPlayerInventory()->RemoveFromInventory(this);
		
		this->Destroy();
	}
}
