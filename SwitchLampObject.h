// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "SwitchLampObject.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ASwitchLampObject : public AInteractableObject
{
	GENERATED_BODY()

public:
	ASwitchLampObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
	UPointLightComponent* SwitchLampObjectLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* SwitchLampObjectSwitchComponent;

	virtual void SetIsInteracted(bool newIsInteracted) override;
	
};
