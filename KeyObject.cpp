// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "KeyObject.h"

AKeyObject::AKeyObject()
{
	_key = 0;
}


void AKeyObject::BeginPlay()
{
	Super::BeginPlay();

	FString name = FString("Key #");
	name.AppendInt(_key);

	_interactableObjectName = FText::FromString(name);
}
