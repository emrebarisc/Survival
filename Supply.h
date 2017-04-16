// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "Supply.generated.h"

UCLASS()
class SURVIVAL_API ASupply : public AInteractableObject
{
	GENERATED_BODY()

public:
	ASupply();
	virtual void SetIsInteracted(bool newIsInteracted) override;

	void Use() override;
	
	UPROPERTY(EditAnywhere, Category = "Interactable Object | Supply")
	float _EnergySupply;

	UPROPERTY(EditAnywhere, Category = "Interactable Object | Supply")
	float _HungerSupply;

	UPROPERTY(EditAnywhere, Category = "Interactable Object | Supply")
	float _ThirstSupply;

	UPROPERTY(EditAnywhere, Category = "Interactable Object | Supply")
	float _HealthSupply;
};
