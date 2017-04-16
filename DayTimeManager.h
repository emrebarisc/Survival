// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DayTimeManager.generated.h"

UCLASS()
class SURVIVAL_API ADayTimeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayTimeManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Daylight")
	UDirectionalLightComponent* DirectionalLightComponent;
	
private:
	float LastTimeOfTheDay;
	float TimeSinceLastIncrement;
};
