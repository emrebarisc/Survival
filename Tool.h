// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "Tool.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ATool : public AInteractableObject
{
	GENERATED_BODY()

public:
	ATool();

	virtual void SetIsInteracted(bool newIsInteracted) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Tool)
	float _damage;

	UPROPERTY(EditAnywhere, Category = Tool)
	float _durability;
private:
	USkeletalMeshSocket* toolSocket;
	
};
