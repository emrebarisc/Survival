// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"
#include "OpenableObject.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AOpenableObject : public AInteractableObject
{
	GENERATED_BODY()


public:
	AOpenableObject();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay();

	void SetIsInteracted(bool _isInteracted) override;

	UPROPERTY(EditAnywhere, Category = "Openable")
	unsigned int _key;

	UPROPERTY(EditAnywhere, Category = "Openable")
	bool _locked;

private:
	
	bool _animate;
	bool _openClose = false; // 1 - open, 0 - close

	UPROPERTY(EditAnywhere, Category = "Openable")
	bool _openDirection = true;

	float initialRotationYaw;
	float _openSpeed;
};
