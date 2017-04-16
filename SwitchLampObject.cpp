// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "SwitchLampObject.h"


ASwitchLampObject::ASwitchLampObject()
{
	SwitchLampObjectLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	SwitchLampObjectLightComponent->Intensity = 5000.f;
	SwitchLampObjectLightComponent->AttenuationRadius = 1000.f;
	SwitchLampObjectLightComponent->bAffectsWorld = true;
	SwitchLampObjectLightComponent->Mobility = EComponentMobility::Stationary;

	SwitchLampObjectSwitchComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchComponent"));

}

void ASwitchLampObject::SetIsInteracted(bool newIsInteracted)
{
	Super::SetIsInteracted(newIsInteracted);
	SwitchLampObjectLightComponent->SetVisibility(GetIsInteracted());

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Light is open: %d"), GetIsOpen()));
}