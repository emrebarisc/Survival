// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "KeyObject.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AKeyObject : public AInteractableObject
{
	GENERATED_BODY()

public:
	AKeyObject();
	virtual void BeginPlay();

	unsigned int GetKey() { return _key; }

private:

	UPROPERTY(EditAnywhere, Category = "Key")
	unsigned int _key;
	
};
